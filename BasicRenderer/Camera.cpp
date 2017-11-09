#include "Camera.h"
#include "PrimitiveTypes.h"
#include <math.h>


Camera::Camera()
{

}


Camera::~Camera()
{
}

void Camera::SetAspectRatio(int w, int h)
{
	aspectRatio = ((float)w) / ((float)h);
}

// Using right handed coordinate system
Matrix4 Camera::LookAt(Vector3 target, Vector3 up)
{
	Vector3 pos = transform.GetPosition();
	Vector3 za = (pos - target).Normalize(); // Forward
	Vector3 xa = Vector3::CrossProduct(up.Normalize(), za).Normalize(); // Right
	Vector3 ya = Vector3::CrossProduct(za, xa).Normalize(); // Up
	
	Matrix4 nview = Matrix4(	xa.x,					ya.x,					za.x,					za.x,
								xa.y,					ya.y,					za.y,					za.y,
								xa.z,					ya.z,					za.z,					za.z,
								-Vector3::Dot(xa, pos), -Vector3::Dot(ya, pos), -Vector3::Dot(za, pos), -Vector3::Dot(za, pos)
								);
	
	view = nview;
	return nview;
}

Matrix4 Camera::GetViewMatrix()
{
	return transform.GetInverseMatrix();
}

Matrix4 Camera::GetProjectionMatrix()
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
