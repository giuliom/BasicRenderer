#pragma once

#include <memory>
#include <vector>
#include "Face.h"

namespace BasicRenderer
{
	class Mesh final
	{
	protected:
		std::vector<Face> m_faces;

	public:
		Mesh() = delete;
		explicit Mesh(std::vector<Face> faces) noexcept;
		Mesh(const Mesh& m);
		Mesh(Mesh&& m) noexcept;

		Mesh& operator=(const Mesh& m) = delete;
		Mesh& operator=(Mesh&& m) noexcept;

		inline size_t NumFaces()					const noexcept { return m_faces.size(); }
		inline const std::vector<Face>& GetFaces()	const noexcept { return m_faces; }
		inline const Face& GetFace(uint index)		const noexcept { return m_faces[index]; }
	};
}
