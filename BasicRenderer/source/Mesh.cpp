#include "Mesh.h"
#include "PrimitiveTypes.h"
#include <cassert>

namespace BasicRenderer
{
	Mesh::Mesh(uint nVertices, const Vertex vertices[], uint nFaces, const Face faces[])
	{
		(void)nVertices;
		(void)vertices;
		assert(nFaces >= 0 && faces != nullptr);

		for (uint i = 0; i < nFaces; ++i)
		{
			m_faces.emplace_back(faces[i]);
		}
	}

	Mesh::Mesh(const Mesh& m)
	{
		m_faces = m.m_faces;
	}

	Mesh::Mesh(Mesh&& m)
	{
		m_faces = m.m_faces;
	}


	Mesh::~Mesh()
	{

	}


	Mesh& Mesh::operator=(Mesh&& m)
	{
		m_faces = m.m_faces;
		return *this;
	}
}


