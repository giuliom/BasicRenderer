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
	/*
	Matrix4 m1, m2, result;

	m1.x1 = 1;
	m1.y2 = 2;
	m1.z3 = 3;
	m1.w4 = 4;
	m1.x2 = 22;
	m1.x3 = 33;
	m1.x4 = 44;
	m1.y1 = -22;
	m1.z1 = -33;
	m1.w1 = -44;

	m2.x1 = 5;
	m2.y2 = 6;
	m2.z3 = 7;
	m2.w4 = 8;
	m2.x2 = 55;
	m2.x3 = 66;
	m2.x4 = 77;
	m2.y1 = -55;
	m2.z1 = -66;
	m2.w1 = -77;

	result = m1 * m2;
	*/
	getchar();
	
}

#endif