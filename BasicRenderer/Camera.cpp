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
		aspectRatio = (float)w / (float)h;
		SetFov(fov);
	}

	void Camera::SetFov(float f)
	{
		fov = f;
		const float theta = (fov * PI) / 180.f;
		halfHeight = tanf(theta / 2.0f);
		fovFactor = 1.0f / halfHeight;
		halfWidth = aspectRatio * halfHeight;
	}

	// Using right handed coordinate system
	Matrix4 Camera::LookAt(Vector3 target, Vector3 up)
	{
		Vector3 pos = transform.GetPosition();
		Vector3 za = (pos - target).Normalize(); // Forward
		Vector3 xa = Vector3::CrossProduct(up.Normalize(), za).Normalize(); // Right
		Vector3 ya = Vector3::CrossProduct(za, xa).Normalize(); // Up

		Matrix4 nview = Matrix4(xa.x, ya.x, za.x, za.x,
			xa.y, ya.y, za.y, za.y,
			xa.z, ya.z, za.z, za.z,
			-Vector3::Dot(xa, pos), -Vector3::Dot(ya, pos), -Vector3::Dot(za, pos), -Vector3::Dot(za, pos)
		);

		view = nview;
		return nview;
	}

	Matrix4 Camera::GetViewMatrix() const
	{
		return transform.GetInverseMatrix();
	}

	Matrix4 Camera::GetProjectionMatrix() //TODO fix projection
	{
		float radfov = (fov * PI) / 180.0f;
		float f = 1.0f / tanf(radfov * 0.5f);
		Matrix4 m = Matrix4::Zero();

		m.x1 = f / aspectRatio;
		m.y2 = f;
		m.z3 = -(farClip / (farClip - nearClip));
		m.w3 = -1.0f * ((farClip * nearClip) / (farClip - nearClip));
		m.z4 = -1.0f;
		m.w4 = 0.0f;

		projection = m;
		return projection;
	}

	Ray Camera::GetCameraRay(const float u, const float v) const
	{
		// u,v comes from Top-left coordinates
		Vector3 direction = Vector3(-halfWidth + u * 2.f * halfWidth, halfHeight - v * 2.f * halfHeight, -fovFactor);
		return Ray(transform.GetPosition(), transform.GetMatrix() * direction); //TODO fix
	}
}
