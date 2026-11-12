#include "RenderSystem.h"
#include <algorithm>
#include <thread>

namespace BasicRenderer
{
	uint RenderSystem::MaxThreadCount() noexcept
	{
		return std::max(1u, std::thread::hardware_concurrency());
	}

	uint RenderSystem::DefaultThreadCount() noexcept
	{
		return std::max(1u, MaxThreadCount() - 1u);
	}

	void RenderSystem::SetThreadCount(uint count) noexcept
	{
		m_threadCount = std::clamp(count, 1u, MaxThreadCount());
	}
}