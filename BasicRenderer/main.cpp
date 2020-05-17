#include "BasicRenderer.h"
#include "ObjLoader.h"
#include <iostream>
#include <ctime>
#include <string>
#include "SceneObject.h"
#include "Material.h"
#include "ImageExporter.h"

#if !LIB_DEBUG && !LIB_RELEASE


std::unique_ptr<World> SetScene(const char* filename)
{
	std::unique_ptr<World> scene = std::make_unique<World>();

	scene->sun.SetDirection({ 0.0f, -1.0f, 0.0f });
	scene->sun.intensity = 0.f;
	scene->ambientLightIntensity = 0.0f;

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

	//TODO resource manager needed
	std::shared_ptr<Mesh> bunnyMesh(ObjLoader::Load(filename));
	std::shared_ptr<Mesh> cubeMesh(ObjLoader::Load("../../../assets/cube.obj"));
	std::shared_ptr<Mesh> quadMesh(ObjLoader::Load("../../../assets/quad.obj"));

	SceneObject* bunny = new SceneObject(bunnyMesh, red);
	bunny->GetTransform().SetScale(10.f, 10.f, 10.f);
	bunny->GetTransform().SetPosition(0.0f, -1.0f, -5.0f);

	Sphere* sp = new Sphere({ -0.15f, -0.25f, -0.4f }, 0.15f, silver);
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

	//scene.get()->Add(bunny);

	scene.get()->Add(sp);

	scene.get()->Add(floor);
	scene.get()->Add(ceiling);
	scene.get()->Add(back);
	scene.get()->Add(front);
	scene.get()->Add(left);
	scene.get()->Add(right);

	scene.get()->Add(light);

	scene.get()->Add(cube);
	scene.get()->Add(cube2);

	return scene;
}

int main(int argc, char *argv[])
{
	std::string file = "../../../assets/bunny.obj";

	int width = 720;
	int height = 480;

	std::string outputFile = "../../../output/";

	std::string renderingModeName = "rasterizer";
	std::string shadingModeName = "lit";

	BasicRenderer::RenderingMode renderingMode = BasicRenderer::RenderingMode::RASTERIZER;
	BasicRenderer::ShadingMode shadingMode = BasicRenderer::ShadingMode::LIT;

	int pixelSamples = 4;
	int maxBounces = 3;

	if (argc < 2)
	{
		if (std::strcmp(argv[1], "help") == 0)
		{
			std::cout << "The following options are available:" << std::endl << std::endl
			<< "-n output image name and path" << std::endl
			<< "-f input obj file name and path" << std::endl
			<< "-w output image width" << std::endl
			<< "-h output image height" << std::endl
			<< "-r rendering mode" << std::endl
			<< "-s shading mode" << std::endl
			<< "-p pixel samples" << std::endl
			<< "-b max bounces" << std::endl
			<< std::endl;
			std::cin.get();
			return 0;
		}
	}


	for (int an = 1; an < argc -1;)
	{
		if (std::strcmp(argv[an], "-w") == 0)
		{
			width = atoi(argv[an + 1]);
		}
		else if (std::strcmp(argv[an], "-h") == 0)
		{
			height = atoi(argv[an + 1]);
		}
		else if (std::strcmp(argv[an], "-n") == 0)
		{
			outputFile = argv[an + 1];
		}
		else if (std::strcmp(argv[an], "-r") == 0)
		{
		
			if (std::strcmp(argv[an + 1], "raytracer") == 0)
			{
				renderingModeName = argv[an + 1];
				renderingMode = BasicRenderer::RenderingMode::RAYTRACER;
			}
		
		}
		else if (std::strcmp(argv[an], "-s") == 0)
		{
				
			if (std::strcmp(argv[an + 1], "normal") == 0)
			{
				shadingModeName = argv[an + 1];
				shadingMode = BasicRenderer::ShadingMode::NORMAL;
			}
		}
		else if (std::strcmp(argv[an], "-f") == 0)
		{
			file = argv[an + 1];
		}
		else if (std::strcmp(argv[an], "-p") == 0)
		{
			pixelSamples = atoi(argv[an + 1]);
		}
		else if (std::strcmp(argv[an], "-b") == 0)
		{
			maxBounces = atoi(argv[an + 1]);
		}
		else
		{
			if (std::strcmp(argv[an], "help") != 0) std::cout << std::string(argv[an]) + " is not valid.";
			std::cout<< "Digit 'help' to show the available options" << std::endl;
			std::cin.get();
			return 0;
		}

		an += 2;
	}

	std::cout << "\n----- PARAMETERS -----" << std::endl;
	std::cout << "Output: " + outputFile << std::endl;
	std::cout << "Input: " + file << std::endl;
	std::cout << "Width: " << width << std::endl;
	std::cout << "Height: " << height << std::endl;
	std::cout << "Rendering: " + renderingModeName << std::endl;
	std::cout << "Shading: " + shadingModeName << std::endl;
	std::cout << "Pixel Samples: " << pixelSamples << std::endl;
	std::cout << "Max Bounces: " << maxBounces << std::endl;

	//std::cout << std::endl << "Press any key to start" << std::endl;
	//std::cin.get();

	std::cout << "\n----- RENDERING -----\n"<<std::endl;

	
	BasicRenderer renderer;
	std::unique_ptr<World> scene = SetScene(file.c_str());
	renderer.camera.SetFov(85.f);

	auto beginClock = clock();
	
	std::shared_ptr<const FrameBuffer> frame = renderer.Render(width, height, *scene, renderingMode, shadingMode, pixelSamples, maxBounces);
	
	auto endClock = clock();

	std::string rt = std::to_string(endClock - beginClock);

	//Saving Image
	ImageExporter::ExportToBMP(outputFile.c_str(), frame);

	std::cout <<std::endl << "Rendering Time: " <<rt.substr(0, rt.length() - 7).append(" ms ").c_str() << "\n\n";
	
	std::cin.get();
	
}

#endif
