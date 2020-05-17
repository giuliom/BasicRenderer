#include "Vertex.h"

namespace BasicRenderer
{
	Vertex& Vertex::operator=(const Vertex& v)
	{
		pos = v.pos;
		nrml = v.nrml;
		uv = v.uv;
		return *this;
	}

	Vertex& Vertex::operator=(Vertex&& v)
	{
		pos = v.pos;
		nrml = v.nrml;
		uv = v.uv;
		return *this;
	}

	bool Vertex::operator==(const Vertex& v) const
	{
		return pos == v.pos && nrml == v.nrml && uv == v.uv;
	}

	bool Vertex::operator!=(const Vertex& v) const
	{
		return !(pos == v.pos && nrml == v.nrml && uv == v.uv);
	}
}

