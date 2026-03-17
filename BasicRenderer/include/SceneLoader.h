#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>

namespace BasicRenderer
{
	class Mesh;
	class Material;
	class SceneObject;
	class World;

	using json = nlohmann::json;
	using MeshMap = std::unordered_map<std::string, std::shared_ptr<Mesh>>;
	using MaterialMap = std::unordered_map<std::string, std::shared_ptr<Material>>;

	class SceneLoader
	{
	public:
		static std::unique_ptr<SceneObject> parseSceneObject(const json& objJson, const MeshMap& meshes, const MaterialMap& materials);
		static std::unique_ptr<World> LoadFromFile(const std::string& filepath);
	};
}
