#include "Mesh.h"
#include "PrimitiveTypes.h"
#include <cassert>


Mesh::Mesh(int nfaces, Face faces[])
{
	assert(nfaces >= 0 && faces != nullptr);

	this->nfaces = nfaces;
	this->faces = new Face[nfaces];

	for (int i = 0; i < nfaces; ++i)
	{
		this->faces[i] = faces[i];
	}
}


Mesh::~Mesh()
{
	delete[] faces;
}

//TODO return copy
Face * Mesh::GetFaces()
{
	return faces;
}


