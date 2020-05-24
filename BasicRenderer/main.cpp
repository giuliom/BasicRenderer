
#include <iostream>
#include <ctime>
#include <string>
#include "Global.h"
#include "BasicRenderer.h"
#include "ObjLoader.h"
#include "SceneObject.h"
#include "Material.h"
#include "ImageExporter.h"
#include "TestScene.h"

#if !LIB_DEBUG && !LIB_RELEASE

using namespace BasicRenderer;

int main(int argc, char *argv[])
{
	std::string file = "../../../assets/bunny.obj";

	int width = 720;
	int height = 480;

	std::string outputFile = "../../../output/";

	std::string renderingModeName = "rasterizer";
	std::string shadingModeName = "lit";

	Renderer::RenderingMode renderingMode = Renderer::RenderingMode::RASTERIZER;
	Renderer::ShadingMode shadingMode = Renderer::ShadingMode::LIT;

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
				renderingMode = Renderer::RenderingMode::RAYTRACER;
			}
		
		}
		else if (std::strcmp(argv[an], "-s") == 0)
		{
				
			if (std::strcmp(argv[an + 1], "normal") == 0)
			{
				shadingModeName = argv[an + 1];
				shadingMode = Renderer::ShadingMode::NORMAL;
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

	
	Renderer renderer;
	std::unique_ptr<World> scene(TestScene());

	Raytracer& raytracer = renderer.GetRaytracer();
	raytracer.pixelSamples = pixelSamples;
	raytracer.maxBounces = maxBounces;

	auto beginClock = clock();
	
	std::shared_ptr<const FrameBuffer> frame = renderer.Render(width, height, *scene, renderingMode, shadingMode);
	
	auto endClock = clock();

	double diff = endClock - beginClock;
	double ms = diff / (CLOCKS_PER_SEC / 1000);

	//Saving Image
	ImageExporter::ExportToBMP(outputFile, renderingModeName, frame);

	std::cout <<std::endl << "Rendering Time: " << ms << " ms " << "\n\n";
	
	std::cin.get();
	
}

#endif
