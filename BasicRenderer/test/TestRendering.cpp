#include <gtest/gtest.h>
#include <memory>
#include <BasicRenderer.h>
#include <Model.h>

#define SUITE_NAME TestRendering

using namespace BasicRenderer;

World* TestScene()
{
	World* scene = new World();

		scene->GetSun().SetDirection({ 0.5f, -1.0f, -1.0f });
		scene->GetSun().intensity = 1.f;
		scene->SetAmbientLightIntensity(0.1f);
		scene->SetAmbientLightColor({ 0.529f, 0.808f, 0.922f });

		scene->GetMainCamera().GetTransform().SetPosition(0.f, 0.f, 4.f);

		std::shared_ptr<Material> emissive(new Material({ 1.0f, 1.0f, 1.0f }));
		emissive->emissive = 10.f;

		std::shared_ptr<Material> gold(new Material({ 1.0f, 0.766f, 0.336f }, Material::Type::METALLIC));
		gold->metallic = 0.5f;

		// TODO move to transform hierarchy when ready
		const float primitive_scale = 4.f;
		const float half_primitive_scale = primitive_scale * 0.5f;
		const Vector3 primitive_scale_vector = { primitive_scale, primitive_scale, primitive_scale };
		const Vector3 box_position = { 0.f, 0.f, 0.f };

		Sphere* sp = new Sphere(box_position + Vector3(primitive_scale * -0.25f, primitive_scale * -0.25f, primitive_scale * 0.2f), primitive_scale * 0.15f, gold, "");
		SceneObject* sphere = new SceneObject(sp, "Spere");

		Sphere* light_sp = new Sphere(box_position + Vector3(primitive_scale * -0.25f, primitive_scale * -0.25f, primitive_scale * 0.2f), primitive_scale * 0.15f, emissive, "");
		SceneObject* light = new SceneObject(light_sp, "Light");
		light->GetTransform().SetPosition(box_position + Vector3(0.f, half_primitive_scale - 0.05f, 0.f));
		light->GetTransform().RotateDeg(90.f, 0.f, 0.f);
		light->GetTransform().SetScale(primitive_scale_vector * 0.3f);

		scene->Add(sphere);
		scene->Add(light);

		return scene;
}


TEST(SUITE_NAME, Raytracer)
{
	Renderer renderer;
	std::unique_ptr<Model> model = std::make_unique<Model>(TestScene());

	const uint width = 320;
	const uint height = 240;
	model->SetMainCameraAspectRatio(static_cast<float>(width), static_cast<float>(height));
	std::unique_ptr<RenderState> renderState(model->ProcessForRendering());

	Raytracer& raytracer = renderer.GetRaytracer();
	raytracer.m_pixelSamples = 4;
	raytracer.m_maxBounces = 2;
	
	const FrameBuffer* frame = renderer.Render(*renderState, width, height, Renderer::RenderingMode::RAYTRACER, Renderer::ShadingMode::LIT);
	
	EXPECT_TRUE(frame != nullptr);
}

TEST(SUITE_NAME, Rasterizer)
{
	Renderer renderer;
	std::unique_ptr<Model> model = std::make_unique<Model>(TestScene());

	const uint width = 320;
	const uint height = 240;
	model->SetMainCameraAspectRatio(static_cast<float>(width), static_cast<float>(height));
	std::unique_ptr<RenderState> renderState(model->ProcessForRendering());
	
	const FrameBuffer* frame = renderer.Render(*renderState, width, height, Renderer::RenderingMode::RASTERIZER, Renderer::ShadingMode::LIT);
	
	EXPECT_TRUE(frame != nullptr);
}