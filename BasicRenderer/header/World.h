#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include "Vertex.h"
#include "DirectionalLight.h"
#include "Camera.h"
#include "Primitive.h"
#include "SceneObject.h"

namespace BasicRenderer
{
	class Ray;

	typedef std::unordered_map<uint, std::unique_ptr<SceneObject>> ObjectList;

	struct EnvironmentSettings
	{
		DirectionalLight m_sun;
		float m_ambientLightIntensity = 1.0f;
		Color m_ambientLightColor{ 1.f, 1.f, 1.f };

		EnvironmentSettings() {}
	};

	class World
	{
	protected:

		//TODO implement it properly
		ObjectList m_objectList;

		Camera m_mainCamera;
		EnvironmentSettings m_environmentSettings;

	public:

		World() {}
		World(const World& other) = delete;
		~World();

		inline const ObjectList& GetObjects() const		{ return m_objectList; }

		inline const Camera& GetMainCamera() const		{ return m_mainCamera; }
		inline Camera& GetMainCamera()					{ return m_mainCamera; }

		inline const DirectionalLight& GetSun() const	{ return m_environmentSettings.m_sun; }
		inline DirectionalLight& GetSun()				{ return m_environmentSettings.m_sun; }

		inline const EnvironmentSettings& GetEnvironmentSettings()	const	{ return m_environmentSettings; }
		inline float GetAmbientLightIntensity()						const	{ return m_environmentSettings.m_ambientLightIntensity; }
		inline void SetAmbientLightIntensity(const float intensity)			{ m_environmentSettings.m_ambientLightIntensity = intensity; }
		inline const Color& GetAmbientLightColor()					const	{ return m_environmentSettings.m_ambientLightColor; }
		inline void SetAmbientLightColor(const Color& color)				{ m_environmentSettings. m_ambientLightColor = color; }

		void Add(SceneObject* obj);
		bool Remove(const uint id);
		SceneObject* Find(const uint id);
		const SceneObject* Find(const uint id) const;

		void Update(const TimeDuration& deltaTime);
		void ProcessForRendering(std::vector<Primitive*>& outProcessedPrimitives);

		const Primitive* OldRaycast(const Ray& r, float tMin, float tMax, HitResult& outHit) const;
	};
}