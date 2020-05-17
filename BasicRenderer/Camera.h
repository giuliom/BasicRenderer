#pragma once

#include "Transform.h"

namespace BasicRenderer
{
	class Ray;

	/* Pinhole camera aligned with -z therefore left handed */
	class Camera
	{
	protected:
		//void UpdateTransform(); //T * R

		Matrix4 view;
		Matrix4 projection; // MVP = P * V * M

		float fov = 85.0f;
		float fovFactor;
		float aspectRatio = 4.0f / 3.0f;
		float nearClip = 0.3f;
		float farClip = 1000.0f;

		float halfWidth, halfHeight;

	public:
		Camera();
		~Camera();

		Transform transform;

		void SetAspectRatio(int w, int h);
		void SetFov(float f);
		Matrix4 LookAt(Vector3 target, Vector3 up = Vector3(0.0f, 1.0f, 0.0f));

		inline float GetFov() const { return fov; }
		inline float GetAspectRatio() const { return aspectRatio; }
		inline float GetHalfWidth() const { return halfWidth; }
		inline float GetHalfHeight() const { return halfHeight; }
		Matrix4 GetViewMatrix() const;
		Matrix4 GetProjectionMatrix();
		Ray GetCameraRay(const float u, const float v) const;
	};

}