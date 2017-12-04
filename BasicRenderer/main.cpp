#include "BasicRenderer.h"
#include "ObjLoader.h"
#include <iostream>
#include <ctime>
#include <string>
#include "SceneObject.h"

#if !LIB_DEBUG && !LIB_RELEASE

int main(int argc, char *argv[])
{
	std::string file = "C:/BasicRenderer/Assets/bunny.obj"; //TODO "../../Assets/bunny.obj";

	int width = 1920;
	int height = 1080;

	std::string outputFile = "./brender.bmp";

	std::string renderingMode = "TODOr";
	std::string shadingMode = "TODOs";


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
			renderingMode = argv[an + 1];
		}
		else if (std::strcmp(argv[an], "-s") == 0)
		{
			shadingMode = argv[an + 1];
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

	std::cout << "Output: " + outputFile<< std::endl;
	std::cout << "Input: " + file<< std::endl;
	std::cout << "Width: " << width<< std::endl;
	std::cout << "Height: " << height << std::endl;
	std::cout << "Rendering: " + renderingMode<< std::endl;
	std::cout << "Shading: " + shadingMode << std::endl;
	std::cout << "----- RENDERING -----"<<std::endl;

	
	BasicRenderer renderer;
	SceneObject bunny(ObjLoader::Load(file.c_str()));

	auto beginClock = clock();
	
	renderer.Render(width, height, bunny);
	
	auto endClock = clock();

	std::string rt = std::to_string(endClock - beginClock);
	std::cout << "Rendering Time: " <<rt.substr(0, rt.length() - 7).append("ms ").c_str() << "\n";


	int i = 0;
	while (i < 10)
	{
		beginClock = clock();

		renderer.Render(width, height, bunny);

		endClock = clock();

		rt = std::to_string(endClock - beginClock);
		std::cout << "Rendering Time: " << rt.substr(0, rt.length() - 6).append("ms ").c_str() << "\n";
		++i;
	}
	
	getchar();
	
}

#endif