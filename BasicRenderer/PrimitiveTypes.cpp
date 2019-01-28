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
		float q = b > 0.f ?  -0.5f * (b + sqrtf(discriminant)) : -0.5f * (b - sqrtf(discriminant));
		float t0 = q / a;
		float t1 = c / q;
		
		float t = t1;
		if (t0 > t1) t = t1;

		result.t = t;
		result.pos = r.GetPoint(result.t);
		result.normal = ((result.pos - pos) / radius);
		result.material = GetMaterial();
		return true;

	}
	return false;
}
