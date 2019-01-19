#pragma once

#include "Transform.h"

/* Pinhole camera aligned with -z therefore left handed */
class Camera
{
protected:
	//void UpdateTransform(); //T * R

	Matrix4 view;
	Matrix4 projection; // MVP = P * V * M

	float fov = 70.0f;
	float aspectRatio = 4.0f / 3.0f;
	float nearClip = 0.3f;
	float farClip = 1000.0f;

	float u, v;

public:
	Camera();
	~Camera();

	Transform transform;

	void SetAspectRatio(int w, int h);
	Matrix4 LookAt(Vector3 target, Vector3 up = Vector3(0.0f, 1.0f, 0.0f));

	float GetAspectRatio() { return aspectRatio; }
	float GetWidth() { return u; }
	float GetHeight() { return v; }
	Matrix4 GetViewMatrix();
	Matrix4 GetProjectionMatrix();
};

