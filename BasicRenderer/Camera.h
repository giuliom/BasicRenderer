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
		Transform m_transform;
		Matrix4 m_view;
		Matrix4 m_projection; // MVP = P * V * M

		float m_fov = 75.0f;
		float m_fovFactor;
		float m_aspectRatio = 4.0f / 3.0f;
		float m_nearClip = 0.3f;
		float m_farClip = 1000.0f;

		float m_halfWidth, m_halfHeight;

	public:
		Camera();
		~Camera();

		void SetAspectRatio(int w, int h);
		void SetFov(float f);
		Matrix4 LookAt(Vector3 target, Vector3 up = Vector3(0.0f, 1.0f, 0.0f));

		inline Transform& GetTransform() { return m_transform; }
		inline float GetFov() const { return m_fov; }
		inline float GetAspectRatio() const { return m_aspectRatio; }
		inline float GetHalfWidth() const { return m_halfWidth; }
		inline float GetHalfHeight() const { return m_halfHeight; }
		Matrix4 GetViewMatrix() const;
		Matrix4 GetProjectionMatrix() const;
		Ray GetCameraRay(const float u, const float v) const;
	};

}