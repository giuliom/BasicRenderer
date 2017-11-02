#include "PrimitiveTypes.h"

Face::Face()
{
	
}

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

Face Face::ToMatrixSpace(const Matrix4& m) const
{
	return Face(	Vertex(m * v0.pos, v0.nrml, v0.uv),
					Vertex(m * v1.pos, v1.nrml, v1.uv),
					Vertex(m * v2.pos, v2.nrml, v2.uv));
}

Vector3 Face::CalculateNormal() const
{
	return Vector3(	(v0.nrml.x + v1.nrml.x + v2.nrml.x) / 3.0f,
					(v0.nrml.y + v1.nrml.y + v2.nrml.y) / 3.0f,
					(v0.nrml.z + v1.nrml.z + v2.nrml.z) / 3.0f);
}

Vertex & Vertex::operator=(const Vertex & v)
{
	pos = v.pos;
	nrml = v.nrml;
	uv = v.uv;
	return *this;
}

Vertex Vertex::GetMatrixVertex(const Matrix4& m) const
{
	return Vertex(m * pos, nrml, uv); 
}
