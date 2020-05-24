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

		scene->sun.SetDirection({ 0.5f, -1.0f, -1.0f });
		scene->sun.intensity = 1.f;
		scene->ambientLightIntensity = 0.1f;
		scene->ambientLightColor = { 0.529f, 0.808f, 0.922f };

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

		// TODO move to transform hierarchy when ready
		const float primitive_scale = 4.f;
		const float half_primitive_scale = primitive_scale * 0.5f;
		const Vector3 primitive_scale_vector = { primitive_scale, primitive_scale, primitive_scale };
		const Vector3 box_position = { 0.f, 0.f, -half_primitive_scale };

		SceneObject* bunny = new SceneObject(bunnyMesh, gold);
		bunny->GetTransform().SetScale(primitive_scale * 3.f, primitive_scale * 3.f, primitive_scale * 3.f);
		bunny->GetTransform().SetPosition(box_position + Vector3(primitive_scale * 0.25f, primitive_scale * -0.6f, primitive_scale * 0.15f));

		Sphere* sp = new Sphere(box_position + Vector3(primitive_scale * -0.25f, primitive_scale * -0.25f, primitive_scale * 0.2f), primitive_scale * 0.2f, silver);

		Quad* floor = new Quad(quadMesh, white);
		floor->GetTransform().SetPosition(box_position + Vector3(0.f, -half_primitive_scale, 0.f));
		floor->GetTransform().RotateDeg(-90.f, 0.f, 0.f);
		floor->GetTransform().SetScale(primitive_scale_vector);

		Quad* ceiling = new Quad(quadMesh, white);
		ceiling->GetTransform().SetPosition(box_position + Vector3(0.f, half_primitive_scale, 0.f));
		ceiling->GetTransform().RotateDeg(90.f, 0.f, 0.f);
		ceiling->GetTransform().SetScale(primitive_scale_vector);

		Quad* back = new Quad(quadMesh, white);
		back->GetTransform().SetPosition(box_position + Vector3(0.f, 0.f, -half_primitive_scale ));
		back->GetTransform().RotateDeg(0.f, 0.f, 0.f);
		back->GetTransform().SetScale(primitive_scale_vector);

		Quad* front = new Quad(quadMesh, white);
		front->GetTransform().SetPosition(box_position + Vector3(0.f, 0.f, half_primitive_scale));
		front->GetTransform().RotateDeg(0.f, 180.f, 0.f);
		front->GetTransform().SetScale(primitive_scale_vector);

		Quad* left = new Quad(quadMesh, red);
		left->GetTransform().SetPosition(box_position + Vector3(-half_primitive_scale, 0.f, 0.f));
		left->GetTransform().RotateDeg(0.f, 90.f, 0.f);
		left->GetTransform().SetScale(primitive_scale_vector);

		Quad* right = new Quad(quadMesh, green);
		right->GetTransform().SetPosition(box_position + Vector3(half_primitive_scale, 0.f, 0.f));
		right->GetTransform().RotateDeg(0.f, -90.f, 0.f);
		right->GetTransform().SetScale(primitive_scale_vector);

		Quad* light = new Quad(quadMesh, emissive);
		light->GetTransform().SetPosition(box_position + Vector3(0.f, half_primitive_scale - 0.05f, 0.f));
		light->GetTransform().RotateDeg(90.f, 0.f, 0.f);
		light->GetTransform().SetScale(primitive_scale_vector * 0.3f);

		Cube* cube = new Cube(cubeMesh, white);
		cube->GetTransform().SetPosition(box_position + Vector3(primitive_scale * -0.18f, primitive_scale * -0.15f, primitive_scale * -0.2f ));
		cube->GetTransform().RotateDeg(0.f, 120.f, 0.f);
		cube->GetTransform().SetScale(primitive_scale * 0.3f, primitive_scale * 0.7f, primitive_scale * 0.3f);

		Cube* cube2 = new Cube(cubeMesh, white);
		cube2->GetTransform().SetPosition(box_position + Vector3(primitive_scale * 0.25f, primitive_scale * -0.375f, primitive_scale * -0.25f ));
		cube2->GetTransform().RotateDeg(0.f, -25.f, 0.f);
		cube2->GetTransform().SetScale(primitive_scale_vector * 0.25f);

		//scene->Add(bunny);

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