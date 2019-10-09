#pragma once

#include <memory>
#include "Face.h"

class Mesh
{
protected:

	int nfaces = 0;
	Face* faces = nullptr;

public:
	Mesh() = delete;
	Mesh(int nfaces, Face faces[]);
	Mesh(const Mesh& m);
	Mesh(Mesh&& m);
	~Mesh();

	Mesh& operator=(const Mesh& m) = delete;
	Mesh& operator=(Mesh&& m);
	
	int GetFacesCount() const { return nfaces; }
	const Face* const GetFaces() const { return faces; }
};

