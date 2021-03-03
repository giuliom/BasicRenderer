
#include <iostream>
#include <chrono>
#include <string>
#include <iomanip>
#include "Global.h"
#include "BasicRenderer.h"
#include "Model.h"
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

	int width = 1280;
	int height = 720;

	std::string outputPath = "../../../output/";

	std::string renderingModeName = "raytracer";
	std::string shadingModeName = "lit";

	Renderer::RenderingMode renderingMode = Renderer::RenderingMode::RAYTRACER;
	Renderer::ShadingMode shadingMode = Renderer::ShadingMode::LIT;

	int pixelSamples = 4;
	int maxBounces = 3;

	for (int an = 1; an < argc; ++an)
	{
		if (std::strcmp(argv[an], "help") == 0)
		{
			std::cout << "\n*** Basic Renderer ***\n\nThe following options are available:" << std::endl << std::endl
			<< "-n output image name and path" << std::endl
			<< "-f input obj file name and path" << std::endl
			<< "-w output image width" << std::endl
			<< "-h output image height" << std::endl
			<< "-r rendering mode [rasterizer, raytracer]" << std::endl
			<< "-s shading mode [lit, normal]" << std::endl
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
			outputPath = argv[an + 1];
		}
		else if (std::strcmp(argv[an], "-r") == 0)
		{	
			if (std::strcmp(argv[an + 1], "rasterizer") == 0)
			{
				renderingModeName = argv[an + 1];
				renderingMode = Renderer::RenderingMode::RASTERIZER;
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

	const std::string fileName = Renderer::GenerateFilename("cmd", height, renderingMode, shadingMode, pixelSamples, maxBounces);

	std::cout << "\n----- PARAMETERS -----" << std::endl;
	std::cout << "Output: " + outputPath + fileName + ".bmp" << std::endl;
	std::cout << "Input: " + file << std::endl;
	std::cout << "Width: " << width << std::endl;
	std::cout << "Height: " << height << std::endl;
	std::cout << "Rendering: " + renderingModeName << std::endl;
	std::cout << "Shading: " + shadingModeName << std::endl;
	std::cout << "Pixel Samples: " << pixelSamples << std::endl;
	std::cout << "Max Bounces: " << maxBounces << std::endl;

	std::cout << "\n----- RENDERING -----\n"<<std::endl;

	
	Renderer renderer;
	std::unique_ptr<Model> model = std::make_unique<Model>(TestScene());

	model->SetMainCameraAspectRatio(static_cast<float>(width), static_cast<float>(height));
	model->Update(model->UpdateTime());
	std::unique_ptr<RenderState> renderState(model->ProcessForRendering());

	Raytracer& raytracer = renderer.GetRaytracer();
	raytracer.m_pixelSamples = pixelSamples;
	raytracer.m_maxBounces = maxBounces;

	const auto beginTime = std::chrono::high_resolution_clock::now();
	
	const FrameBuffer* frame = renderer.Render(*renderState, width, height, renderingMode, shadingMode);
	
	const auto endTime = std::chrono::high_resolution_clock::now();

	double ms = ConvertChronoDuration<double, std::chrono::milliseconds>(endTime - beginTime);

	//Saving Image
	ImageExporter::ExportToBMP(outputPath, fileName, *frame);

	std::cout << std::fixed << std::setprecision(0);
	std::cout << std::endl << "Rendering Time: " << ms << " ms " << "\n\n";
	std::cout << std::scientific;
	
	std::cin.get();
	
}

#endif
