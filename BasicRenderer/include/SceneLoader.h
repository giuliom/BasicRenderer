#pragma once

#include <memory>
#include <string>

namespace BasicRenderer
{
	class World;

	class SceneLoader
	{
	public:
		// TODO: proper implementation with scene structure support
		static std::unique_ptr<World> LoadFromFile(const std::string& filepath);
	};
}
