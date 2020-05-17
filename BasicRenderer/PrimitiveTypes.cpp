#include "PrimitiveTypes.h"

namespace BasicRenderer
{
	bool Sphere::GetHit(const Ray& r, float tMin, float tMax, float& tHit, Vector3& normalHit) const
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
				tHit = temp;
				normalHit = ((r.GetPoint(temp) - pos) / radius);
				return true;
			}
			temp = (-b + sqDiscr) / a;
			if (temp > tMin && temp < tMax)
			{
				tHit = temp;
				normalHit = ((r.GetPoint(temp) - pos) / radius);
				return true;
			}
		}
		return false;
	}

	bool Plane::GetHit(const Ray& r, float tMin, float tMax, float& tHit, Vector3& normalHit) const
	{
		float div = Vector3::Dot(normal, r.direction);

		if (abs(div) > 0.0001f)
		{
			float t = Vector3::Dot(centre - r.origin, normal) / div;

			if (t >= 0.0001f)
			{
				tHit = t;
				normalHit = normal;
				return true;
			}
		}

		return false;
	}
}


