#include "Rasterizer.h"
#include "BasicRenderer.h"
#include "World.h"
#include "Material.h"
#include "SceneObject.h"
#include <atomic>
#include <chrono>
#include <future>
#include <iostream>
#include <thread>

namespace BasicRenderer
{
	void Rasterizer::Render(FrameBuffer& fBuffer, const RenderState& state, const ShadingFunc& Shading)
	{
		const uint width = fBuffer.GetWidth();
		const uint height = fBuffer.GetHeight();

		const auto beginTime = std::chrono::high_resolution_clock::now();

		// Phase 1: transform, clip and bin all faces into screen-space triangles
		m_triangles.clear();

		for (const auto& instance : state.m_instances)
		{
			if (instance != nullptr)
			{
				SetupInstance(width, height, state, *instance, Shading);
			}
		}

		if (m_triangles.empty())
		{
			return;
		}

		// Phase 2: rasterize in parallel. Each job owns disjoint chunks of rows,
		// pulled dynamically from a shared counter, so framebuffer writes never race.
		constexpr uint rowsPerChunk = 16u;
		const uint chunkCount = (height + rowsPerChunk - 1u) / rowsPerChunk;
		const uint threadCount = std::min(std::thread::hardware_concurrency() > 1u ? std::thread::hardware_concurrency() - 1u : 1u, chunkCount);

		std::atomic<uint> nextChunk = 0u;

		const auto job = [&]()
		{
			for (uint chunk = nextChunk.fetch_add(1u, std::memory_order_relaxed); chunk < chunkCount; chunk = nextChunk.fetch_add(1u, std::memory_order_relaxed))
			{
				const uint rowStart = chunk * rowsPerChunk;
				const uint rowEnd = std::min(rowStart + rowsPerChunk, height);
				RasterizeRows(fBuffer, width, height, rowStart, rowEnd);
			}
		};

		std::vector<std::future<void>> futures;
		futures.reserve(threadCount);
		for (uint i = 1u; i < threadCount; i++)
		{
			futures.push_back(std::async(std::launch::async, job));
		}
		job();

		for (auto& f : futures)
		{
			f.get();
		}

		const auto endTime = std::chrono::high_resolution_clock::now();
		const double ms = ConvertChronoDuration<double, std::chrono::milliseconds>(endTime - beginTime);
		std::cout << "Rasterized " << m_triangles.size() << " triangles with " << threadCount << " threads in " << ms << " ms\n";
	}

	void Rasterizer::SetupInstance(const uint width, const uint height, const RenderState& state, const MeshInstance& instance, const ShadingFunc& Shading)
	{
		// Only triangles meshes are supported for now
		if (instance.GetType() != PrimitiveType::FACE)
		{
			return;
		}

		const Camera& camera = state.m_camera;

		const float fwidth = static_cast<float>(width);
		const float fheight = static_cast<float>(height);

		const auto& mat = instance.GetMaterial();
		Color c = Material::MissingMaterialColor;
		const Matrix4 mvp = camera.GetProjectionMatrix() * camera.GetViewMatrix();

		const auto& faces = instance.GetFaces();
		for (size_t i = 0u; i < faces.size(); i++)
		{
			Face f = faces[i];

			if (mat)
			{
				c = Shading(*mat, state, Vector3::Zero(), f.GetNormal());
				c.r = c.r < 1.f ? c.r : 1.f;
				c.g = c.g < 1.f ? c.g : 1.f;
				c.b = c.b < 1.f ? c.b : 1.f;
			}

			ToMatrixSpace(f, mvp);

			Face clippedFaces[4];
			uint nClippedFaces = Clip(f, clippedFaces);

			for (uint j = 0u; j < nClippedFaces; j++)
			{
				f = clippedFaces[j];

				PerspectiveDivide(f);
				NormalizedToScreenSpace(f, fwidth, fheight);

				if (CullFace(f)) continue;

				const float x0 = f.v[0].pos.x, y0 = f.v[0].pos.y, z0 = f.v[0].pos.z;
				const float x1 = f.v[1].pos.x, y1 = f.v[1].pos.y, z1 = f.v[1].pos.z;
				const float x2 = f.v[2].pos.x, y2 = f.v[2].pos.y, z2 = f.v[2].pos.z;

				// Signed double area; faces surviving CullFace have area >= 0
				const float area = (x1 - x0) * (y2 - y0) - (y1 - y0) * (x2 - x0);
				if (area < 1e-2f) continue; // degenerate

				const Vector4 bbox = BoundingBox(f, fwidth, fheight);

				ScreenTriangle tri;
				tri.minX = static_cast<int>(bbox.x);
				tri.minY = static_cast<int>(bbox.y);
				tri.maxX = static_cast<int>(bbox.z);
				tri.maxY = static_cast<int>(bbox.w);
				if (tri.minX >= tri.maxX || tri.minY >= tri.maxY) continue;

				// Edge functions: w0 -> edge v1v2, w1 -> edge v2v0, w2 -> edge v0v1
				tri.dx0 = y1 - y2; tri.dy0 = x2 - x1;
				tri.dx1 = y2 - y0; tri.dy1 = x0 - x2;
				tri.dx2 = y0 - y1; tri.dy2 = x1 - x0;

				const float px = static_cast<float>(tri.minX);
				const float py = static_cast<float>(tri.minY);
				tri.w0 = (x2 - x1) * (py - y1) - (y2 - y1) * (px - x1);
				tri.w1 = (x0 - x2) * (py - y2) - (y0 - y2) * (px - x2);
				tri.w2 = (x1 - x0) * (py - y0) - (y1 - y0) * (px - x0);

				const float invArea = 1.0f / area;
				tri.bz0 = z0 * invArea;
				tri.bz1 = z1 * invArea;
				tri.bz2 = z2 * invArea;

				tri.color = c;

				m_triangles.push_back(tri);
			}
		}
	}

	void Rasterizer::RasterizeRows(FrameBuffer& fBuffer, const uint width, const uint height, const uint rowStart, const uint rowEnd) const
	{
		for (const ScreenTriangle& tri : m_triangles)
		{
			const int yBegin = std::max(tri.minY, static_cast<int>(rowStart));
			const int yEnd = std::min(tri.maxY, static_cast<int>(rowEnd));

			if (yBegin >= yEnd) continue;

			const float yOffset = static_cast<float>(yBegin - tri.minY);
			float w0row = tri.w0 + tri.dy0 * yOffset;
			float w1row = tri.w1 + tri.dy1 * yOffset;
			float w2row = tri.w2 + tri.dy2 * yOffset;

			for (int y = yBegin; y < yEnd; y++)
			{
				float w0 = w0row;
				float w1 = w1row;
				float w2 = w2row;

				//Because of the Pinhole model
				const uint rowIndex = width * (height - static_cast<uint>(y) - 1u);

				for (int x = tri.minX; x < tri.maxX; x++)
				{
					if (w0 >= 0.f && w1 >= 0.f && w2 >= 0.f)
					{
						const float z = w0 * tri.bz0 + w1 * tri.bz1 + w2 * tri.bz2;
						const uint index = rowIndex + static_cast<uint>(x);

						if (z < fBuffer.GetDepth(index))
						{
							fBuffer.WriteToColor(index, tri.color);
							fBuffer.WriteToDepth(index, z);
						}
					}

					w0 += tri.dx0;
					w1 += tri.dx1;
					w2 += tri.dx2;
				}

				w0row += tri.dy0;
				w1row += tri.dy1;
				w2row += tri.dy2;
			}
		}
	}

	inline void Rasterizer::PerspectiveDivide(Face& f) const noexcept
	{
		float v0w = 1.0f / f.v[0].pos.w;
		float v1w = 1.0f / f.v[1].pos.w;
		float v2w = 1.0f / f.v[2].pos.w;
		Vector4 v0 = Vector4(f.v[0].pos.x * v0w,
			f.v[0].pos.y * v0w,
			f.v[0].pos.z * v0w,
			f.v[0].pos.w);
		Vector4 v1 = Vector4(f.v[1].pos.x * v1w,
			f.v[1].pos.y * v1w,
			f.v[1].pos.z * v1w,
			f.v[1].pos.w);
		Vector4 v2 = Vector4(f.v[2].pos.x * v2w,
			f.v[2].pos.y * v2w,
			f.v[2].pos.z * v2w,
			f.v[2].pos.w);

		f = Face(v0, v1, v2, f);
	}

	inline void Rasterizer::NormalizedToScreenSpace(Face& f, const float fwidth, const float fheight) const noexcept
	{
		Vector4 v0 = Vector4(floorf(0.5f * fwidth * (f.v[0].pos.x + 1.0f)),
			floorf(0.5f * fheight * (f.v[0].pos.y + 1.0f)),
			f.v[0].pos.z,
			f.v[0].pos.w);
		Vector4 v1 = Vector4(floorf(0.5f * fwidth * (f.v[1].pos.x + 1.0f)),
			floorf(0.5f * fheight * (f.v[1].pos.y + 1.0f)),
			f.v[1].pos.z,
			f.v[1].pos.w);
		Vector4 v2 = Vector4(floorf(0.5f * fwidth * (f.v[2].pos.x + 1.0f)),
			floorf(0.5f * fheight * (f.v[2].pos.y + 1.0f)),
			f.v[2].pos.z,
			f.v[2].pos.w);

		f = Face(v0, v1, v2, f);
	}

	inline uint Rasterizer::Clip(const Face& f, Face(&clippedFaces)[4]) const noexcept
	{
		int nfaces = 0;

		if (f.v[0].pos.w <= 0.0 && f.v[1].pos.w <= 0.0 && f.v[2].pos.w <= 0.0) {
			return 0;
		}

		if (f.v[0].pos.w > 0.0 &&
			f.v[1].pos.w > 0.0 &&
			f.v[2].pos.w > 0.0 &&
			abs(f.v[0].pos.z) < f.v[0].pos.w &&
			abs(f.v[1].pos.z) < f.v[1].pos.w &&
			abs(f.v[2].pos.z) < f.v[2].pos.w)
		{
			clippedFaces[0] = f;
			return 1;
		}
		else
		{
			Vertex vertices[6];
			int size = 0;
			size = ClipEdge(f.v[0], f.v[1], vertices, size);
			size = ClipEdge(f.v[1], f.v[2], vertices, size);
			size = ClipEdge(f.v[2], f.v[0], vertices, size);

			// max size() is 6
			if (size < 3)
			{
				return 0;
			}
			if (vertices[size - 1] != vertices[0])
			{
				--size;
			}

			for (int i = 1; i < size - 1; ++i)
			{
				clippedFaces[i - 1] = Face(vertices[0], vertices[i], vertices[i + 1]);
				++nfaces;
			}
		}
		return nfaces;
	}

	inline uint Rasterizer::ClipEdge(const Vertex& v0, const Vertex& v1, Vertex(&vertices)[6], int index) const noexcept
	{
		assert(index < 5);

		int size = index;
		Vertex n_v0 = v0;
		Vertex n_v1 = v1;

		bool v0Inside = v0.pos.w > 0.0 && v0.pos.z > -v0.pos.w;
		bool v1Inside = v1.pos.w > 0.0 && v1.pos.z > -v1.pos.w;

		if (!v0Inside && !v1Inside)
		{
			return size;
		}
		else if (v0Inside != v1Inside)
		{
			float d0 = v0.pos.z + v0.pos.w;
			float d1 = v1.pos.z + v1.pos.w;
			float factor = 1.0f / (d1 - d0);

			Vertex nVertex = Vertex((v0.pos * d1 - v1.pos * d0) * factor,
				(v0.nrml * d1 - v1.nrml * d0) * factor,
				(v0.uv * d1 - v1.uv * d0) * factor);
			if (v0Inside)
			{
				n_v1 = nVertex;
			}
			else {
				n_v0 = nVertex;
			}
		}

		if ((size == 0 || vertices[size - 1] != n_v0) && size < 5)
		{
			vertices[size] = n_v0;
			++size;
		}
		vertices[size] = n_v1;
		return ++size;
	}

	inline bool Rasterizer::CullFace(const Face& f) const noexcept
	{
		float d = (f.v[1].pos.x - f.v[0].pos.x) *
			(f.v[2].pos.y - f.v[0].pos.y) -
			(f.v[1].pos.y - f.v[0].pos.y) *
			(f.v[2].pos.x - f.v[0].pos.x);
		return d < 0.0f;
	}

	inline Vector4 Rasterizer::BoundingBox(const Face& f, const float fwidth, const float fheight) const noexcept
	{
		const Vector2 v0 = Vector2(f.v[0].pos.x, f.v[0].pos.y);
		const Vector2 v1 = Vector2(f.v[1].pos.x, f.v[1].pos.y);
		const Vector2 v2 = Vector2(f.v[2].pos.x, f.v[2].pos.y);

		const Vector2 mini = Vector2::Min(Vector2::Min(v0, v1), v2);
		const Vector2 maxi = Vector2::Max(Vector2::Max(v0, v1), v2);

		const Vector2 lim = Vector2(fwidth - 1.0f, fheight - 1.0f);

		Vector2 finalMin = Vector2::Min(mini, maxi);
		Vector2 finalMax = Vector2::Max(mini, maxi);

		Clamp(finalMin, Vector2::Zero(), lim);
		Clamp(finalMax, Vector2::Zero(), lim);

		return Vector4(finalMin.x, finalMin.y, finalMax.x, finalMax.y);
	}

}