#include "TestScene.h"
#include "Material.h"
#include "Mesh.h"
#include "ObjLoader.h"
#include "SceneObject.h"
#include "PrimitiveTypes.h"

namespace BasicRenderer
{
	// TODO move to file
	World* TestScene()
	{
		World* scene = new World();

		scene->sun.SetDirection({ 0.0f, -1.0f, -1.0f });
		scene->sun.intensity = 1.f;
		scene->ambientLightIntensity = 0.1f;
		scene->ambientLightColor = { 1.0f, 1.0f, 1.0f };

		Material* emissive = new Material({ 1.0f, 1.0f, 1.0f });
		emissive->emissive = 10.f;

		Material* white = new Material({ 1.0f, 1.0f, 1.0f });
		Material* red = new Material({ 1.0f, 0.0f, 0.0f });
		Material* green = new Material({ 0.0f, 1.0f, 0.0f });
		Material* blue = new Material({ 0.0f, 0.1f, 1.0f });

		Material* silver = new Material({ 0.972f, 0.960f, 0.915f }, Material::Type::METALLIC);
		silver->metallic = 1.0f;
		Material* copper = new Material({ 0.955f, 0.637f, 0.538f });
		Material* gold = new Material({ 1.0f, 0.766f, 0.336f }, Material::Type::METALLIC);
		gold->metallic = 0.5f;
		Material* chromium = new Material({ .550f, 0.556f, 0.554f }, Material::Type::DIELECTRIC);

		std::shared_ptr<Mesh> bunnyMesh(ObjLoader::Load("../../../assets/bunny.obj"));
		std::shared_ptr<Mesh> cubeMesh(ObjLoader::Load("../../../assets/cube.obj"));
		std::shared_ptr<Mesh> quadMesh(ObjLoader::Load("../../../assets/quad.obj"));

		SceneObject* bunny = new SceneObject(bunnyMesh, red);
		bunny->GetTransform().SetScale(10.f, 10.f, 10.f);
		bunny->GetTransform().SetPosition(0.0f, -1.0f, -3.0f);

		Sphere* sp = new Sphere({ -0.15f, -0.25f, -0.4f }, 0.2f, silver);
		Sphere* sp2 = new Sphere({ 0.25f, -0.25f, -0.4f }, 0.1f, chromium);
		Sphere* sp3 = new Sphere({ 0.8f, 0.0f, -1.5f }, 0.4f, gold);
		Sphere* sp4 = new Sphere({ 0.6f, 0.0f, -3.5f }, 1.0f, copper);

		Quad* floor = new Quad(quadMesh, white);
		floor->GetTransform().SetPosition({ 0.f, -0.5f, -0.5f });
		floor->GetTransform().RotateDeg(-90.f, 0.f, 0.f);

		Quad* ceiling = new Quad(quadMesh, white);
		ceiling->GetTransform().SetPosition({ 0.f, 0.5f, -0.5f });
		ceiling->GetTransform().RotateDeg(90.f, 0.f, 0.f);

		Quad* back = new Quad(quadMesh, white);
		back->GetTransform().SetPosition({ 0.f, 0.f, -1.0f });
		back->GetTransform().RotateDeg(0.f, 0.f, 0.f);

		Quad* front = new Quad(quadMesh, white);
		front->GetTransform().SetPosition({ 0.f, 0.f, 0.0001f });
		front->GetTransform().RotateDeg(0.f, 180.f, 0.f);

		Quad* left = new Quad(quadMesh, green);
		left->GetTransform().SetPosition({ -0.5f, 0.f, -0.5f });
		left->GetTransform().RotateDeg(0.f, 90.f, 0.f);

		Quad* right = new Quad(quadMesh, red);
		right->GetTransform().SetPosition({ 0.5f, 0.f, -0.5f });
		right->GetTransform().RotateDeg(0.f, -90.f, 0.f);

		Quad* light = new Quad(quadMesh, emissive);
		light->GetTransform().SetPosition({ 0.f, 0.4999f, -0.5f });
		light->GetTransform().RotateDeg(90.f, 0.f, 0.f);
		light->GetTransform().SetScale({ 0.3, 0.3f, 0.3f });

		Cube* cube = new Cube(cubeMesh, white);
		cube->GetTransform().SetPosition({ -0.18f, -0.2f, -0.7f });
		cube->GetTransform().SetScale(0.3f, 0.8f, 0.3f);
		cube->GetTransform().RotateDeg(0.f, 120.f, 0.f);

		Cube* cube2 = new Cube(cubeMesh, white);
		cube2->GetTransform().SetPosition({ 0.25f, -0.25f, -0.5f });
		cube2->GetTransform().SetScale(0.25f, 0.25f, 0.25f);
		cube2->GetTransform().RotateDeg(0.f, -25.f, 0.f);

		scene->Add(bunny);

		scene->Add(sp);

		scene->Add(floor);
		scene->Add(ceiling);
		scene->Add(back);
		scene->Add(front);
		scene->Add(left);
		scene->Add(right);

		scene->Add(light);

		scene->Add(cube);
		scene->Add(cube2);

		return scene;
	}

}