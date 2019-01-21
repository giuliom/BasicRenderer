#include "PrimitiveTypes.h"

//TODO check bug
bool Sphere::GetHit(const Ray& r, float tMin, float tMax, HitResult& result) const
{
	Vector3 oc = r.origin - pos;
	float a = Vector3::Dot(r.direction, r.direction);
	float b = 2.f * Vector3::Dot(oc, r.direction);
	float c = Vector3::Dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4.f * a * c;
	if (discriminant > 0.f)
	{
		//TODO optimize
		float t = (-b - sqrtf(b * b - a * c)) / a;
		if (t > tMin && t < tMax)
		{
			result.t = t;
			result.pos = r.GetPoint(result.t);
			result.normal = (result.pos - pos) / radius;
			return true;
		}
		t = (-b + sqrtf(b * b - a * c)) / a;
		if (t > tMin && t < tMax)
		{
			result.t = t;
			result.pos = r.GetPoint(result.t);
			result.normal = (result.pos - pos) / radius;
			return true;
		}
	}
	return false;
}
