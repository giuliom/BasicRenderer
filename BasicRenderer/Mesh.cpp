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

Mesh::Mesh(const Mesh & m)
{
	nfaces = m.nfaces;
	faces = new Face[nfaces];
	memcpy(faces, m.faces, sizeof(Face) * nfaces);
}

Mesh::Mesh(Mesh && m)
{
	nfaces = m.nfaces;
	faces = m.faces;
	m.faces = nullptr;
}


Mesh::~Mesh()
{
	delete[] faces;
}


Mesh & Mesh::operator=(Mesh && m)
{
	nfaces = m.nfaces;
	faces = m.faces;
	m.faces = nullptr;
	return *this;
}


const Face* const Mesh::GetFaces() const
{
	return faces;
}


