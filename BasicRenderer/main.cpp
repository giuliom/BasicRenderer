#include "BasicRenderer.h"
#include "ObjLoader.h"
#include <iostream>
#include <ctime>
#include <string>
#include "SceneObject.h"

#if !LIB_DEBUG && !LIB_RELEASE

int main(int argc, char *argv[])
{

	const char* file = "C:/BasicRenderer/Assets/bunny.obj"; //TODO "../../Assets/bunny.obj";

	if (argc > 1) file = argv[1];

	int width = 1920;
	int height = 1080;

	BasicRenderer renderer;
	SceneObject bunny(ObjLoader::Load(file));

	auto beginClock = clock();
	
	renderer.Render(width, height, bunny);
	
	auto endClock = clock();

	std::string rt = std::to_string(endClock - beginClock);
	std::cout << "Rendering Time: " <<rt.substr(0, rt.length() - 6).append("ms ").c_str() << "\n";


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