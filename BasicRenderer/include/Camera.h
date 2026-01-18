#pragma once

#include "Transform.h"
#include "Ray.h"

namespace BasicRenderer
{
	class Ray;

	/* Pinhole camera facing -z therefore right handed like OpenGL */
	class Camera
	{
	protected:
		//void UpdateTransform(); //T * R
		Transform m_transform;
		Matrix4 m_view = Matrix4::Identity();
		Matrix4 m_projection = Matrix4::Zero(); // MVP = P * V * M

		float m_degHorizontalFov = 90.f;
		float m_radVerticalFov = 59.f * TO_RADIANS;
		float m_fovFactor = 0.5f;
		float m_aspectRatio = 4.f / 3.f;
		float m_nearClip = 0.1f;
		float m_farClip = 1000.0f;

		float m_viewportWidth = 0.f;
		float m_viewportHeight = 0;
		Vector3 m_viewportUpperLeft = { -0.5f, 0.5f, -1.f };

		const float m_movementSpeed = 0.25f;
		const float m_rotationSpeed = 0.005f;

	public:
		Camera();
		virtual ~Camera();

		void SetAspectRatio(float w, float h);
		void SetFov(float f);

		inline Transform& GetTransform() { return m_transform; }
		inline float GetFov() const { return m_degHorizontalFov; }
		inline float GetAspectRatio() const { return m_aspectRatio; }
		inline float GetViewportWidth() const { return m_viewportWidth; }
		inline float GetViewportHeight() const { return m_viewportHeight; }
		inline const Matrix4& GetViewMatrix() const { return m_transform.GetInverseMatrix(); }
		inline const Matrix4& GetProjectionMatrix() const { return m_projection; }
		inline float GetMovementSpeed() const { return m_movementSpeed; }
		inline float GetRotationSpeed() const { return m_rotationSpeed; }

		Ray GetCameraRay(const float u, const float v) const
		{
			// u,v come from Top-left coordinates
			Vector3 direction = m_viewportUpperLeft + Vector3(u * m_viewportWidth, -v * m_viewportHeight, 0);
			return Ray(m_transform.GetPosition(), m_transform.GetMatrix() * (direction) - m_transform.GetPosition());
		}

	protected:
		void UpdateProjection();
	};

}