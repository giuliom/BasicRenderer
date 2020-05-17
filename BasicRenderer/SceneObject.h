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
		std::vector<Transform*> m_children; //TODO Move to Transform?
		std::shared_ptr<Mesh> m_mesh = nullptr;
		std::vector<Face> m_transformedFaces;

	public:
		SceneObject() = delete;
		SceneObject(Material* mat) = delete;
		SceneObject(std::shared_ptr<Mesh> mesh);
		SceneObject(std::shared_ptr<Mesh> mesh, Material* mat);
		SceneObject(const SceneObject& obj);
		SceneObject(SceneObject&& obj);
		virtual ~SceneObject();

		//TODO define assigments

		inline Transform& GetTransform() { return m_transform; }
		inline const Mesh* const GetMesh() const { return m_mesh.get(); }
		inline const Transform& GetWorldTransform() const { return m_worldTransform; }
		inline size_t NumFaces() const { return m_transformedFaces.size(); }
		inline const Face& GetTransformedFace(uint index) const { return m_transformedFaces[index]; }

		void ProcessForRendering() override;
		bool GetHit(const Ray& r, float tMin, float tMax, float& tHit, Vector3& normalHit) const override;
	};
}


