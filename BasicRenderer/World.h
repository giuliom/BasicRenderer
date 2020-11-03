#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include "Vertex.h"
#include "DirectionalLight.h"
#include "Camera.h"

namespace BasicRenderer
{
	class Primitive;
	class BVHnode;
	class Ray;

	typedef std::unordered_map<uint, std::unique_ptr<Primitive>> ObjectList;

	class World
	{
	protected:

		//TODO implement it properly
		ObjectList m_objectList;
		std::unique_ptr<BVHnode> m_bvhRoot;

		Camera m_mainCamera;

		DirectionalLight m_sun;
		float m_ambientLightIntensity = 1.0f;
		Color m_ambientLightColor{ 1.f, 1.f, 1.f };

	public:

		World() : m_bvhRoot(nullptr) {}
		~World();

		inline const ObjectList& GetObjects() const { return m_objectList; }

		inline const Camera& GetMainCamera() const { return m_mainCamera; }
		inline Camera& GetMainCamera() { return m_mainCamera; }

		inline const DirectionalLight& GetSun() const { return m_sun; }
		inline DirectionalLight& GetSun() { return m_sun; }

		inline float GetAmbientLightIntensity() const { return m_ambientLightIntensity; }
		inline void SetAmbientLightIntensity(const float intensity) { m_ambientLightIntensity = intensity; }
		inline const Color& GetAmbientLightColor() const  { return m_ambientLightColor; }
		inline void SetAmbientLightColor(const Color& color) { m_ambientLightColor = color; }

		void Add(Primitive* obj);
		bool Remove(const uint id);
		Primitive* Find(const uint id);
		const Primitive* Find(const uint id) const;

		void BuildBoundingVolumeHierarchy();
		void ProcessForRendering();

		const Primitive* Raycast(const Ray& r, float tMin, float tMax, Vector3& hitPosition, Vector3& hitNormal) const;
	};
}