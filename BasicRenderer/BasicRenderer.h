#pragma once

#include <memory>
#include "FrameBuffer.h"
#include "SceneObject.h"
#include "Camera.h"


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
	~BasicRenderer();

	const std::shared_ptr<const FrameBuffer> Render(int width, int height, SceneObject& scene);

	Camera camera;

protected:
	void DrawObject(const SceneObject& obj);

	inline Face PerspectiveDivide(Face& f) const;
	inline Face NormalizedToScreenSpace(Face& f) const;
	inline bool CullFace(const Face& f) const;
	inline Vector4 BoundingBox(const Face& f) const;
	inline Vector3 Barycentre(const float x, const float y, const Face& f) const;
	inline Vector2 Clamp(Vector2  v, const Vector2 min, const Vector2 max) const;
};