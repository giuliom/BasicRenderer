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

	void Camera::SetAspectRatio(uint w, uint h)
	{
		m_aspectRatio = static_cast<float>(w) / static_cast<float>(h);
		SetFov(m_hFov);
	}

	// Horizontal FOV in degrees
	void Camera::SetFov(float f)
	{
		m_hFov = f;
		m_viewportHeight = tanf(m_hFov * 0.5f * TO_RADIANS);
		m_viewportWidth = m_aspectRatio * m_viewportHeight;
		m_fovFactor = 1.f / m_viewportHeight;
		//TODO make raytracer and rasterizer viewports have the same characteristics
		m_viewportUpperLeft = { -m_viewportWidth * 0.5f, m_viewportHeight * 0.5f, -m_fovFactor };

		UpdateProjection();
	}

	void Camera::UpdateProjection()
	{
		const float scale = tanf(m_hFov * 0.5f * TO_RADIANS) * m_nearClip;
		const float r = m_aspectRatio * scale;
		const float t = scale;

		m_projection.x1 = m_nearClip / r;
		m_projection.y2 = m_nearClip / t;

		m_projection.z3 = - (m_farClip + m_nearClip) / (m_farClip - m_nearClip);
		m_projection.z4 = -1.f;

		m_projection.w3 = -2.f * m_farClip * m_nearClip / (m_farClip - m_nearClip);
	}

	const Matrix4& Camera::GetViewMatrix() const
	{
		return m_transform.GetInverseMatrix();
	}

	const Matrix4& Camera::GetProjectionMatrix() const
	{
		return m_projection;
	}

	// TODO fix aspect ratio and position
	Ray Camera::GetCameraRay(const float u, const float v) const
	{
		// u,v comes from Top-left coordinates
		Vector3 direction = m_viewportUpperLeft + Vector3(u * m_viewportWidth, -v * m_viewportHeight, 0);
		return Ray(m_transform.GetPosition(), m_transform.GetMatrix() * (direction) - m_transform.GetPosition());
	}
}
