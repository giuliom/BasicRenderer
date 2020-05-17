#pragma once

#include <memory>
#include <vector>
#include "FrameBuffer.h"
#include "SceneObject.h"
#include "Camera.h"
#include "World.h"

namespace BasicRenderer
{
	class Ray;
	class Hitable;

	class Renderer
	{
	public:
		enum class RenderingMode
		{
			RASTERIZER,
			RAYTRACER
		};

		enum class ShadingMode
		{
			LIT,
			NORMAL
		};

	protected:
		int width = 640;
		int height = 480;
		float fwidth = 640.0f;
		float fheight = 640.0f;
		std::shared_ptr<FrameBuffer> fBuffer;

		float gamma = 2.2f;
		float gammaEncoding = 1.0f / gamma;

		Color missingMaterialColor = { 1.0f, 0.0f, 1.0f };

	public:
		Renderer() {}
		~Renderer() {}

		//TODO move, and rename
		const std::shared_ptr<const FrameBuffer> Render(int width, int height, World& scene, RenderingMode mode = RenderingMode::RASTERIZER, ShadingMode shading = ShadingMode::LIT, int samplesPerPixel = 4, int bounces = 3);
		const std::shared_ptr<const FrameBuffer> RayTracing(int width, int height, World& scene, int pixelSamples, int bounces, Color(Material::* shading)(const World& w, const Vector3& pos, const Vector3& nrml));

		Camera camera;

	protected:

		Color RayTrace(const Ray& ray, World& scene, int bounces, Color(Material::* shading)(const World& w, const Vector3& pos, const Vector3& nrml));

		void DrawObject(const Primitive* primitive, const World& scene, Color(Material::* shading)(const World& w, const Vector3& pos, const Vector3& nrml));

		inline void PerspectiveDivide(Face& f) const;
		inline void NormalizedToScreenSpace(Face& f) const;
		inline int Clip(const Face& f, Face(&clippedFaces)[4]) const;
		inline int ClipEdge(const Vertex& v0, const Vertex& v1, Vertex(&vertices)[6], int index) const;
		inline bool CullFace(const Face& f) const;
		inline Vector4 BoundingBox(const Face& f) const;

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