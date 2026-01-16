#include "PathUtils.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <limits.h>
#include <unistd.h>
#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif
#endif

namespace BasicRenderer
{
	std::string GetExecutableDirectory()
	{
		std::string execPath;

#ifdef _WIN32
		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		execPath = buffer;
		// Find last backslash
		size_t pos = execPath.find_last_of("\\/");
		if (pos != std::string::npos)
		{
			execPath = execPath.substr(0, pos);
		}
#elif __APPLE__
		char buffer[PATH_MAX];
		uint32_t size = sizeof(buffer);
		if (_NSGetExecutablePath(buffer, &size) == 0)
		{
			// Resolve any symlinks
			char realPath[PATH_MAX];
			if (realpath(buffer, realPath) != nullptr)
			{
				execPath = realPath;
			}
			else
			{
				execPath = buffer;
			}
			// Find last slash
			size_t pos = execPath.find_last_of('/');
			if (pos != std::string::npos)
			{
				execPath = execPath.substr(0, pos);
			}
		}
#else  // Linux
		char buffer[PATH_MAX];
		ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
		if (len != -1)
		{
			buffer[len] = '\0';
			execPath = buffer;
			// Find last slash
			size_t pos = execPath.find_last_of('/');
			if (pos != std::string::npos)
			{
				execPath = execPath.substr(0, pos);
			}
		}
#endif
		return execPath;
	}

	std::string GetProjectRootDirectory()
	{
        // It's assumed to be in ${PROJECT_ROOT}/build/bin
		std::string execDir = GetExecutableDirectory();
		
#ifdef _WIN32
		const char separator = '\\';
#else
		const char separator = '/';
#endif

		size_t pos = execDir.find_last_of("/\\");
		if (pos != std::string::npos)
		{
			execDir = execDir.substr(0, pos);
		}

		pos = execDir.find_last_of("/\\");
		if (pos != std::string::npos)
		{
			execDir = execDir.substr(0, pos);
		}

		return execDir;
	}

	std::string GetAssetPath(const std::string& relativePath)
	{
        // It's assumed to be in ${PROJECT_ROOT}/assets
		std::string root = GetProjectRootDirectory();
		
#ifdef _WIN32
		return root + "\\assets\\" + relativePath;
#else
		return root + "/assets/" + relativePath;
#endif
	}
}
