#include "RenderSystem.h"
#include <algorithm>
#include <thread>

namespace BasicRenderer
{
	uint RenderSystem::MaxThreadCount() noexcept
	{
		const uint hardwareThreads = std::thread::hardware_concurrency();
		return hardwareThreads > 1u ? hardwareThreads - 1u : 1u;
	}

	void RenderSystem::SetThreadCount(uint count) noexcept
	{
		m_threadCount = std::clamp(count, 1u, MaxThreadCount());
	}
}