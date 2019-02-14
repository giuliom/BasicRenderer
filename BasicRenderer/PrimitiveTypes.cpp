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

	if (abs(div) > 0.0001f)
	{
		float t = Vector3::Dot(centre - r.origin, normal) / div;

		if (t >= 0.0001f)
		{
			result.t = t;
			result.pos = r.GetPoint(result.t);
			result.normal = normal;
			result.material = GetMaterial();
			return true;
		}
	}

	return false;
}

bool Quad::GetHit(const Ray & r, float tMin, float tMax, HitResult & result) const
{
	result.material = this->GetMaterial();
	
	Matrix4 m = transform.GetMatrix();
	Face f0WS = f0.ToMatrixSpace(m);
	Face f1WS = f1.ToMatrixSpace(m);

	return f0WS.GetHit(r, tMin, tMax, result) || f1WS.GetHit(r, tMin, tMax, result);
}

Cube::Cube(Material * mat)
{
	material = mat;

}

bool Cube::GetHit(const Ray & r, float tMin, float tMax, HitResult & result) const
{
	Matrix4 m = transform.GetMatrix();

	Face back_f0WS = back_f0.ToMatrixSpace(m);
	Face back_f1WS = back_f1.ToMatrixSpace(m);

	Face front_f0WS = front_f0.ToMatrixSpace(m);
	Face front_f1WS = front_f1.ToMatrixSpace(m);

	Face top_f0WS = top_f0.ToMatrixSpace(m);
	Face top_f1WS = top_f1.ToMatrixSpace(m);

	Face bottom_f0WS = bottom_f0.ToMatrixSpace(m);
	Face bottom_f1WS = bottom_f1.ToMatrixSpace(m);

	Face left_f0WS = left_f0.ToMatrixSpace(m);
	Face left_f1WS = left_f1.ToMatrixSpace(m);

	Face right_f0WS = right_f0.ToMatrixSpace(m);
	Face right_f1WS = right_f1.ToMatrixSpace(m);

	result.t = tMax;
	HitResult test;
	bool hit = false;

	if (back_f0WS.GetHit(r, tMin, tMax, result) || back_f1WS.GetHit(r, tMin, tMax, test))
	{
		hit = true;
		if (test.t < result.t)
		{
			result = test;
		}
	}
	if (front_f0WS.GetHit(r, tMin, tMax, result) || front_f1WS.GetHit(r, tMin, tMax, test))
	{
		hit = true;
		if (test.t < result.t)
		{
			result = test;
		}
	}
	if (top_f0WS.GetHit(r, tMin, tMax, result) || top_f1WS.GetHit(r, tMin, tMax, test))
	{
		hit = true;
		if (test.t < result.t)
		{
			result = test;
		}
	}
	if (top_f0WS.GetHit(r, tMin, tMax, result) || top_f1WS.GetHit(r, tMin, tMax, test))
	{
		hit = true;
		if (test.t < result.t)
		{
			result = test;
		}
	}
	if (top_f0WS.GetHit(r, tMin, tMax, result) || top_f1WS.GetHit(r, tMin, tMax, test))
	{
		hit = true;
		if (test.t < result.t)
		{
			result = test;
		}
	}
	if (top_f0WS.GetHit(r, tMin, tMax, result) || top_f1WS.GetHit(r, tMin, tMax, test))
	{
		hit = true;
		if (test.t < result.t)
		{
			result = test;
		}
	}

	result.material = this->GetMaterial();

	return hit;
}
