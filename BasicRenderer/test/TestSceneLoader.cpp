#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <memory>

#include "SceneLoader.h"
#include "World.h"
#include "SceneObject.h"
#include "Material.h"
#include "Mesh.h"
#include "PrimitiveTypes.h"
#include "MeshInstance.h"
#include "PathUtils.h"

#define SUITE_NAME TestSceneLoader

using namespace BasicRenderer;
using json = nlohmann::json;

// Helper to find a SceneObject by name in the World
static SceneObject* FindByName(World& world, const std::string& name)
{
	for (const auto& [id, obj] : world.GetObjects())
	{
		if (obj->GetName() == name)
			return obj.get();
	}
	return nullptr;
}

// --- parseSceneObject tests ---

TEST(SUITE_NAME, ParseSceneObject_Name)
{
	json j = { {"name", "TestObj"} };
	MeshMap meshes;
	MaterialMap materials;

	auto obj = SceneLoader::parseSceneObject(j, meshes, materials);

	ASSERT_NE(obj, nullptr);
	EXPECT_EQ(obj->GetName(), "TestObj");
}

TEST(SUITE_NAME, ParseSceneObject_DefaultName)
{
	json j = json::object();
	MeshMap meshes;
	MaterialMap materials;

	auto obj = SceneLoader::parseSceneObject(j, meshes, materials);

	ASSERT_NE(obj, nullptr);
	EXPECT_EQ(obj->GetName(), "unnamed");
}

TEST(SUITE_NAME, ParseSceneObject_Transform)
{
	json j = {
		{"name", "Positioned"},
		{"transform", {
			{"position", {1.0f, 2.0f, 3.0f}},
			{"scale", {2.0f, 2.0f, 2.0f}}
		}}
	};
	MeshMap meshes;
	MaterialMap materials;

	auto obj = SceneLoader::parseSceneObject(j, meshes, materials);

	ASSERT_NE(obj, nullptr);
	const auto& pos = obj->GetTransform().GetPosition();
	EXPECT_FLOAT_EQ(pos.x, 1.0f);
	EXPECT_FLOAT_EQ(pos.y, 2.0f);
	EXPECT_FLOAT_EQ(pos.z, 3.0f);

	const auto& scale = obj->GetTransform().GetScale();
	EXPECT_FLOAT_EQ(scale.x, 2.0f);
	EXPECT_FLOAT_EQ(scale.y, 2.0f);
	EXPECT_FLOAT_EQ(scale.z, 2.0f);
}

TEST(SUITE_NAME, ParseSceneObject_UniformScale)
{
	json j = {
		{"name", "Uniform"},
		{"transform", {
			{"scale", 3.0f}
		}}
	};
	MeshMap meshes;
	MaterialMap materials;

	auto obj = SceneLoader::parseSceneObject(j, meshes, materials);

	ASSERT_NE(obj, nullptr);
	const auto& scale = obj->GetTransform().GetScale();
	EXPECT_FLOAT_EQ(scale.x, 3.0f);
	EXPECT_FLOAT_EQ(scale.y, 3.0f);
	EXPECT_FLOAT_EQ(scale.z, 3.0f);
}

TEST(SUITE_NAME, ParseSceneObject_Enabled)
{
	json j = {
		{"name", "Disabled"},
		{"enabled", false}
	};
	MeshMap meshes;
	MaterialMap materials;

	auto obj = SceneLoader::parseSceneObject(j, meshes, materials);

	ASSERT_NE(obj, nullptr);
	EXPECT_FALSE(obj->GetEnabled());
}

TEST(SUITE_NAME, ParseSceneObject_Visible)
{
	json j = {
		{"name", "Hidden"},
		{"visible", false}
	};
	MeshMap meshes;
	MaterialMap materials;

	auto obj = SceneLoader::parseSceneObject(j, meshes, materials);

	ASSERT_NE(obj, nullptr);
	EXPECT_FALSE(obj->GetVisible());
}

TEST(SUITE_NAME, ParseSceneObject_Sphere)
{
	json j = {
		{"name", "MySphere"},
		{"primitive", "sphere"},
		{"position", {1.0f, 2.0f, 3.0f}},
		{"radius", 0.5f}
	};
	MeshMap meshes;
	MaterialMap materials;

	auto obj = SceneLoader::parseSceneObject(j, meshes, materials);

	ASSERT_NE(obj, nullptr);
	ASSERT_NE(obj->GetPrimitive(), nullptr);
}

TEST(SUITE_NAME, ParseSceneObject_Plane)
{
	json j = {
		{"name", "MyPlane"},
		{"primitive", "plane"},
		{"centre", {0.0f, 0.0f, 0.0f}},
		{"normal", {0.0f, 1.0f, 0.0f}}
	};
	MeshMap meshes;
	MaterialMap materials;

	auto obj = SceneLoader::parseSceneObject(j, meshes, materials);

	ASSERT_NE(obj, nullptr);
	ASSERT_NE(obj->GetPrimitive(), nullptr);
}

TEST(SUITE_NAME, ParseSceneObject_WithMaterial)
{
	auto mat = std::make_shared<Material>(Color(1.0f, 0.0f, 0.0f), Material::Type::METALLIC);
	MaterialMap materials = { {"red_metal", mat} };
	MeshMap meshes;

	json j = {
		{"name", "MatObj"},
		{"primitive", "sphere"},
		{"material", "red_metal"}
	};

	auto obj = SceneLoader::parseSceneObject(j, meshes, materials);

	ASSERT_NE(obj, nullptr);
	ASSERT_NE(obj->GetPrimitive(), nullptr);
}

// --- LoadFromFile tests ---

TEST(SUITE_NAME, LoadFromFile_InvalidPath)
{
	auto world = SceneLoader::LoadFromFile("nonexistent_file.json");
	EXPECT_EQ(world, nullptr);
}

TEST(SUITE_NAME, LoadFromFile_CornellBox)
{
	std::string scenePath = GetAssetPath("scenes/cornell_box.json");
	auto world = SceneLoader::LoadFromFile(scenePath);

	ASSERT_NE(world, nullptr);

	// Verify environment
	EXPECT_FLOAT_EQ(world->GetAmbientLightIntensity(), 0.1f);
	EXPECT_FLOAT_EQ(world->GetSun().intensity, 1.0f);

	// Verify camera
	const auto& camPos = world->GetMainCamera().GetTransform().GetPosition();
	EXPECT_FLOAT_EQ(camPos.z, 4.0f);
}

TEST(SUITE_NAME, LoadFromFile_CornellBox_ObjectCount)
{
	std::string scenePath = GetAssetPath("scenes/cornell_box.json");
	auto world = SceneLoader::LoadFromFile(scenePath);

	ASSERT_NE(world, nullptr);

	// cornell_box.json has: Spere, Floor, Ceiling, Back, Front, Left, Right, Light, Cube, Cube2, Bunny, Teapot = 12 objects
	EXPECT_EQ(world->GetObjects().size(), 12u);
}

TEST(SUITE_NAME, LoadFromFile_CornellBox_TopLevelObjects)
{
	std::string scenePath = GetAssetPath("scenes/cornell_box.json");
	auto world = SceneLoader::LoadFromFile(scenePath);

	ASSERT_NE(world, nullptr);

	EXPECT_NE(FindByName(*world, "Spere"), nullptr);
	EXPECT_NE(FindByName(*world, "Floor"), nullptr);
	EXPECT_NE(FindByName(*world, "Cube"), nullptr);
	EXPECT_NE(FindByName(*world, "Cube2"), nullptr);
	EXPECT_NE(FindByName(*world, "Bunny"), nullptr);
}

TEST(SUITE_NAME, LoadFromFile_CornellBox_NestedChildren)
{
	std::string scenePath = GetAssetPath("scenes/cornell_box.json");
	auto world = SceneLoader::LoadFromFile(scenePath);

	ASSERT_NE(world, nullptr);

	// Children of Floor
	EXPECT_NE(FindByName(*world, "Ceiling"), nullptr);
	EXPECT_NE(FindByName(*world, "Back"), nullptr);
	EXPECT_NE(FindByName(*world, "Front"), nullptr);
	EXPECT_NE(FindByName(*world, "Left"), nullptr);
	EXPECT_NE(FindByName(*world, "Right"), nullptr);
	EXPECT_NE(FindByName(*world, "Light"), nullptr);

	// Child of Bunny
	EXPECT_NE(FindByName(*world, "Teapot"), nullptr);
}

TEST(SUITE_NAME, LoadFromFile_CornellBox_Materials)
{
	std::string scenePath = GetAssetPath("scenes/cornell_box.json");
	auto world = SceneLoader::LoadFromFile(scenePath);

	ASSERT_NE(world, nullptr);

	// Spere uses "silver" material (metallic type)
	SceneObject* sphere = FindByName(*world, "Spere");
	ASSERT_NE(sphere, nullptr);
	ASSERT_NE(sphere->GetPrimitive(), nullptr);
}

TEST(SUITE_NAME, LoadFromFile_CornellBox_Transforms)
{
	std::string scenePath = GetAssetPath("scenes/cornell_box.json");
	auto world = SceneLoader::LoadFromFile(scenePath);

	ASSERT_NE(world, nullptr);

	SceneObject* cube = FindByName(*world, "Cube");
	ASSERT_NE(cube, nullptr);

	const auto& pos = cube->GetTransform().GetPosition();
	EXPECT_FLOAT_EQ(pos.x, -0.72f);
	EXPECT_FLOAT_EQ(pos.y, -0.6f);
	EXPECT_FLOAT_EQ(pos.z, -1.2f);

	const auto& scale = cube->GetTransform().GetScale();
	EXPECT_FLOAT_EQ(scale.x, 1.2f);
	EXPECT_FLOAT_EQ(scale.y, 2.8f);
	EXPECT_FLOAT_EQ(scale.z, 1.2f);
}