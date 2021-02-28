#include "Global.h"
#include "Camera.h"
#include "PrimitiveTypes.h"
#include "Ray.h"
#include <math.h>

namespace BasicRenderer
{

	Camera::Camera()
	{
		SetAspectRatio(1u, 1u);
	}

	Camera::~Camera()
	{
	}

	void Camera::SetAspectRatio(float w, float h)
	{
		m_aspectRatio = w / h;
		SetFov(m_degHorizontalFov);
	}

	// Horizontal FOV in degrees
	void Camera::SetFov(float f)
	{
		m_degHorizontalFov = f;
		m_radVerticalFov = 2.f * atanf(tanf(m_degHorizontalFov * TO_RADIANS * 0.5f) / m_aspectRatio);

		m_viewportHeight = tanf(m_radVerticalFov * 0.5f);
		m_viewportWidth = m_viewportHeight * m_aspectRatio;
		m_fovFactor = 0.5f;
		m_viewportUpperLeft = Vector3(-m_viewportWidth * 0.5f, m_viewportHeight * 0.5f, -m_fovFactor);

		UpdateProjection();
	}

	void Camera::UpdateProjection()
	{
		const float height_scale = tanf(m_radVerticalFov * 0.5f) * m_fovFactor;
		const float right = m_aspectRatio * height_scale;
		const float top = height_scale;

		m_projection.x1 = m_nearClip / right;
		m_projection.y2 = m_nearClip / top;

		m_projection.z3 = - (m_farClip + m_nearClip) / (m_farClip - m_nearClip);
		m_projection.z4 = -1.f;

		m_projection.w3 = -2.f * m_farClip * m_nearClip / (m_farClip - m_nearClip);
	}

	Ray Camera::GetCameraRay(const float u, const float v) const
	{
		// u,v come from Top-left coordinates
		Vector3 direction = m_viewportUpperLeft + Vector3(u * m_viewportWidth, -v * m_viewportHeight, 0);
		return Ray(m_transform.GetPosition(), (m_transform.GetMatrix() * (direction) - m_transform.GetPosition()).Normalize());
	}
}
