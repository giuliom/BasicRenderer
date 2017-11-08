#include "Vertex.h"

Vertex & Vertex::operator=(const Vertex & v)
{
	pos = v.pos;
	nrml = v.nrml;
	uv = v.uv;
	return *this;
}

Vertex & Vertex::operator=(Vertex && v)
{
	pos = v.pos;
	nrml = v.nrml;
	uv = v.uv;
	return *this;
}


