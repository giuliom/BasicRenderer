#include "PrimitiveTypes.h"

//TODO check bug
bool Sphere::GetHit(const Ray& r, float tMin, float tMax, HitResult& result) const
{
	Vector3 oc = r.origin - pos;
	float a = Vector3::Dot(r.direction, r.direction);
	float b = Vector3::Dot(oc, r.direction);
	float c = Vector3::Dot(oc, oc) - radius * radius;
	float discriminant = b * b - a * c;
	float sqDiscr = sqrtf(discriminant);
	if (discriminant > 0.f)
	{
		float temp = (-b - sqDiscr) / a;
		if (temp > tMin && temp < tMax)
		{
			result.t = temp;
			result.pos = r.GetPoint(result.t);
			result.normal = ((result.pos - pos) / radius);
			result.material = GetMaterial();
			return true;
		}
		temp = (-b + sqDiscr) / a;
		if (temp > tMin && temp < tMax)
		{
			result.t = temp;
			result.pos = r.GetPoint(result.t);
			result.normal = ((result.pos - pos) / radius);
			result.material = GetMaterial();
			return true;
		}
	}
	return false;
}
