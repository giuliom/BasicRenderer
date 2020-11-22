#pragma once

#include <vector>
#include <memory>
#include "Transform.h"
#include "Primitive.h"
#include "Mesh.h"

namespace BasicRenderer
{
	class SceneObject : public Primitive
	{
	protected:
		mutable Transform m_worldTransform;
		Transform m_transform;
		Material* m_material;
		std::vector<Transform*> m_children; //TODO Move to Transform?
		std::shared_ptr<Mesh> m_mesh = nullptr;
		std::vector<Face> m_transformedFaces;

	public:
		SceneObject() = delete;
		SceneObject(Material* mat) = delete;
		SceneObject(std::shared_ptr<Mesh> mesh, Material* mat, const std::string& name = "");
		SceneObject(const SceneObject& obj);
		SceneObject(SceneObject&& obj) noexcept;
		virtual ~SceneObject();

		//TODO define assigments

		inline Transform& GetTransform() { return m_transform; }
		inline const Mesh* const GetMesh() const { return m_mesh.get(); }
		inline const Transform& GetWorldTransform() const { return m_worldTransform; }
		inline size_t NumFaces() const { return m_transformedFaces.size(); }
		inline const Face& GetTransformedFace(uint index) const { return m_transformedFaces[index]; }
		const Material* GetMaterial() const override { return m_material; }

		void ProcessForRendering() override;
		AxisAlignedBoundingBox UpdateAxisAlignedBoundingBox() const override;

		bool GetHit(const Ray& r, float tMin, float tMax, HitResult& outHit) const override
		{
			outHit.t = tMax;
			float test = 0.f;
			bool hit = false;
			const auto numFaces = m_transformedFaces.size();

			for (uint i = 0; i < numFaces; i++)
			{
				const Face& f = m_transformedFaces[i];
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


