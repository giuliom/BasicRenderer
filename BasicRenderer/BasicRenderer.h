#pragma once

#include <memory>
#include <vector>
#include "FrameBuffer.h"
#include "SceneObject.h"
#include "Camera.h"
#include "DirectionalLight.h"


class BasicRenderer
{
protected:
	int width = 640;
	int height = 480;
	float fwidth = 640.0f;
	float fheight = 640.0f;
	std::shared_ptr<FrameBuffer> fBuffer;
	

public:
	BasicRenderer() {}
	~BasicRenderer() {}

	const std::shared_ptr<const FrameBuffer> Render(int width, int height, SceneObject& scene);

	Camera camera;
	DirectionalLight sun;

protected:
	void DrawObject(const SceneObject& obj);

	inline Face PerspectiveDivide(Face& f) const;
	inline Face NormalizedToScreenSpace(Face& f) const;
	inline std::vector<Face> Clip(Face& f) const;
	inline void ClipEdge(Vertex& v0, Vertex& v1, std::vector<Vertex>& vertices) const;
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