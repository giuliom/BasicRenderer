#include "ObjLoader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <stdio.h>
#include <regex>
#include "Vector3.h"
#include "Vector2.h"
#include "PrimitiveTypes.h"

namespace BasicRenderer
{
	float MatchToFloat(const std::cmatch& m)
	{
		return std::stof(m.str());
	}

	int MatchToInt(const std::cmatch& m)
	{
		return std::stoi(m.str());
	}

	bool regex_search_all_matches(const char* s, std::vector<std::cmatch>& outMatches, const std::regex& rgx)
	{
		std::cmatch match;
		while (std::regex_search(s, match, rgx))
		{
			outMatches.push_back(match);
			s = match.suffix().first;
		}

		return outMatches.size() > 0;
	}


	/** TODO expand to support textures and materials */
	Mesh* ObjLoader::Load(const char* path_name_extension)
	{
		assert(path_name_extension != nullptr);

		std::ifstream file(path_name_extension);

		if (!file.is_open())
		{
			std::cout << "Impossible to open: " << path_name_extension << std::endl;
			return nullptr;
		}

		std::vector<Vector4> rawVertices;
		std::vector<Vector4> rawNormals;
		std::vector<Vector2> rawUVs;

		std::vector<FaceIndices> vIndices;
		std::vector<FaceIndices> nIndices;
		std::vector<FaceIndices> tIndices;

		std::vector<Vertex> vertices;
		std::vector<Face> faces;

		std::string line;

		std::regex intRgx(R"(\d+)");
		std::regex floatRgx(R"([+-]?(?=[.]?[0-9])[0-9]*(?:[.][0-9]*)?(?:[Ee][+-]?[0-9]+)?)");

		std::regex vRgx(R"(^v )");
		std::regex vtRgx(R"(^vt )");
		std::regex vnRgx(R"(^vn )");
		std::regex fRgx(R"(^f )");

		while (std::getline(file, line))
		{
			if (std::regex_search(line, vRgx))  // Vertices
			{
				std::vector<std::cmatch> floats;
				regex_search_all_matches(line.c_str(), floats, floatRgx);

				if (floats.size() == 3)
				{
					const float x = MatchToFloat(floats[0]);
					const float y = MatchToFloat(floats[1]);
					const float z = MatchToFloat(floats[2]);

					rawVertices.emplace_back(x, y, z, 1.f);
				}
			}
			else if (std::regex_search(line, vtRgx)) // UV
			{
				std::vector<std::cmatch> floats;
				regex_search_all_matches(line.c_str(), floats, floatRgx);

				if (floats.size() == 2)
				{
					const float u = MatchToFloat(floats[0]);
					const float v = MatchToFloat(floats[1]);

					rawUVs.emplace_back(u, v);
				}				
			}
			else if (std::regex_search(line, vnRgx))  // Normals
			{
				std::vector<std::cmatch> floats;
				regex_search_all_matches(line.c_str(), floats, floatRgx);

				if (floats.size() == 3)
				{
					const float x = MatchToFloat(floats[0]);
					const float y = MatchToFloat(floats[1]);
					const float z = MatchToFloat(floats[2]);

					rawNormals.emplace_back(x, y, z, 0.f);
				}	
			}
			else if (std::regex_search(line, fRgx)) // Faces
			{
				std::vector<std::cmatch> indices;
				regex_search_all_matches(line.c_str(), indices, intRgx);

				if(indices.size() >= 3) // .obj indices start from 1
				{
					vIndices.emplace_back(FaceIndices(MatchToInt(indices[0]) - 1, MatchToInt(indices[1]) - 1, MatchToInt(indices[2]) - 1));
				}
				if (indices.size() >= 9)
				{
					nIndices.emplace_back(FaceIndices(MatchToInt(indices[3]) - 1, MatchToInt(indices[4]) - 1, MatchToInt(indices[5]) - 1));
					tIndices.emplace_back(FaceIndices(MatchToInt(indices[6]) - 1, MatchToInt(indices[7]) - 1, MatchToInt(indices[8]) - 1));
				}
			}

		}


		if (rawVertices.size() == rawNormals.size() && rawVertices.size() == rawUVs.size() && rawVertices.size() > 0)
		{
			for (int i = 0; i < rawVertices.size(); ++i)
			{
				vertices.push_back(Vertex(rawVertices[i], rawNormals[i], rawUVs[i]));
			}
		}
		else if (rawVertices.size() > 3 && vIndices.size() > 1)
		{
			for (int i = 0; i < rawVertices.size(); ++i)
			{
				vertices.push_back(Vertex(rawVertices[i], Vector4::Zero(), Vector2::Zero()));
			}
		}
		else {
			std::cout << "Invalid .obj format: " << path_name_extension << std::endl;
			return nullptr;
		}

		for (int i = 0; i < vIndices.size(); i++)
		{
			Face f(vertices[vIndices[i].i0], vertices[vIndices[i].i1], vertices[vIndices[i].i2]);
			faces.push_back(f);
		}

		std::cout << "Loaded .obj file: " << path_name_extension << std::endl;

		file.close();

		return new Mesh((int)vertices.size(), vertices.data(), (int)faces.size(), faces.data()); ;
	}
}