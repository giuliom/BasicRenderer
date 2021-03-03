#pragma once

#include "Global.h"
#include "RenderSystem.h"
#include "Face.h"
#include "Material.h"

namespace BasicRenderer
{
	class FrameBuffer;
	class World;
	class Camera;

	class Rasterizer : public RenderSystem
	{

	public:
		Rasterizer() {}

		void Render(FrameBuffer& fBuffer, const RenderState& state, const ShadingFunc& Shading) override;

	protected:

		void DrawObject(const uint width, const uint height, FrameBuffer& fBuffer, const RenderState& state, const Primitive& primitive, const ShadingFunc& Shading);

		inline void PerspectiveDivide(Face& f) const;
		inline void NormalizedToScreenSpace(Face& f, const float fwidth, const float fheight) const;
		inline uint Clip(const Face& f, Face(&clippedFaces)[4]) const;
		inline uint ClipEdge(const Vertex& v0, const Vertex& v1, Vertex(&vertices)[6], int index) const;
		inline bool CullFace(const Face& f) const;
		inline Vector4 BoundingBox(const Face& f, const float fwidth, const float fheight) const;

		inline Vector3 Barycentre(const float x, const float y, const Face& f) const
		{
			const Vector4 ab(f.v1.pos - f.v0.pos);
			const Vector4 ac(f.v2.pos - f.v0.pos);
			const Vector2 pa(f.v0.pos.x - x, f.v0.pos.y - y);
			const Vector3 uv1(Vector3::CrossProduct(Vector3(ac.x, ab.x, pa.x), Vector3(ac.y, ab.y, pa.y)));

			if (abs(uv1.z) < 1e-2f)
			{
				return { -1.0f, 1.0f, 1.0f };
			}
			return { (uv1.z - (uv1.x + uv1.y)) * (1.0f / uv1.z), uv1.y * (1.0f / uv1.z), uv1.x * (1.0f / uv1.z) };
		}

		inline Vector2 Clamp(Vector2& v, const Vector2& min, const Vector2& max) const
		{
			if (v.x < min.x)
			{
				v.x = min.x;
			}
			else if (v.x > max.x)
			{
				v.x = max.x;
			}
			if (v.y < min.y)
			{
				v.y = min.y;
			}
			else if (v.y > max.y)
			{
				v.y = max.y;
			}
			return v;
		}
	};
}