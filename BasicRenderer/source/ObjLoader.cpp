#include "ObjLoader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
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
			outMatches.emplace_back(match);
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

		std::vector<std::cmatch> floats;
		floats.reserve(3);

		std::vector<std::cmatch> indices;
		indices.reserve(9);

		while (std::getline(file, line))
		{
			std::string_view sub = std::string_view(line).substr(0, 2);

			if (sub == "v ")  // Vertices
			{
				regex_search_all_matches(line.c_str(), floats, floatRgx);

				if (floats.size() == 3)
				{
					const float x = MatchToFloat(floats[0]);
					const float y = MatchToFloat(floats[1]);
					const float z = MatchToFloat(floats[2]);

					rawVertices.emplace_back(x, y, z, 1.f);
				}

				floats.clear();
			}
			else if (sub == "vt ") // UV
			{
				regex_search_all_matches(line.c_str(), floats, floatRgx);

				if (floats.size() == 2)
				{
					const float u = MatchToFloat(floats[0]);
					const float v = MatchToFloat(floats[1]);

					rawUVs.emplace_back(u, v);
				}

				floats.clear();
			}
			else if (sub == "vn ")  // Normals
			{
				regex_search_all_matches(line.c_str(), floats, floatRgx);

				if (floats.size() == 3)
				{
					const float x = MatchToFloat(floats[0]);
					const float y = MatchToFloat(floats[1]);
					const float z = MatchToFloat(floats[2]);

					rawNormals.emplace_back(x, y, z, 0.f);
				}	

				floats.clear();
			}
			else if (sub == "f ") // Faces
			{
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

				indices.clear();
			}

		}


		if (rawVertices.size() == rawNormals.size() && rawVertices.size() == rawUVs.size() && rawVertices.size() > 0)
		{
			for (size_t i = 0; i < rawVertices.size(); ++i)
			{
				vertices.push_back(Vertex(rawVertices[i], rawNormals[i], rawUVs[i]));
			}
		}
		else if (rawVertices.size() > 3 && vIndices.size() > 1)
		{
			for (size_t i = 0; i < rawVertices.size(); ++i)
			{
				vertices.push_back(Vertex(rawVertices[i], Vector4::Zero(), Vector2::Zero()));
			}
		}
		else {
			std::cout << "Invalid .obj format: " << path_name_extension << std::endl;
			return nullptr;
		}

		for (size_t i = 0; i < vIndices.size(); i++)
		{
			Face f(vertices[vIndices[i].i0], vertices[vIndices[i].i1], vertices[vIndices[i].i2]);
			faces.push_back(f);
		}

		std::cout << "Loaded .obj file: " << path_name_extension << std::endl;

		file.close();

		return new Mesh((int)vertices.size(), vertices.data(), (int)faces.size(), faces.data()); ;
	}
}