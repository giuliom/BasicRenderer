#include "SceneLoader.h"

#include <fstream>
#include <iostream>
#include <stack>
#include <unordered_map>

#include "World.h"
#include "Material.h"
#include "Mesh.h"
#include "ObjLoader.h"
#include "SceneObject.h"
#include "MeshInstance.h"
#include "PrimitiveTypes.h"
#include "PathUtils.h"

namespace BasicRenderer
{
	static Vector3 ParseVector3(const json& j)
	{
		return Vector3(j[0].get<float>(), j[1].get<float>(), j[2].get<float>());
	}

	static Color ParseColor(const json& j)
	{
		return Color(j[0].get<float>(), j[1].get<float>(), j[2].get<float>());
	}

	static Material::Type ParseMaterialType(const std::string& typeStr)
	{
		if (typeStr == "metallic")  return Material::Type::METALLIC;
		if (typeStr == "dielectric") return Material::Type::DIELECTRIC;
		return Material::Type::DIFFUSE;
	}

	static std::pair<std::optional<MeshInstance>, std::shared_ptr<Material>> parsePrimitive(const json& objJson, const std::string& name, const MaterialMap& materials)
	{
		std::string primitiveType = objJson.value("primitive", "");

		// Resolve material
		std::shared_ptr<Material> mat;
		if (objJson.contains("material"))
		{
			const std::string& matName = objJson["material"].get<std::string>();
			auto it = materials.find(matName);
			if (it != materials.end())
				mat = it->second;
			else
				std::cerr << "SceneLoader: Unknown material '" << matName << "' for object '" << name << "'" << std::endl;
		}

		if (primitiveType == "sphere")
		{
			Vector3 pos = Vector3(0.f, 0.f, 0.f);
			float radius = 1.0f;

			if (objJson.contains("position"))
				pos = ParseVector3(objJson["position"]);
			if (objJson.contains("radius"))
				radius = objJson["radius"].get<float>();

			Sphere sphere(pos, radius, mat.get());
			return { MeshInstance{sphere, mat}, mat };
		}
		else if (primitiveType == "plane")
		{
			Vector3 centre = Vector3(0.f, 0.f, 0.f);
			Vector3 normal = Vector3(0.f, 1.f, 0.f);

			if (objJson.contains("centre"))
				centre = ParseVector3(objJson["centre"]);
			if (objJson.contains("normal"))
				normal = ParseVector3(objJson["normal"]);

			Plane plane(centre, normal, mat.get());
			return { MeshInstance{plane, mat}, mat };
		}

		return { std::nullopt, nullptr };
	}


	// --- Helper: parse a single node ---
	std::unique_ptr<SceneObject> SceneLoader::parseSceneObject(const json& objJson, const MeshMap& meshes, const MaterialMap& materials)
	{
		const std::string name = objJson.value("name", "unnamed");
		const std::string primitiveType = objJson.value("primitive", "");

		std::unique_ptr<SceneObject> sceneObj;

		if (primitiveType == "mesh")
		{
			// Resolve material
			std::shared_ptr<Material> mat;
			if (objJson.contains("material"))
			{
				const std::string& matName = objJson["material"].get<std::string>();
				auto it = materials.find(matName);
				if (it != materials.end())
					mat = it->second;
				else
					std::cerr << "SceneLoader: Unknown material '" << matName << "' for object '" << name << "'" << std::endl;
			}

			// Resolve mesh
			if (objJson.contains("mesh"))
			{
				const std::string& meshName = objJson["mesh"].get<std::string>();
				auto it = meshes.find(meshName);
				if (it != meshes.end())
					sceneObj = std::make_unique<SceneObject>(it->second, mat, name);
				else
					std::cerr << "SceneLoader: Unknown mesh '" << meshName << "' for object '" << name << "'" << std::endl;
			}

			if (!sceneObj)
				sceneObj = std::make_unique<SceneObject>();
		}
		else
		{
			auto [instance, mat] = parsePrimitive(objJson, name, materials);

			if (instance){
				sceneObj = std::make_unique<SceneObject>(*instance, mat, name);
			}
			else
			{
				std::cerr << "SceneLoader: Unknown primitive type '" << primitiveType << "' for object '" << name << "'. Defaulting to empty SceneObject." << std::endl;
				sceneObj = std::make_unique<SceneObject>();
			}
		}

		if (objJson.contains("transform"))
		{
			const auto& t = objJson["transform"];

			if (t.contains("position"))
				sceneObj->GetTransform().SetPosition(ParseVector3(t["position"]));

			if (t.contains("rotation"))
				sceneObj->GetTransform().RotateDeg(ParseVector3(t["rotation"]));

			if (t.contains("scale"))
			{
				const auto& s = t["scale"];
				if (s.is_array())
					sceneObj->GetTransform().SetScale(ParseVector3(s));
				else
				{
					float uniform = s.get<float>();
					sceneObj->GetTransform().SetScale(uniform, uniform, uniform);
				}
			}
		}

		if (objJson.contains("enabled"))
			sceneObj->SetEnabled(objJson["enabled"].get<bool>());
		if (objJson.contains("visible"))
			sceneObj->SetVisible(objJson["visible"].get<bool>());
		return sceneObj;
	}

	std::unique_ptr<World> SceneLoader::LoadFromFile(const std::string& filepath)
	{
		std::ifstream file(filepath);
		if (!file.is_open())
		{
			std::cerr << "SceneLoader: Failed to open file: " << filepath << std::endl;
			return nullptr;
		}

		json sceneJson;
		try
		{
			sceneJson = json::parse(file);
		}
		catch (const json::parse_error& e)
		{
			std::cerr << "SceneLoader: JSON parse error: " << e.what() << std::endl;
			return nullptr;
		}

		std::unique_ptr<World> scene = std::make_unique<World>();

		// --- Environment ---
		if (sceneJson.contains("environment"))
		{
			const auto& env = sceneJson["environment"];

			if (env.contains("sun"))
			{
				const auto& sun = env["sun"];
				if (sun.contains("direction"))
					scene->GetSun().SetDirection(ParseVector3(sun["direction"]));
				if (sun.contains("intensity"))
					scene->GetSun().intensity = sun["intensity"].get<float>();
			}

			if (env.contains("ambientLightIntensity"))
				scene->SetAmbientLightIntensity(env["ambientLightIntensity"].get<float>());

			if (env.contains("ambientLightColor"))
				scene->SetAmbientLightColor(ParseColor(env["ambientLightColor"]));
		}

		// --- Camera ---
		if (sceneJson.contains("camera"))
		{
			const auto& cam = sceneJson["camera"];
			if (cam.contains("position"))
				scene->GetMainCamera().GetTransform().SetPosition(ParseVector3(cam["position"]));
			if (cam.contains("rotation"))
				scene->GetMainCamera().GetTransform().SetRotation(ParseVector3(cam["rotation"]));
			if (cam.contains("fov"))
				scene->GetMainCamera().SetFov(cam["fov"].get<float>());
		}

		// --- Materials ---
		MaterialMap materials;

		if (sceneJson.contains("materials"))
		{
			for (const auto& [name, matJson] : sceneJson["materials"].items())
			{
				Color baseColor = Color::White();
				if (matJson.contains("baseColor"))
					baseColor = ParseColor(matJson["baseColor"]);

				Material::Type type = Material::Type::DIFFUSE;
				if (matJson.contains("type"))
					type = ParseMaterialType(matJson["type"].get<std::string>());

				auto mat = std::make_shared<Material>(baseColor, type);

				if (matJson.contains("emissive"))
					mat->emissive = matJson["emissive"].get<float>();
				if (matJson.contains("metallic"))
					mat->metallic = matJson["metallic"].get<float>();
				if (matJson.contains("roughness"))
					mat->roughness = matJson["roughness"].get<float>();
				if (matJson.contains("refractiveIndex"))
					mat->refractiveIndex = matJson["refractiveIndex"].get<float>();

				materials[name] = mat;
			}
		}

		// --- Meshes ---
		MeshMap meshes;

		if (sceneJson.contains("meshes"))
		{
			for (const auto& [name, meshJson] : sceneJson["meshes"].items())
			{
				std::string meshFile = meshJson["file"].get<std::string>();
				std::shared_ptr<Mesh> mesh(ObjLoader::Load(GetAssetPath(meshFile)));
				if (mesh)
					meshes[name] = mesh;
				else
					std::cerr << "SceneLoader: Failed to load mesh: " << meshFile << std::endl;
			}
		}

		// --- Scene Objects (iterative traversal) ---
		if (sceneJson.contains("root"))
		{
			const auto& root = sceneJson["root"];
			if (root.contains("children"))
			{
				// Stack entries: (json node, parent transform pointer)
				std::stack<std::pair<const json*, Transform*>> stack;

				// Push root children in reverse order to preserve original ordering
				const auto& rootChildren = root["children"];
				for (auto it = rootChildren.rbegin(); it != rootChildren.rend(); ++it)
				{
					stack.push({ &(*it), nullptr });
				}

				while (!stack.empty())
				{
					auto [nodeJson, parentTransform] = stack.top();
					stack.pop();

					auto sceneObj = SceneLoader::parseSceneObject(*nodeJson, meshes, materials);
					Transform* currentTransform = &sceneObj->GetTransform();

					scene->Add(std::move(sceneObj), parentTransform);

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
		}

		return scene;
	}
}
