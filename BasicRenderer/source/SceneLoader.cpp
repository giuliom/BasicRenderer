#include "SceneLoader.h"

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <nlohmann/json.hpp>

#include "World.h"
#include "Material.h"
#include "Mesh.h"
#include "ObjLoader.h"
#include "SceneObject.h"
#include "PrimitiveTypes.h"
#include "PathUtils.h"

using json = nlohmann::json;

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
		std::unordered_map<std::string, std::shared_ptr<Material>> materials;

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
		std::unordered_map<std::string, std::shared_ptr<Mesh>> meshes;

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

		// --- Objects ---
		if (sceneJson.contains("objects"))
		{
			for (const auto& objJson : sceneJson["objects"])
			{
				std::string name = objJson.value("name", "");

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

				SceneObject* sceneObj = nullptr;

				std::string primitiveType = objJson.value("primitive", "");

				if (primitiveType == "sphere")
				{
					Vector3 pos = Vector3(0.f, 0.f, 0.f);
					float radius = 1.0f;

					if (objJson.contains("position"))
						pos = ParseVector3(objJson["position"]);
					if (objJson.contains("radius"))
						radius = objJson["radius"].get<float>();

					Sphere* sphere = new Sphere(pos, radius, mat, name);
					sceneObj = new SceneObject(sphere, name);
				}
				else if (primitiveType == "plane")
				{
					Vector3 centre = Vector3(0.f, 0.f, 0.f);
					Vector3 normal = Vector3(0.f, 1.f, 0.f);

					if (objJson.contains("centre"))
						centre = ParseVector3(objJson["centre"]);
					if (objJson.contains("normal"))
						normal = ParseVector3(objJson["normal"]);

					Plane* plane = new Plane(centre, normal, mat, name);
					sceneObj = new SceneObject(plane, name);
				}
				else
				{
					// Mesh-based object
					if (objJson.contains("mesh"))
					{
						const std::string& meshName = objJson["mesh"].get<std::string>();
						auto it = meshes.find(meshName);
						if (it != meshes.end())
							sceneObj = new SceneObject(it->second, mat, name);
						else
							std::cerr << "SceneLoader: Unknown mesh '" << meshName << "' for object '" << name << "'" << std::endl;
					}
				}

				if (sceneObj == nullptr)
					continue;

				// Apply transform
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

				scene->Add(sceneObj);
			}
		}

		return scene;
	}
}
