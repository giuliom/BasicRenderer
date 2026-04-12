#pragma once

#include <memory>
#include <vector>
#include "Face.h"

namespace BasicRenderer
{
	class Mesh final
	{
		friend class MeshInstance;
	protected:
		std::vector<Face> m_faces;

	public:
		Mesh() = delete;
		Mesh(uint nVertices, const Vertex vertices[], uint nFaces, const Face faces[]);
		Mesh(const Mesh& m);
		Mesh(Mesh&& m) noexcept;

		Mesh& operator=(const Mesh& m) = delete;
		Mesh& operator=(Mesh&& m) noexcept;

		inline size_t NumFaces()					const noexcept { return m_faces.size(); }
		inline const std::vector<Face>& GetFaces()	const noexcept { return m_faces; }
		inline const Face& GetFace(uint index)		const noexcept { return m_faces[index]; }
	};
}
