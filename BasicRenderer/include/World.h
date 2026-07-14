#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include "Vertex.h"
#include "DirectionalLight.h"
#include "Camera.h"
#include "SceneObject.h"

namespace BasicRenderer
{
	typedef std::unordered_map<uint, std::unique_ptr<SceneObject>> ObjectList;

	struct EnvironmentSettings
	{
		DirectionalLight m_sun;
		float m_ambientLightIntensity = 1.0f;
		Color m_ambientLightColor{ 1.f, 1.f, 1.f };

		EnvironmentSettings() noexcept {}
	};

	class World
	{
	protected:

		Transform m_root;
		ObjectList m_objectList;

		Camera m_mainCamera;
		EnvironmentSettings m_environmentSettings;

	public:

		World() {}
		World(const World& other) = delete;
		~World();

		inline const ObjectList& GetObjects() const noexcept		{ return m_objectList; }

		inline const Camera& GetMainCamera() const noexcept		{ return m_mainCamera; }
		inline Camera& GetMainCamera() noexcept					{ return m_mainCamera; }

		inline const DirectionalLight& GetSun() const noexcept	{ return m_environmentSettings.m_sun; }
		inline DirectionalLight& GetSun() noexcept				{ return m_environmentSettings.m_sun; }

		inline const EnvironmentSettings& GetEnvironmentSettings()	const noexcept	{ return m_environmentSettings; }
		inline float GetAmbientLightIntensity()						const noexcept	{ return m_environmentSettings.m_ambientLightIntensity; }
		inline void SetAmbientLightIntensity(const float intensity) noexcept		{ m_environmentSettings.m_ambientLightIntensity = intensity; }
		inline const Color& GetAmbientLightColor()					const noexcept	{ return m_environmentSettings.m_ambientLightColor; }
		inline void SetAmbientLightColor(const Color& color) noexcept			{ m_environmentSettings.m_ambientLightColor = color; }

		void Add(std::unique_ptr<SceneObject> obj, Transform* parent = nullptr);
		bool Remove(const uint id);
		SceneObject* Find(const uint id);
		const SceneObject* Find(const uint id) const;

		void Update(const TimeDuration& deltaTime);

		// Produces the world-space DrawableInstances of all visible objects.
		// The faces of every mesh instance are appended to outFaceBuffer
		// TODO: don't reprocess static drawables every frame, only when they change
		DrawableInstanceList ProcessForRendering(FaceBuffer& outFaceBuffer);
	};
}
