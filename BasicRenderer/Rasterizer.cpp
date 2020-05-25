#include "Rasterizer.h"
#include "BasicRenderer.h"
#include "World.h"
#include "Material.h"
#include "SceneObject.h"

namespace BasicRenderer
{
	void Rasterizer::Render(FrameBuffer& fBuffer, const World& scene, const ShadingFunc& Shading)
	{
		const uint width = fBuffer.GetWidth();
		const uint height = fBuffer.GetHeight();

		for (const std::shared_ptr<Primitive> obj : scene.GetHierarchy())
		{
			DrawObject(width, height, fBuffer, scene, obj.get(), Shading);
		}
	}

	void Rasterizer::DrawObject(const uint width, const uint height, FrameBuffer& fBuffer, const World& scene, const Primitive* primitive, const ShadingFunc& Shading)
	{
		const Camera& camera = scene.GetMainCamera();
		const SceneObject* obj = dynamic_cast<const SceneObject*>(primitive);

		const float fwidth = static_cast<float>(width);
		const float fheight = static_cast<float>(height);

		if (obj != nullptr)
		{
			Material* mat = obj->GetMaterial();
			Color c = Material::MissingMaterialColor;
			const Matrix4 mvp = camera.GetProjectionMatrix() * camera.GetViewMatrix();

			for (uint i = 0; i < obj->NumFaces(); i++)
			{
				Face f = obj->GetTransformedFace(i);

				if (mat)
				{
					c = Shading(*mat, scene, Vector3::Zero(), f.normal);
					c.x = c.x < 1.f ? c.x : 1.f;
					c.y = c.y < 1.f ? c.y : 1.f;
					c.z = c.z < 1.f ? c.z : 1.f;
				}

				ToMatrixSpace(f, mvp);

				PerspectiveDivide(f);
				NormalizedToScreenSpace(f, fwidth, fheight);

				Face clippedFaces[4];
				uint nClippedFaces = Clip(f, clippedFaces);

				for (uint j = 0u; j < nClippedFaces; j++)
				{
					f = clippedFaces[j];

					if (CullFace(f)) continue;

					Vector4 bbox = BoundingBox(f, fwidth, fheight);
					uint bbz = static_cast<uint>(bbox.z);
					uint bbw = static_cast<uint>(bbox.w);
					uint bbx = static_cast<uint>(bbox.x);
					uint bby = static_cast<uint>(bbox.y);

					for (uint x = bbx; x < bbz; ++x)
					{
						for (uint y = bby; y < bbw; ++y)
						{
							Vector3 bary = Barycentre(static_cast<float>(x), static_cast<float>(y), f);

							if (bary.x < 0.0f || bary.y < 0.0f || bary.z < 0.0f) continue;

							float z = f.v0.pos.z * bary.x + f.v1.pos.z * bary.y + f.v2.pos.z * bary.z;

							//Because of the Pinhole model
							uint index = width * (height - y - 1) + x;

							if (z < fBuffer.GetDepth(index))
							{
								fBuffer.WriteToColor(index, c);
								fBuffer.WriteToDepth(index, z);
							}

						}
					}
				}
			}
		}
	}

	inline void Rasterizer::PerspectiveDivide(Face& f) const
	{
		float v0w = 1.0f / f.v0.pos.w;
		float v1w = 1.0f / f.v1.pos.w;
		float v2w = 1.0f / f.v2.pos.w;
		Vector4 v0 = Vector4(f.v0.pos.x * v0w,
			f.v0.pos.y * v0w,
			f.v0.pos.z * v0w,
			f.v0.pos.w);
		Vector4 v1 = Vector4(f.v1.pos.x * v1w,
			f.v1.pos.y * v1w,
			f.v1.pos.z * v1w,
			f.v1.pos.w);
		Vector4 v2 = Vector4(f.v2.pos.x * v2w,
			f.v2.pos.y * v2w,
			f.v2.pos.z * v2w,
			f.v2.pos.w);

		f = Face(v0, v1, v2, f);
	}

	inline void Rasterizer::NormalizedToScreenSpace(Face& f, const float fwidth, const float fheight) const
	{
		Vector4 v0 = Vector4(floorf(0.5f * fwidth * (f.v0.pos.x + 1.0f)),
			floorf(0.5f * fheight * (f.v0.pos.y + 1.0f)),
			f.v0.pos.z,
			f.v0.pos.w);
		Vector4 v1 = Vector4(floorf(0.5f * fwidth * (f.v1.pos.x + 1.0f)),
			floorf(0.5f * fheight * (f.v1.pos.y + 1.0f)),
			f.v1.pos.z,
			f.v1.pos.w);
		Vector4 v2 = Vector4(floorf(0.5f * fwidth * (f.v2.pos.x + 1.0f)),
			floorf(0.5f * fheight * (f.v2.pos.y + 1.0f)),
			f.v2.pos.z,
			f.v2.pos.w);

		f = Face(v0, v1, v2, f);
	}

	inline uint Rasterizer::Clip(const Face& f, Face(&clippedFaces)[4]) const
	{
		int nfaces = 0;

		if (f.v0.pos.w <= 0.0 && f.v1.pos.w <= 0.0 && f.v2.pos.w <= 0.0) {
			return 0;
		}

		if (f.v0.pos.w > 0.0 &&
			f.v1.pos.w > 0.0 &&
			f.v2.pos.w > 0.0 &&
			abs(f.v0.pos.z) < f.v0.pos.w &&
			abs(f.v1.pos.z) < f.v1.pos.w &&
			abs(f.v2.pos.z) < f.v2.pos.w)
		{
			return 0;
		}
		else
		{
			Vertex vertices[6];
			int size = 0;
			size = ClipEdge(f.v0, f.v1, vertices, size);
			size = ClipEdge(f.v1, f.v2, vertices, size);
			size = ClipEdge(f.v2, f.v0, vertices, size);

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

	inline uint Rasterizer::ClipEdge(const Vertex& v0, const Vertex& v1, Vertex(&vertices)[6], int index) const
	{
		assert(index < 5);

		int size = index;
		Vertex n_v0 = v0;
		Vertex n_v1 = v1;

		bool v0Inside = v0.pos.w > 0.0 && v0.pos.z > -v0.pos.w;
		bool v1Inside = v1.pos.w > 0.0 && v1.pos.z > -v1.pos.w;

		if (!v0Inside && !v1Inside)
		{
			return 0;
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

		if ((size == 0 || !(vertices[size - 1] != n_v0)) && size < 6)
		{
			vertices[size] = n_v0;
			++size;
		}
		vertices[size] = n_v1;
		return ++size;
	}

	inline bool Rasterizer::CullFace(const Face& f) const
	{
		float d = (f.v1.pos.x - f.v0.pos.x) *
			(f.v2.pos.y - f.v0.pos.y) -
			(f.v1.pos.y - f.v0.pos.y) *
			(f.v2.pos.x - f.v0.pos.x);
		return d < 0.0f;
	}

	inline Vector4 Rasterizer::BoundingBox(const Face& f, const float fwidth, const float fheight) const
	{
		Vector2 v0 = Vector2(f.v0.pos.x, f.v0.pos.y);
		Vector2 v1 = Vector2(f.v1.pos.x, f.v1.pos.y);
		Vector2 v2 = Vector2(f.v2.pos.x, f.v2.pos.y);

		Vector2 mini = Vector2::Min(Vector2::Min(v0, v1), v2);
		Vector2 maxi = Vector2::Max(Vector2::Max(v0, v1), v2);

		Vector2 lim = Vector2(fwidth - 1.0f, fheight - 1.0f);

		Vector2 finalMin = Clamp(Vector2::Min(mini, maxi), Vector2(0.0f, 0.0f), lim);
		Vector2 finalMax = Clamp(Vector2::Max(mini, maxi), Vector2(0.0f, 0.0f), lim);

		return Vector4(finalMin.x, finalMin.y, finalMax.x, finalMax.y);
	}

}