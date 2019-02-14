#include "Face.h"
#include "Hitable.h"

Face::Face(const Position& p0, const Position& p1, const  Position& p2, const Face& face)
{
	v0 = Vertex(p0, face.v0.nrml, face.v0.uv);
	v1 = Vertex(p1, face.v1.nrml, face.v1.uv);
	v2 = Vertex(p2, face.v2.nrml, face.v2.uv);
}

Face & Face::operator=(const Face & f)
{
	v0 = f.v0;
	v1 = f.v1;
	v2 = f.v2;
	return *this;
}

Face & Face::operator=(Face && f)
{
	v0 = f.v0;
	v1 = f.v1;
	v2 = f.v2;
	return *this;
}

//Möller–Trumbore intersection algorithm
bool Face::GetHit(const Ray & r, float tMin, float tMax, HitResult & result) const
{
	const float EPSILON = 0.0000001;
	Vector3 faceNormal = Vector3::CrossProduct(v1.pos - v0.pos, v2.pos - v0.pos).Normalize();

	Vector3 edge1, edge2, h, s, q;
	float a, f, u, v;
	edge1 = v1.pos - v0.pos;
	edge2 = v2.pos - v0.pos;
	h = Vector3::CrossProduct(r.direction, edge2);
	a = Vector3::Dot(edge1, h);
	
	if (a > -EPSILON && a < EPSILON)
	{
		return false;    // This ray is parallel to this triangle.
	}
	
	f = 1.0 / a;
	s = r.origin - v0.pos;
	u = f * Vector3::Dot(s, h);
	
	if (u < 0.0 || u > 1.0)
	{
		return false;
	}
	
	q = Vector3::CrossProduct(s, edge1);
	v = f * Vector3::Dot(r.direction, q);
	
	if (v < 0.0 || u + v > 1.0)
	{
		return false;
	}
	
	// At this stage we can compute t to find out where the intersection point is on the line.
	float t = f * Vector3::Dot(edge2, q);
	
	if (t > EPSILON) // ray intersection
	{
		result.t = t;
		result.pos = r.GetPoint(result.t);
		result.normal = faceNormal;
		return true;
	}
	else // This means that there is a line intersection but not a ray intersection.
	{
		return false;
	}
	
}


Vector3 Face::CalculateNormal() const
{
	return Vector3((v0.nrml.x + v1.nrml.x + v2.nrml.x) * 0.333333f,
		(v0.nrml.y + v1.nrml.y + v2.nrml.y) * 0.333333f,
		(v0.nrml.z + v1.nrml.z + v2.nrml.z) * 0.333333f);
}
