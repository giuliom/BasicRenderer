#pragma once

#include <memory>
#include <vector>
#include "Face.h"

namespace BasicRenderer
{
	class Mesh
	{
	protected:
		std::vector<Face> m_faces;

	public:
		Mesh() = delete;
		Mesh(uint nVertices, const Vertex vertices[], uint nFaces, const Face faces[]);
		Mesh(const Mesh& m);
		Mesh(Mesh&& m);
		~Mesh();

		Mesh& operator=(const Mesh& m) = delete;
		Mesh& operator=(Mesh&& m);

		inline size_t NumFaces() const { return m_faces.size(); }
		inline const Face& GetFace(uint index) const { return m_faces[index]; }
	};
}
