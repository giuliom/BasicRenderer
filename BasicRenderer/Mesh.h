#pragma once

#include "PrimitiveTypes.h"

class Mesh
{

public:
	Mesh(int nfaces, Face faces[]);
	~Mesh();

	int nfaces = 0;
	Face* faces;

	Face* GetFaces();
};

