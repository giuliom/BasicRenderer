#include "Camera.h"
#include "PrimitiveTypes.h"
#include "Ray.h"
#include <math.h>

namespace BasicRenderer
{
	Camera::Camera()
	{

	}


	Camera::~Camera()
	{
	}

	void Camera::SetAspectRatio(int w, int h)
	{
		m_aspectRatio = (float)w / (float)h;
		SetFov(m_fov);
		m_projection = GetProjectionMatrix();
	}

	void Camera::SetFov(float f)
	{
		m_fov = f;
		const float theta = (m_fov * PI) / 180.f;
		m_halfHeight = tanf(theta / 2.0f);
		m_fovFactor = 1.0f / m_halfHeight;
		m_halfWidth = m_aspectRatio * m_halfHeight;
	}

	// Using right handed coordinate system
	Matrix4 Camera::LookAt(Vector3 target, Vector3 up)
	{
		Vector3 pos = m_transform.GetPosition();
		Vector3 za = (pos - target).Normalize(); // Forward
		Vector3 xa = Vector3::CrossProduct(up.Normalize(), za).Normalize(); // Right
		Vector3 ya = Vector3::CrossProduct(za, xa).Normalize(); // Up

		Matrix4 nview = Matrix4(xa.x, ya.x, za.x, za.x,
			xa.y, ya.y, za.y, za.y,
			xa.z, ya.z, za.z, za.z,
			-Vector3::Dot(xa, pos), -Vector3::Dot(ya, pos), -Vector3::Dot(za, pos), -Vector3::Dot(za, pos)
		);

		m_view = nview;
		return nview;
	}

	Matrix4 Camera::GetViewMatrix() const
	{
		return m_transform.GetInverseMatrix();
	}

	Matrix4 Camera::GetProjectionMatrix() const
	{
		float radfov = (m_fov * PI) / 180.0f;
		float f = 1.0f / tanf(radfov * 0.5f);
		Matrix4 m = Matrix4::Zero();

		m.x1 = f / m_aspectRatio;
		m.y2 = f;
		m.z3 = -m_farClip / (m_farClip - m_nearClip);
		m.w3 = (-m_farClip * m_nearClip) / (m_farClip - m_nearClip);
		m.z4 = -1.0f;
		m.w4 = 0.0f;
		return m;
	}

	Ray Camera::GetCameraRay(const float u, const float v) const
	{
		// u,v comes from Top-left coordinates
		Vector3 direction = Vector3(-m_halfWidth + u * 2.f * m_halfWidth, m_halfHeight - v * 2.f * m_halfHeight, -m_fovFactor);
		return Ray(m_transform.GetPosition(), m_transform.GetMatrix() * direction); //TODO fix
	}
}
