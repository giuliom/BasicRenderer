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

bool Plane::GetHit(const Ray & r, float tMin, float tMax, HitResult & result) const
{
	float div = Vector3::Dot(normal, r.direction);

	if (div == 0.f)
	{
		return false;
	}
	
	float t = Vector3::Dot(normal, centre - r.origin) / div;
	
	if (t >= 0.f)
	{
		result.t = t;
		result.pos = r.GetPoint(result.t);
		result.normal = normal;
		result.material = GetMaterial();
		return true;
	}

	return false;
}
