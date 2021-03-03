#pragma once

#include "Global.h"
#include "Mesh.h"
#include "Primitive.h"

namespace BasicRenderer
{
	class MeshInstance : public Primitive
	{
		std::shared_ptr<Mesh> m_originalMesh;

	public:

		MeshInstance(std::shared_ptr<Mesh>& mesh, std::shared_ptr<Material> mat = nullptr, const std::string& name = "");

		void ProcessForRendering(const Transform& transform) override;
		AxisAlignedBoundingBox UpdateAxisAlignedBoundingBox() const override;

		bool GetHit(const Ray& r, float tMin, float tMax, HitResult& outHit) const override
		{
			outHit.t = tMax;
			float test = 0.f;
			bool hit = false;
			const auto numFaces = m_faces.size();

			for (uint i = 0; i < numFaces; i++)
			{
				const Face& f = m_faces[i];
				if (Intersection(f, r, tMin, tMax, test))
				{
					hit = true;
					if (test < outHit.t)
					{
						outHit.t = test;
						outHit.normal = f.normal;
					}
				}
			}

			return hit;
		}
	};
}