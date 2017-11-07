#pragma once

#include "Transform.h"

class Camera
{
protected:
	void UpdateTransform(); //T * R

	Matrix4 view;
	Matrix4 projection; // MVP = P * V * M

	float fov = 70.0f;
	float aspectRatio = 4.0f / 3.0f;
	float nearClip = 0.1f;
	float farClip = 100.0f;

public:
	Camera();
	~Camera();

	Transform transform;

	

	void SetAspectRatio(int w, int h);
	Matrix4 LookAt(Vector3 target, Vector3 up = Vector3(0.0f, 1.0f, 0.0f));

	float GetAspectRatio() { return aspectRatio; }
	Matrix4 GetViewMatrix();
	Matrix4 GetProjectionMatrix();
};

