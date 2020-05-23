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
		Matrix4 m_view = Matrix4::Identity();
		Matrix4 m_projection = Matrix4::Zero(); // MVP = P * V * M

		float m_hFov = 90.0f;
		float m_fovFactor;
		float m_aspectRatio = 4.0f / 3.0f;
		float m_nearClip = 0.1f;
		float m_farClip = 10000.0f;

		float m_viewportWidth = 0.f;
		float m_viewportHeight = 0;
		Vector3 m_viewportUpperLeft = { -0.5f, 0.5f, -1.f };

	public:
		Camera();
		~Camera();

		void SetAspectRatio(uint w, uint h);
		void SetFov(float f);

		inline Transform& GetTransform() { return m_transform; }
		inline float GetFov() const { return m_hFov; }
		inline float GetAspectRatio() const { return m_aspectRatio; }
		inline float GetViewportWidth() const { return m_viewportWidth; }
		inline float GetViewportHeight() const { return m_viewportHeight; }
		const Matrix4& GetViewMatrix() const;
		const Matrix4& GetProjectionMatrix() const;
		Ray GetCameraRay(const float u, const float v) const;

	protected:
		void UpdateProjection();
	};

}