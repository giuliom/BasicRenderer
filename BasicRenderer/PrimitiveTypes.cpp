#include "PrimitiveTypes.h"

float Sphere::GetHit(const Ray & r)
{
	Vector3 oc = r.origin - pos;
	float a = Vector3::Dot(r.direction, r.direction);
	float b = 2.f * Vector3::Dot(oc, r.direction);
	float c = Vector3::Dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4.f * a * c;
	if (discriminant < 0.f)
	{
		return -1.f;
	}
	else
	{
		return (-b - sqrtf(discriminant)) / (2.f * a);
	}
}
