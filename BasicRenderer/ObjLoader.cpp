#include "ObjLoader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <stdio.h>
#include "Vector3.h"
#include "Vector2.h"
#include "PrimitiveTypes.h"

/** TODO expand to support textures and materials */
Mesh* ObjLoader::Load(const char* path_name_extension)
{
	assert(path_name_extension != nullptr);

	std::ifstream file(path_name_extension);

	if (!file.is_open())
	{
		std::cout<<"Impossible to open: " << path_name_extension << std::endl;
		return nullptr;
	}

	std::vector<Vector4> vertices;
	std::vector<Vector4> normals;
	std::vector<Vector2> uvs;

	std::vector<FaceIndices> vIndices;
	std::vector<FaceIndices> nIndices;
	std::vector<FaceIndices> tIndices;

	std::vector<Face> faces;

	std::string line;

	while (std::getline(file, line))
	{
		std::string sub = line.substr(0, 2);

		if (sub.compare("v ") == 0)  // Vertices
		{
			Vector4 vertex = Vector4::Zero();
			vertex.w = 1.0f;
			sscanf_s(line.c_str(), "v %f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vertices.push_back(vertex);
		}
		else if (sub.compare("vt") == 0) // UV
		{
			Vector2 uv;
			sscanf_s(line.c_str(), "vt %f %f\n", &uv.x, &uv.y);
			uvs.push_back(uv);
		}
		else if (sub.compare("vn") == 0)  // Normals
		{
			Vector4 normal = Vector4::Zero();
			sscanf_s(line.c_str(), "vn %f %f %f\n", &normal.x, &normal.y, &normal.z);
			normals.push_back(normal);
		}
		else if (sub.compare("f ") == 0) // Faces
		{
			unsigned int vIndex[3], nIndex[3], tIndex[3];
			int matches = 0;
								matches = sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d\n", &vIndex[0], &vIndex[1], &vIndex[2], &nIndex[0], &nIndex[1], &nIndex[2], &tIndex[0], &tIndex[1], &tIndex[2]);
			if (matches < 3)	matches = sscanf_s(line.c_str(), "f %d %d %d %d %d %d %d %d %d\n", &vIndex[0], &vIndex[1], &vIndex[2], &nIndex[0], &nIndex[1], &nIndex[2], &tIndex[0], &tIndex[1], &tIndex[2]);

			if (matches >= 3) // .obj indices start from 1
			{
				vIndices.push_back(FaceIndices(vIndex[0]-1, vIndex[1]-1, vIndex[2]-1));
			}
			if (matches >= 9)
			{
				nIndices.push_back(FaceIndices(nIndex[0]-1, nIndex[1]-1, nIndex[2]-1));
				tIndices.push_back(FaceIndices(tIndex[0]-1, tIndex[1]-1, tIndex[2]-1));
			}
		}

	}

	
	if (vIndices.size() == nIndices.size() && vIndices.size() == tIndices.size() && vIndices.size() > 0)
	{
		for (int i = 0; i < vIndices.size(); ++i)
		{
			Vertex v0 = Vertex(vertices[vIndices[i].i0], normals[nIndices[i].i0], uvs[tIndices[i].i0]);
			Vertex v1 = Vertex(vertices[vIndices[i].i1], normals[nIndices[i].i1], uvs[tIndices[i].i1]);
			Vertex v2 = Vertex(vertices[vIndices[i].i2], normals[nIndices[i].i2], uvs[tIndices[i].i2]);
			
			faces.push_back(Face(v0, v1, v2));
		}
	}
	else if (vertices.size() > 3 && vIndices.size() > 1)
	{
		for (int i = 0; i < vIndices.size(); ++i)
		{
			Vertex v0 = Vertex(vertices[vIndices[i].i0], Vector4::Zero(), Vector2::Zero());
			Vertex v1 = Vertex(vertices[vIndices[i].i1], Vector4::Zero(), Vector2::Zero());
			Vertex v2 = Vertex(vertices[vIndices[i].i2], Vector4::Zero(), Vector2::Zero());

			faces.push_back(Face(v0, v1, v2));
		}
	}
	else {
		std::cout << "Invalid .obj format: " << path_name_extension << std::endl;
		return nullptr;
	}

	std::cout << "Loaded .obj file: " << path_name_extension << std::endl;

	file.close();

	return new Mesh((int) faces.size(), faces.data()); ;
}
