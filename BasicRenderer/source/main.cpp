
#include <iostream>
#include <chrono>
#include <string>
#include "Global.h"
#include "PathUtils.h"
#include "BasicRenderer.h"
#include "Model.h"
#include "ObjLoader.h"
#include "SceneObject.h"
#include "Material.h"
#include "ImageExporter.h"
#include "SceneLoader.h"
#include <argvnaut.h>

#if !LIB_DEBUG && !LIB_RELEASE

using namespace BasicRenderer;

int main(int argc, char *argv[])
{
	ArgvNaut::Parser parser("BasicRenderer");

	parser.addFlag("help", "h");
	parser.addOption("width", "w", ArgvNaut::OptionType::INTEGER, "1280");
	parser.addOption("height", "e", ArgvNaut::OptionType::INTEGER, "720");
	parser.addOption("output", "o", ArgvNaut::OptionType::STRING, "");
	parser.addOption("rendering", "r", ArgvNaut::OptionType::STRING, "raytracer");
	parser.addOption("shading", "s", ArgvNaut::OptionType::STRING, "lit");
	parser.addOption("file", "f", ArgvNaut::OptionType::STRING, "");
	parser.addOption("samples", "p", ArgvNaut::OptionType::INTEGER, "4");
	parser.addOption("bounces", "b", ArgvNaut::OptionType::INTEGER, "3");

	if (!parser.parse(argc, const_cast<const char**>(argv)))
	{
		std::cerr << "Error: invalid arguments. Use --help for usage information." << std::endl;
		return 1;
	}

	if (parser.getFlag("help"))
	{
		std::cout << "\n*** Basic Renderer ***\n\nThe following options are available:" << std::endl << std::endl
		<< "-o, --output    output image name and path" << std::endl
		<< "-f, --file      input scene file name and path" << std::endl
		<< "-w, --width     output image width" << std::endl
		<< "-e, --height    output image height" << std::endl
		<< "-r, --rendering rendering mode [rasterizer, raytracer]" << std::endl
		<< "-s, --shading   shading mode [lit, normal]" << std::endl
		<< "-p, --samples   pixel samples" << std::endl
		<< "-b, --bounces   max bounces" << std::endl
		<< std::endl;
		return 0;
	}

	int width = parser.getInt("width").value_or(1280);
	int height = parser.getInt("height").value_or(720);
	int pixelSamples = parser.getInt("samples").value_or(4);
	int maxBounces = parser.getInt("bounces").value_or(3);

	std::string sceneFile = parser.getString("file").value_or("");
	if (sceneFile.empty())
		sceneFile = GetAssetPath("scenes/cornell_box.json");

	std::string outputPath = parser.getString("output").value_or("");
	if (outputPath.empty())
		outputPath = GetProjectRootDirectory() + "/output/";

	std::string renderingModeName = parser.getString("rendering").value_or("raytracer");
	std::string shadingModeName = parser.getString("shading").value_or("lit");

	Renderer::RenderingMode renderingMode = Renderer::RenderingMode::RAYTRACER;
	Renderer::ShadingMode shadingMode = Renderer::ShadingMode::LIT;

	if (renderingModeName == "rasterizer")
		renderingMode = Renderer::RenderingMode::RASTERIZER;

	if (shadingModeName == "normal")
		shadingMode = Renderer::ShadingMode::NORMAL;

	const std::string fileName = Renderer::GenerateFilename("cmd", height, renderingMode, shadingMode, pixelSamples, maxBounces);

	std::cout << "\n----- PARAMETERS -----" << std::endl;
	std::cout << "Output: " + outputPath + fileName + ".bmp" << std::endl;
	std::cout << "Input: " + sceneFile << std::endl;
	std::cout << "Width: " << width << std::endl;
	std::cout << "Height: " << height << std::endl;
	std::cout << "Rendering: " + renderingModeName << std::endl;
	std::cout << "Shading: " + shadingModeName << std::endl;
	std::cout << "Pixel Samples: " << pixelSamples << std::endl;
	std::cout << "Max Bounces: " << maxBounces << std::endl;

	std::cout << "\n----- RENDERING -----"<<std::endl;

	
	Renderer renderer;
	std::unique_ptr<World> scene = SceneLoader::LoadFromFile(sceneFile);
	if (!scene)
	{
		std::cerr << "Failed to load scene from file: " << sceneFile << std::endl;
		return 1;
	}
	std::unique_ptr<Model> model = std::make_unique<Model>(std::move(scene));

	model->SetMainCameraAspectRatio(static_cast<float>(width), static_cast<float>(height));
	model->Update(model->UpdateTime());
	std::unique_ptr<RenderState> renderState(model->ProcessForRendering());

	Raytracer& raytracer = renderer.GetRaytracer();
	raytracer.m_pixelSamples = pixelSamples;
	raytracer.m_maxBounces = maxBounces;
	
	const FrameBuffer* frame = renderer.Render(*renderState, width, height, renderingMode, shadingMode);

	//Saving Image
	ImageExporter::ExportToBMP(outputPath, fileName, *frame);
}

#endif
