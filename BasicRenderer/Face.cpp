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

bool Face::GetHit(const Ray & r, float tMin, float tMax, HitResult & result) const
{
	//Face normal
	Vector3 N = Vector3::CrossProduct(v1.pos - v0.pos, v2.pos - v0.pos).Normalize();

	//Parallel check
	float NdotRayDirection = Vector3::Dot(N, r.direction);
	if (fabs(NdotRayDirection) < 0.0001f) 
		return false; 

	float d = Vector3::Dot(N, v0.pos);

	//Behind check
	float t = (Vector3::Dot(N, r.origin) + d) / NdotRayDirection;
	if (t < 0) return false;

	Vector3 P = r.origin + r.direction * t;

	Vector3 C;

	//Edge 0
	Vector3 edge0 = v1.pos - v0.pos;
	Vector3 vp0 = P - v0.pos;
	C = Vector3::CrossProduct(edge0, vp0);
	if (Vector3::Dot(N, C) < 0) return false;

	//Edge 1
	Vector3 edge1 = v2.pos - v1.pos;
	Vector3 vp1 = P - v1.pos;
	C = Vector3::CrossProduct(edge1, vp1);
	if (Vector3::Dot(N, C) < 0)  return false;

	//Edge 2
	Vector3 edge2 = v0.pos - v2.pos;
	Vector3 vp2 = P - v2.pos;
	C = Vector3::CrossProduct(edge2, vp2);
	if (Vector3::Dot(N, C) < 0) return false;

	result.t = t;
	result.pos = r.GetPoint(result.t);
	result.normal = N;

	return true; 
}


Vector3 Face::CalculateNormal() const
{
	return Vector3((v0.nrml.x + v1.nrml.x + v2.nrml.x) * 0.333333f,
		(v0.nrml.y + v1.nrml.y + v2.nrml.y) * 0.333333f,
		(v0.nrml.z + v1.nrml.z + v2.nrml.z) * 0.333333f);
}
