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
		m_triangles.reserve(state.m_faceBuffer.size());
		const Matrix4 viewProjection = state.m_camera.GetProjectionMatrix() * state.m_camera.GetViewMatrix();

		for (const DrawableInstance& instance : state.m_instances)
		{
			// Only triangle meshes are supported for now
			if (instance.NumFaces() > 0u)
			{
				SetupInstance(width, height, viewProjection, state, instance, Shading);
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

		m_triangleBins.resize(chunkCount);
		for (auto& bin : m_triangleBins)
		{
			bin.clear();
		}

		for (uint32_t triangleIndex = 0u; triangleIndex < m_triangles.size(); ++triangleIndex)
		{
			const ScreenTriangle& triangle = m_triangles[triangleIndex];
			const uint firstChunk = static_cast<uint>(triangle.minY) / rowsPerChunk;
			const uint lastChunk = static_cast<uint>(triangle.maxY - 1) / rowsPerChunk;

			for (uint chunk = firstChunk; chunk <= lastChunk; ++chunk)
			{
				m_triangleBins[chunk].push_back(triangleIndex);
			}
		}

		std::atomic<uint> nextChunk = 0u;

		const auto job = [&]()
		{
			for (uint chunk = nextChunk.fetch_add(1u, std::memory_order_relaxed); chunk < chunkCount; chunk = nextChunk.fetch_add(1u, std::memory_order_relaxed))
			{
				const uint rowStart = chunk * rowsPerChunk;
				const uint rowEnd = std::min(rowStart + rowsPerChunk, height);
				RasterizeRows(fBuffer, width, height, rowStart, rowEnd, m_triangleBins[chunk]);
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

	void Rasterizer::SetupInstance(const uint width, const uint height, const Matrix4& viewProjection, const RenderState& state, const DrawableInstance& instance, const ShadingFunc& Shading)
	{
		const float fwidth = static_cast<float>(width);
		const float fheight = static_cast<float>(height);

		const Material* mat = instance.GetMaterial();
		Color c = Material::MissingMaterialColor;

		const Face* const faces = state.m_faceBuffer.data() + instance.GetFirstFace();
		const uint32_t faceCount = instance.NumFaces();
		for (uint32_t i = 0u; i < faceCount; i++)
		{
			const Face& face = faces[i];

			if (mat)
			{
				c = Shading(*mat, state, Vector3::Zero(), face.GetNormal());
				c.r = c.r < 1.f ? c.r : 1.f;
				c.g = c.g < 1.f ? c.g : 1.f;
				c.b = c.b < 1.f ? c.b : 1.f;
			}

			const ClipTriangle projectedTriangle{ {
				viewProjection * face.v[0].pos,
				viewProjection * face.v[1].pos,
				viewProjection * face.v[2].pos
			} };

			ClipTriangle clippedTriangles[4];
			const uint clippedTriangleCount = Clip(projectedTriangle, clippedTriangles);

			for (uint j = 0u; j < clippedTriangleCount; ++j)
			{
				ClipTriangle& triangle = clippedTriangles[j];
				ToScreenSpace(triangle, fwidth, fheight);

				const float x0 = triangle.positions[0].x, y0 = triangle.positions[0].y, z0 = triangle.positions[0].z;
				const float x1 = triangle.positions[1].x, y1 = triangle.positions[1].y, z1 = triangle.positions[1].z;
				const float x2 = triangle.positions[2].x, y2 = triangle.positions[2].y, z2 = triangle.positions[2].z;

				// Signed double area: negative faces are back-facing; tiny faces are degenerate.
				const float area = (x1 - x0) * (y2 - y0) - (y1 - y0) * (x2 - x0);
				if (area < 1e-2f) continue;

				const Vector4 bbox = BoundingBox(triangle, fwidth, fheight);
				const int minX = static_cast<int>(bbox.x);
				const int minY = static_cast<int>(bbox.y);
				const int maxX = static_cast<int>(bbox.z);
				const int maxY = static_cast<int>(bbox.w);
				if (minX >= maxX || minY >= maxY) continue;

				ScreenTriangle& tri = m_triangles.emplace_back();
				tri.minX = minX;
				tri.minY = minY;
				tri.maxX = maxX;
				tri.maxY = maxY;

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
			}
		}
	}

	void Rasterizer::RasterizeRows(FrameBuffer& fBuffer, const uint width, const uint height, const uint rowStart, const uint rowEnd, const std::vector<uint32_t>& triangleIndices) const
	{
		for (const uint32_t triangleIndex : triangleIndices)
		{
			const ScreenTriangle& tri = m_triangles[triangleIndex];
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

	inline void Rasterizer::ToScreenSpace(ClipTriangle& triangle, const float fwidth, const float fheight) const noexcept
	{
		for (Vector4& position : triangle.positions)
		{
			const float invW = 1.0f / position.w;
			position.x = floorf(0.5f * fwidth * (position.x * invW + 1.0f));
			position.y = floorf(0.5f * fheight * (position.y * invW + 1.0f));
			position.z *= invW;
		}
	}

	inline uint Rasterizer::Clip(const ClipTriangle& triangle, ClipTriangle(&clippedTriangles)[4]) const noexcept
	{
		int nfaces = 0;

		if (triangle.positions[0].w <= 0.0 && triangle.positions[1].w <= 0.0 && triangle.positions[2].w <= 0.0) {
			return 0;
		}

		if (triangle.positions[0].w > 0.0 &&
			triangle.positions[1].w > 0.0 &&
			triangle.positions[2].w > 0.0 &&
			abs(triangle.positions[0].z) < triangle.positions[0].w &&
			abs(triangle.positions[1].z) < triangle.positions[1].w &&
			abs(triangle.positions[2].z) < triangle.positions[2].w)
		{
			clippedTriangles[0] = triangle;
			return 1;
		}
		else
		{
			Vector4 vertices[6];
			int size = 0;
			size = ClipEdge(triangle.positions[0], triangle.positions[1], vertices, size);
			size = ClipEdge(triangle.positions[1], triangle.positions[2], vertices, size);
			size = ClipEdge(triangle.positions[2], triangle.positions[0], vertices, size);

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
				clippedTriangles[i - 1] = ClipTriangle{ { vertices[0], vertices[i], vertices[i + 1] } };
				++nfaces;
			}
		}
		return nfaces;
	}

	inline uint Rasterizer::ClipEdge(const Vector4& v0, const Vector4& v1, Vector4(&vertices)[6], int index) const noexcept
	{
		assert(index < 5);

		int size = index;
		Vector4 n_v0 = v0;
		Vector4 n_v1 = v1;

		bool v0Inside = v0.w > 0.0 && v0.z > -v0.w;
		bool v1Inside = v1.w > 0.0 && v1.z > -v1.w;

		if (!v0Inside && !v1Inside)
		{
			return size;
		}
		else if (v0Inside != v1Inside)
		{
			float d0 = v0.z + v0.w;
			float d1 = v1.z + v1.w;
			float factor = 1.0f / (d1 - d0);

			const Vector4 nVertex = (v0 * d1 - v1 * d0) * factor;
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

	inline Vector4 Rasterizer::BoundingBox(const ClipTriangle& triangle, const float fwidth, const float fheight) const noexcept
	{
		const Vector2 v0 = Vector2(triangle.positions[0].x, triangle.positions[0].y);
		const Vector2 v1 = Vector2(triangle.positions[1].x, triangle.positions[1].y);
		const Vector2 v2 = Vector2(triangle.positions[2].x, triangle.positions[2].y);

		const Vector2 mini = Vector2::Min(Vector2::Min(v0, v1), v2);
		const Vector2 maxi = Vector2::Max(Vector2::Max(v0, v1), v2);

		const Vector2 lim = Vector2(fwidth - 1.0f, fheight - 1.0f);

		Vector2 finalMin = mini;
		Vector2 finalMax = maxi;

		Clamp(finalMin, Vector2::Zero(), lim);
		Clamp(finalMax, Vector2::Zero(), lim);

		return Vector4(finalMin.x, finalMin.y, finalMax.x, finalMax.y);
	}

}
