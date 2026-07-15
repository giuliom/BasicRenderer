#include "SceneLoader.h"

#include <fstream>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <nlohmann/json.hpp>

#include "World.h"
#include "Material.h"
#include "Mesh.h"
#include "ObjLoader.h"
#include "SceneObject.h"
#include "DrawableInstance.h"
#include "PathUtils.h"

namespace BasicRenderer
{
	static Vector3 ParseVector3(const json& j)
	{
		return Vector3(j.at(0).get<float>(), j.at(1).get<float>(), j.at(2).get<float>());
	}

	static Color ParseColor(const json& j)
	{
		return Color(j.at(0).get<float>(), j.at(1).get<float>(), j.at(2).get<float>());
	}

	static Material::Type ParseMaterialType(const std::string& typeStr)
	{
		if (typeStr == "metallic")  return Material::Type::METALLIC;
		if (typeStr == "dielectric") return Material::Type::DIELECTRIC;
		return Material::Type::DIFFUSE;
	}

	static std::shared_ptr<Material> ResolveMaterial(const json& objJson, const std::string& name, const MaterialMap& materials)
	{
		if (!objJson.contains("material"))
			return nullptr;

		const std::string matName = objJson["material"].get<std::string>();
		auto it = materials.find(matName);
		if (it == materials.end())
		{
			std::cerr << "SceneLoader: Unknown material '" << matName << "' for object '" << name << "'" << std::endl;
			return nullptr;
		}
		return it->second;
	}

	static std::optional<DrawableInstance> ParsePrimitive(const json& objJson, const std::string& primitiveType, std::shared_ptr<Material> mat)
	{
		if (primitiveType == "sphere")
		{
			Vector3 pos = Vector3(0.f, 0.f, 0.f);
			if (objJson.contains("position"))
				pos = ParseVector3(objJson["position"]);

			const float radius = objJson.value("radius", 1.0f);

			return DrawableInstance(Sphere(pos, radius), std::move(mat));
		}
		else if (primitiveType == "plane")
		{
			Vector3 centre = Vector3(0.f, 0.f, 0.f);
			Vector3 normal = Vector3(0.f, 1.f, 0.f);

			if (objJson.contains("centre"))
				centre = ParseVector3(objJson["centre"]);
			if (objJson.contains("normal"))
				normal = ParseVector3(objJson["normal"]);

			return DrawableInstance(Plane(centre, normal), std::move(mat));
		}

		return std::nullopt;
	}

	static void ParseTransform(const json& objJson, Transform& transform)
	{
		if (!objJson.contains("transform"))
			return;

		const auto& t = objJson["transform"];

		if (t.contains("position"))
			transform.SetPosition(ParseVector3(t["position"]));

		if (t.contains("rotation"))
			transform.RotateDeg(ParseVector3(t["rotation"]));

		if (t.contains("scale"))
		{
			const auto& s = t["scale"];
			if (s.is_array())
				transform.SetScale(ParseVector3(s));
			else
			{
				const float uniform = s.get<float>();
				transform.SetScale(uniform, uniform, uniform);
			}
		}
	}

	std::unique_ptr<SceneObject> SceneLoader::ParseSceneObject(const json& objJson, const MeshMap& meshes, const MaterialMap& materials)
	{
		const std::string name = objJson.value("name", "unnamed");
		const std::string primitiveType = objJson.value("primitive", "");

		std::shared_ptr<Material> mat = ResolveMaterial(objJson, name, materials);

		std::unique_ptr<SceneObject> sceneObj;

		if (primitiveType == "mesh")
		{
			if (objJson.contains("mesh"))
			{
				const std::string meshName = objJson["mesh"].get<std::string>();
				auto it = meshes.find(meshName);
				if (it != meshes.end())
					sceneObj = std::make_unique<SceneObject>(it->second, std::move(mat), name);
				else
					std::cerr << "SceneLoader: Unknown mesh '" << meshName << "' for object '" << name << "'" << std::endl;
			}

			if (!sceneObj)
				sceneObj = std::make_unique<SceneObject>(name);
		}
		else
		{
			auto drawable = ParsePrimitive(objJson, primitiveType, std::move(mat));

			if (drawable)
			{
				sceneObj = std::make_unique<SceneObject>(std::move(*drawable), name);
			}
			else
			{
				std::cerr << "SceneLoader: Unknown primitive type '" << primitiveType << "' for object '" << name << "'. Defaulting to empty SceneObject." << std::endl;
				sceneObj = std::make_unique<SceneObject>(name);
			}
		}

		ParseTransform(objJson, sceneObj->GetTransform());

		if (objJson.contains("enabled"))
			sceneObj->SetEnabled(objJson["enabled"].get<bool>());
		if (objJson.contains("visible"))
			sceneObj->SetVisible(objJson["visible"].get<bool>());

		return sceneObj;
	}

	static void ParseEnvironment(const json& sceneJson, World& scene)
	{
		if (!sceneJson.contains("environment"))
			return;

		const auto& env = sceneJson["environment"];

		if (env.contains("sun"))
		{
			const auto& sun = env["sun"];
			if (sun.contains("direction"))
				scene.GetSun().SetDirection(ParseVector3(sun["direction"]));
			if (sun.contains("intensity"))
				scene.GetSun().intensity = sun["intensity"].get<float>();
		}

		if (env.contains("ambientLightIntensity"))
			scene.SetAmbientLightIntensity(env["ambientLightIntensity"].get<float>());

		if (env.contains("ambientLightColor"))
			scene.SetAmbientLightColor(ParseColor(env["ambientLightColor"]));
	}

	static void ParseCamera(const json& sceneJson, World& scene)
	{
		if (!sceneJson.contains("camera"))
			return;

		const auto& cam = sceneJson["camera"];
		if (cam.contains("position"))
			scene.GetMainCamera().GetTransform().SetPosition(ParseVector3(cam["position"]));
		if (cam.contains("rotation"))
			scene.GetMainCamera().GetTransform().SetRotation(ParseVector3(cam["rotation"]));
		if (cam.contains("fov"))
			scene.GetMainCamera().SetFov(cam["fov"].get<float>());
	}

	static MaterialMap ParseMaterials(const json& sceneJson)
	{
		MaterialMap materials;

		if (!sceneJson.contains("materials"))
			return materials;

		for (const auto& [name, matJson] : sceneJson["materials"].items())
		{
			Color baseColor = Color::White();
			if (matJson.contains("baseColor"))
				baseColor = ParseColor(matJson["baseColor"]);

			Material::Type type = Material::Type::DIFFUSE;
			if (matJson.contains("type"))
				type = ParseMaterialType(matJson["type"].get<std::string>());

			auto mat = std::make_shared<Material>(baseColor, type);

			mat->emissive = matJson.value("emissive", mat->emissive);
			mat->metallic = matJson.value("metallic", mat->metallic);
			mat->roughness = matJson.value("roughness", mat->roughness);
			mat->refractiveIndex = matJson.value("refractiveIndex", mat->refractiveIndex);

			materials[name] = std::move(mat);
		}

		return materials;
	}

	static MeshMap ParseMeshes(const json& sceneJson)
	{
		MeshMap meshes;

		if (!sceneJson.contains("meshes"))
			return meshes;

		for (const auto& [name, meshJson] : sceneJson["meshes"].items())
		{
			const std::string meshFile = meshJson["file"].get<std::string>();
			std::shared_ptr<Mesh> mesh = ObjLoader::Load(GetAssetPath(meshFile));
			if (mesh)
				meshes[name] = std::move(mesh);
			else
				std::cerr << "SceneLoader: Failed to load mesh: " << meshFile << std::endl;
		}

		return meshes;
	}

	static void ParseHierarchy(const json& sceneJson, World& scene, const MeshMap& meshes, const MaterialMap& materials)
	{
		if (!sceneJson.contains("root") || !sceneJson["root"].contains("children"))
			return;

		// Stack entries: (json node, parent transform pointer)
		std::stack<std::pair<const json*, Transform*>> stack;

		// Push root children in reverse order to preserve original ordering
		const auto& rootChildren = sceneJson["root"]["children"];
		for (auto it = rootChildren.rbegin(); it != rootChildren.rend(); ++it)
		{
			stack.push({ &(*it), nullptr });
		}

		while (!stack.empty())
		{
			auto [nodeJson, parentTransform] = stack.top();
			stack.pop();

			auto sceneObj = SceneLoader::ParseSceneObject(*nodeJson, meshes, materials);
			Transform* currentTransform = &sceneObj->GetTransform();

			scene.Add(std::move(sceneObj), parentTransform);

			// Push children in reverse order to preserve original ordering
			if (nodeJson->contains("children"))
			{
				const auto& children = (*nodeJson)["children"];
				for (auto it = children.rbegin(); it != children.rend(); ++it)
				{
					stack.push({ &(*it), currentTransform });
				}
			}
		}
	}

	std::unique_ptr<World> SceneLoader::LoadFromFile(const std::string& filepath)
	{
		std::ifstream file(filepath);
		if (!file.is_open())
		{
			std::cerr << "SceneLoader: Failed to open file: " << filepath << std::endl;
			return nullptr;
		}

		try
		{
			const json sceneJson = json::parse(file);

			std::unique_ptr<World> scene = std::make_unique<World>();

			ParseEnvironment(sceneJson, *scene);
			ParseCamera(sceneJson, *scene);

			const MaterialMap materials = ParseMaterials(sceneJson);
			const MeshMap meshes = ParseMeshes(sceneJson);

			ParseHierarchy(sceneJson, *scene, meshes, materials);

			return scene;
		}
		catch (const json::exception& e)
		{
			std::cerr << "SceneLoader: Failed to load scene '" << filepath << "': " << e.what() << std::endl;
			return nullptr;
		}
	}
}
