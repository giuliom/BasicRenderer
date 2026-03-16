#include "PathUtils.h"

#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#else
#include <limits.h>
#include <unistd.h>
#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif
#endif

namespace fs = std::filesystem;

namespace BasicRenderer
{
	std::string GetExecutableDirectory()
	{
		std::string execPath;

#ifdef _WIN32
		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		execPath = buffer;
#elif __APPLE__
		char buffer[PATH_MAX];
		uint32_t size = sizeof(buffer);
		if (_NSGetExecutablePath(buffer, &size) == 0)
		{
			char realPath[PATH_MAX];
			if (realpath(buffer, realPath) != nullptr)
			{
				execPath = realPath;
			}
			else
			{
				execPath = buffer;
			}
		}
#else  // Linux
		char buffer[PATH_MAX];
		ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
		if (len != -1)
		{
			buffer[len] = '\0';
			execPath = buffer;
		}
#endif
		return fs::path(execPath).parent_path().string();
	}

	std::string GetProjectRootDirectory()
	{
		// It's assumed to be in ${PROJECT_ROOT}/build/bin
		fs::path execDir = GetExecutableDirectory();
		return execDir.parent_path().parent_path().string();
	}

	std::string GetAssetPath(const std::string& relativePath)
	{
		// It's assumed to be in ${PROJECT_ROOT}/assets
		fs::path assetPath = fs::path(GetProjectRootDirectory()) / "assets" / relativePath;
		return assetPath.string();
	}
}
