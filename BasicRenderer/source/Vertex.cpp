#include "Vertex.h"

namespace BasicRenderer
{
	bool Vertex::operator==(const Vertex& v) const noexcept
	{
		return pos == v.pos && nrml == v.nrml && uv == v.uv;
	}

	bool Vertex::operator!=(const Vertex& v) const noexcept
	{
		return !(pos == v.pos && nrml == v.nrml && uv == v.uv);
	}
}

