#pragma once

#include <vector>
#include <memory>
#include "Vertex.h"
#include "DirectionalLight.h"
#include "Camera.h"
#include "AccelerationStructure.h"

namespace BasicRenderer
{
	class Primitive;
	class Ray;

	typedef BoundingVolumeHierarchy AccelerationStructure;

	class World
	{
	protected:

		//TODO implement it properly
		ObjectList m_objectList;

		Camera m_mainCamera;

		DirectionalLight m_sun;
		float m_ambientLightIntensity = 1.0f;
		Color m_ambientLightColor{ 1.f, 1.f, 1.f };
		AccelerationStructure m_bvh;

	public:

		World() {}
		~World();

		inline const ObjectList& GetObjects() const		{ return m_objectList; }

		inline const Camera& GetMainCamera() const		{ return m_mainCamera; }
		inline Camera& GetMainCamera()					{ return m_mainCamera; }

		inline const DirectionalLight& GetSun() const	{ return m_sun; }
		inline DirectionalLight& GetSun()				{ return m_sun; }

		inline float GetAmbientLightIntensity() const					{ return m_ambientLightIntensity; }
		inline void SetAmbientLightIntensity(const float intensity)		{ m_ambientLightIntensity = intensity; }
		inline const Color& GetAmbientLightColor() const				{ return m_ambientLightColor; }
		inline void SetAmbientLightColor(const Color& color)			{ m_ambientLightColor = color; }
		const AccelerationStructure& GetAccelerationStructure() const	{ return m_bvh; }

		void Add(SceneObject* obj);
		bool Remove(const uint id);
		SceneObject* Find(const uint id);
		const SceneObject* Find(const uint id) const;

		void Update(const TimeDuration& deltaTime);
		void ProcessForRendering();

		const Primitive* OldRaycast(const Ray& r, float tMin, float tMax, HitResult& outHit) const;
		inline const Primitive* Raycast(const Ray& r, float tMin, float tMax, std::vector<const BVHnode*>& dfsStack, Vector3& hitPosition, Vector3& hitNormal) const
		{
			const Primitive* anyHit = nullptr;
			HitResult hit;
			anyHit = m_bvh.GetHit(r, tMin, tMax, dfsStack, hit);

			if (anyHit != nullptr)
			{
				hitPosition = r.GetPoint(hit.t);
				hitNormal = hit.normal;
			}

			return anyHit;
		}
	};
}