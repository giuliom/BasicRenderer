#pragma once

#include <array>
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
		// Minimal working triangle used between projection and screen-space setup.
		// Shading has already consumed the world-space normal, so normals and UVs
		// do not need to pass through clipping or the remaining transforms.
		struct alignas(16) ClipTriangle
		{
			std::array<Vector4, 3> positions;
		};

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
		std::vector<std::vector<uint32_t>> m_triangleBins;

		void SetupInstance(const uint width, const uint height, const Matrix4& viewProjection, const RenderState& state, const DrawableInstance& instance, const ShadingFunc& Shading);
		void RasterizeRows(FrameBuffer& fBuffer, const uint width, const uint height, const uint rowStart, const uint rowEnd, const std::vector<uint32_t>& triangleIndices) const;

		inline void ToScreenSpace(ClipTriangle& triangle, const float fwidth, const float fheight) const noexcept;
		inline uint Clip(const ClipTriangle& triangle, ClipTriangle(&clippedTriangles)[4]) const noexcept;
		inline uint ClipEdge(const Vector4& v0, const Vector4& v1, Vector4(&vertices)[6], int index) const noexcept;
		inline Vector4 BoundingBox(const ClipTriangle& triangle, const float fwidth, const float fheight) const noexcept;

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
