#pragma once

#include <vector>
#include <memory>
#include "Vertex.h"
#include "DirectionalLight.h"
#include "Camera.h"

namespace BasicRenderer
{
	class Primitive;
	class Ray;

	typedef std::vector<std::shared_ptr<Primitive>> Hierarchy;

	class World
	{
	protected:

		//TODO implement it properly
		Hierarchy hierarchy;

		Camera m_mainCamera;

	public:

		World() {}
		~World();

		DirectionalLight sun;
		float ambientLightIntensity = 1.0f;
		Color ambientLightColor{ 1.f, 1.f, 1.f };

		inline const Hierarchy& GetHierarchy() const { return hierarchy; }
		inline const Camera& GetMainCamera() const { return m_mainCamera; }
		inline Camera& GetMainCamera() { return m_mainCamera; }

		void Add(Primitive* obj);
		void ProcessForRendering();
		const Primitive* Raycast(const Ray& r, float tMin, float tMax, Vector3& hitPosition, Vector3& hitNormal) const;
	};
}