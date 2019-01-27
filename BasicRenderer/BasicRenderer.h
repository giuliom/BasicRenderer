#pragma once

#include <memory>
#include <vector>
#include "FrameBuffer.h"
#include "SceneObject.h"
#include "Camera.h"
#include "World.h"


class BasicRenderer
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

	Color fillColor = Color(0.64f, 0.92f, 0.92f);
	float gamma = 2.2f;
	float gammaEncoding = 1.0f / gamma;

public:
	BasicRenderer() {}
	~BasicRenderer() {}

	const std::shared_ptr<const FrameBuffer> Render(int width, int height, World& scene, RenderingMode mode = RenderingMode::RASTERIZER, ShadingMode shading = ShadingMode::LIT);
	const std::shared_ptr<const FrameBuffer> RayTrace(int width, int height, World& scene, Color (BasicRenderer::*shading)(const World& w, const Vector3& pos, const Vector3& nrml));

	Camera camera;

protected:
	void DrawObject(const SceneObject& objl, const World& scene, Color(BasicRenderer::*shading)(const World& w, const Vector3& pos, const Vector3& nrml));

	Color NormalShading(const World& scene, const Vector3& pos, const Vector3& normal);
	Color LitShading(const World& scene, const Vector3& pos, const Vector3& normal);

	inline Face PerspectiveDivide(Face& f) const;
	inline Face NormalizedToScreenSpace(Face& f) const;
	inline int Clip(Face& f, Face (&clippedFaces)[4]) const;
	inline int ClipEdge(Vertex& v0, Vertex& v1, Vertex (&vertices)[6], int index) const;
	inline bool CullFace(const Face& f) const;
	inline Vector4 BoundingBox(const Face& f) const;

	inline Vector3 Barycentre(const float x, const float y, const Face& f) const
	{
		Vector4 ab = f.v1.pos - f.v0.pos;
		Vector4 ac = f.v2.pos - f.v0.pos;
		Vector2 pa = Vector2(f.v0.pos.x - x, f.v0.pos.y - y);
		Vector3 uv1 = Vector3::CrossProduct(Vector3(ac.x, ab.x, pa.x), Vector3(ac.y, ab.y, pa.y));

		if (abs(uv1.z) < 1e-2f)
		{
			return Vector3(-1.0f, 1.0f, 1.0f);
		}
		return Vector3((uv1.z - (uv1.x + uv1.y)) * (1.0f / uv1.z), uv1.y * (1.0f / uv1.z), uv1.x * (1.0f / uv1.z));
	}

	inline Vector2 Clamp(Vector2  v, const Vector2 min, const Vector2 max) const
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