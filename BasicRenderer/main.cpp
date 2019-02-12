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
	scene->sun.intensity = 1.f;

	Material* white = new Material({ 1.0f, 1.0f, 1.0f });
	Material* red = new Material({ 1.0f, 0.0f, 0.0f });
	Material* green = new Material({ 0.0f, 1.0f, 0.0f });
	Material* blue = new Material({ 0.0f, 0.1f, 1.0f });

	Material* silver = new Material({ 0.972f, 0.960f, 0.915f }, Material::Type::METALLIC);
	silver->metallic = 0.5f;
	Material* copper = new Material({ 0.955f, 0.637f, 0.538f });
	Material* gold = new Material({ 1.0f, 0.766f, 0.336f });
	Material* chromium = new Material({ .550f, 0.556f, 0.554f });//, Material::Type::DIELECTRIC);

	//TODO resource manager needed
	std::shared_ptr<Mesh> bunnyMesh(ObjLoader::Load(filename));

	SceneObject* bunny = new SceneObject(bunnyMesh, red);
	bunny->transform.SetScale(10.f, 10.f, 10.f);
	bunny->transform.SetPosition(1.0f, -1.0f, -5.0f);
	//bunny->transform.Rotate(0.0f, 0.01f, 0.0f);
	scene.get()->hierarchy.push_back(bunny);

	SceneObject* bunny2 = new SceneObject(bunnyMesh);
	bunny2->transform.SetScale(10.f, 10.f, 10.f);
	bunny2->transform.SetPosition(-0.5f, -1.0f, -5.0f);
	//bunny2->transform.Rotate(0.0f, 0.01f, 0.0f);
	scene.get()->hierarchy.push_back(bunny2);

	Sphere* sp = new Sphere({ -0.25f, -0.1f, -1.0f }, 0.2f, silver);
	Sphere* sp2 = new Sphere({ 0.25f, -0.1f, -0.50f }, 0.2f, gold);
	Sphere* sp3 = new Sphere({ 0.8f, 0.0f, -1.5f }, 0.4f, chromium);
	Sphere* sp4 = new Sphere({ 0.6f, 0.0f, -3.5f }, 1.0f, copper);

	Plane* floor = new Plane({ 0.f, -0.5f, 0.0f }, { 0.f, 1.f, 0.f }, white);
	Plane* ceiling = new Plane({ 0.f, 0.5f, 0.0f }, { 0.f, -1.f, 0.f }, white);
	Plane* back = new Plane({ 0.f, 0.0f, -1.5f }, { 0.f, 0.f, 1.f }, white);
	Plane* left = new Plane({ -0.5f, 0.0f, 0.f }, { 1.f, 0.f, 0.f }, green);
	Plane* right = new Plane({ 0.5f, 0.0f, 0.f }, { -1.f, 0.f, 0.f }, red);

	scene.get()->hierarchy.push_back(sp);
	scene.get()->hierarchy.push_back(sp2);
	//scene.get()->hierarchy.push_back(sp3);
	//scene.get()->hierarchy.push_back(sp4);
	scene.get()->hierarchy.push_back(floor);
	//scene.get()->hierarchy.push_back(ceiling);
	scene.get()->hierarchy.push_back(back);
	scene.get()->hierarchy.push_back(left);
	scene.get()->hierarchy.push_back(right);

	return scene;
}

int main(int argc, char *argv[])
{
	std::string file = "../../Assets/bunny.obj";

	int width = 1920;
	int height = 1080;

	std::string outputFile = "./b";

	std::string renderingModeName = "rasterizer";
	std::string shadingModeName = "lit";

	BasicRenderer::RenderingMode renderingMode = BasicRenderer::RenderingMode::RASTERIZER;
	BasicRenderer::ShadingMode shadingMode = BasicRenderer::ShadingMode::LIT;

	if (argc == 2)
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
			<< std::endl;
		}
		else
		{
			std::cout << "Digit 'help' to show the available options" << std::endl;
		}	
		return 0;
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
		else
		{
			if (std::strcmp(argv[an], "help") != 0) std::cout << std::string(argv[an]) + " is not valid.";
			std::cout<< "Digit 'help' to show the available options" << std::endl;
			return 0;
		}

		an += 2;
	}

	std::cout << "\nOutput: " + outputFile << std::endl;
	std::cout << "Input: " + file << std::endl;
	std::cout << "Width: " << width << std::endl;
	std::cout << "Height: " << height << std::endl;
	std::cout << "Rendering: " + renderingModeName << std::endl;
	std::cout << "Shading: " + shadingModeName << std::endl;
	std::cout << "\n----- RENDERING -----\n"<<std::endl;

	
	BasicRenderer renderer;
	std::unique_ptr<World> scene = SetScene(file.c_str());

	auto beginClock = clock();
	
	std::shared_ptr<const FrameBuffer> frame = renderer.Render(width, height, *scene, renderingMode, shadingMode);
	
	auto endClock = clock();

	std::string rt = std::to_string(endClock - beginClock);

	//Saving Image
	ImageExporter::ExportToBMP(outputFile.c_str(), frame);

	std::cout << "Rendering Time: " <<rt.substr(0, rt.length() - 7).append("ms ").c_str() << "\n\n";

	/*
	int i = 0;
	while (i < 10)
	{
		beginClock = clock();

		renderer.Render(width, height, *scene, renderingMode, shadingMode);

		endClock = clock();

		rt = std::to_string(endClock - beginClock);
		std::cout << "Rendering Time: " << rt.substr(0, rt.length() - 6).append("ms ").c_str() << "\n";
		++i;
	}
	*/
	
	//getchar();
	
}

#endif
