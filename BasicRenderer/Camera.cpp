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
		SetFov(m_fov);
	}

	void Camera::SetFov(float f)
	{
		m_fov = f;
		m_viewportHeight = tanf(m_fov * TO_RADIANS * 0.5f) * 2.f;
		m_viewportWidth = m_aspectRatio * m_viewportHeight;
		m_fovFactor = 1.f / m_viewportHeight;

		m_viewportUpperLeft = { -m_viewportWidth * 0.5f, m_viewportHeight * 0.5f, -m_fovFactor };

		UpdateProjection();
	}

	void Camera::UpdateProjection()
	{
		const float radfov = (m_fov * TO_RADIANS * 0.5f);;
		const float scale = 1.0f / tanf(radfov) * m_nearClip;
		const float w = m_aspectRatio * scale;
		const float h = scale;

		m_projection.x1 = m_nearClip / w;
		m_projection.y2 = m_nearClip / h;

		m_projection.z3 = -(m_farClip + m_nearClip) / (m_farClip - m_nearClip);
		m_projection.w3 = (-m_farClip * m_nearClip) / (m_farClip - m_nearClip);
		m_projection.z3 = -1.0f;

		m_projection.z4 = -2.f * m_farClip * m_nearClip / (m_farClip - m_nearClip);
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
		return Ray(m_transform.GetPosition(), m_transform.GetMatrix() * direction);
	}
}
