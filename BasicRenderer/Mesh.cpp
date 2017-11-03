#include "Mesh.h"
#include "PrimitiveTypes.h"



Mesh::Mesh(int nfaces, Face faces[])
{
	this->nfaces = nfaces;
	this->faces = new Face[nfaces];

	for (int i = 0; i < nfaces; ++i)
	{
		this->faces[i] = faces[i];
	}
}


Mesh::~Mesh()
{
}

//TODO return copy
Face * Mesh::GetFaces()
{
	return faces;
}


