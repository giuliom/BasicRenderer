#include "Mesh.h"
#include "PrimitiveTypes.h"
#include <cassert>

namespace BasicRenderer
{
	Mesh::Mesh(std::vector<Face> faces) noexcept : m_faces(std::move(faces)) {}

	Mesh::Mesh(const Mesh& m)
	{
		m_faces = m.m_faces;
	}

	Mesh::Mesh(Mesh&& m) noexcept
	{
		m_faces = std::move(m.m_faces);
	}

	Mesh& Mesh::operator=(Mesh&& m) noexcept
	{
		m_faces = std::move(m.m_faces);
		return *this;
	}
}


