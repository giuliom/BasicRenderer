#pragma once

#include <string>

namespace BasicRenderer
{
	std::string GetExecutableDirectory();
	std::string GetProjectRootDirectory();
	std::string GetAssetPath(const std::string& relativePath);
}
