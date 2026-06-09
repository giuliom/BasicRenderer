#pragma once

#include "Global.h"
#include <vector>
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

		// Screen-space triangle with precomputed edge functions for incremental rasterization
		struct ScreenTriangle
		{
			// Edge function values at (minX, minY) and their per-pixel increments
			float w0, w1, w2;
			float dx0, dx1, dx2;
			float dy0, dy1, dy2;
			// Per-edge depth weights (z / area), so z = w0 * bz0 + w1 * bz1 + w2 * bz2
			float bz0, bz1, bz2;
			int minX, minY, maxX, maxY; // x in [minX, maxX), y in [minY, maxY)
			Color color;
		};

		std::vector<ScreenTriangle> m_triangles;

		void SetupInstance(const uint width, const uint height, const RenderState& state, const MeshInstance& instance, const ShadingFunc& Shading);
		void RasterizeRows(FrameBuffer& fBuffer, const uint width, const uint height, const uint rowStart, const uint rowEnd) const;

		inline void PerspectiveDivide(Face& f) const noexcept;
		inline void NormalizedToScreenSpace(Face& f, const float fwidth, const float fheight) const noexcept;
		inline uint Clip(const Face& f, Face(&clippedFaces)[4]) const noexcept;
		inline uint ClipEdge(const Vertex& v0, const Vertex& v1, Vertex(&vertices)[6], int index) const noexcept;
		inline bool CullFace(const Face& f) const noexcept;
		inline Vector4 BoundingBox(const Face& f, const float fwidth, const float fheight) const noexcept;

		inline void Clamp(Vector2& v, const Vector2& min, const Vector2& max) const noexcept
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
		}
	};
}