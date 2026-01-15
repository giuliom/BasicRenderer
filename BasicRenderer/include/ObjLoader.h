#pragma once

#include "Mesh.h"

namespace BasicRenderer
{
	class ObjLoader
	{
	public:
		static Mesh* Load(const std::string& filename);
	};

	struct FaceIndices
	{
	public:
		uint i0;
		uint i1;
		uint i2;

		FaceIndices(uint pi0, uint pi1, uint pi2) : i0(pi0), i1(pi1), i2(pi2) {}
	};
}
