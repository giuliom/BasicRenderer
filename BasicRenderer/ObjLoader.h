#pragma once

#include "Mesh.h"

class ObjLoader
{
public:
	static Mesh* Load(const char* filename);
};

