#include "RenderState.h"

namespace BasicRenderer
{
	RenderState::RenderState(const Camera& camera, const EnvironmentSettings& environmentSettings, std::vector<Primitive*>& primitives)
		: m_camera(camera)
		, m_environmentSettings(environmentSettings)
		, m_creationTime(TimeClock::now())
		, m_bvh()
	{
		m_primitives.resize(primitives.size());

		for (size_t i = 0u; i < m_primitives.size(); ++i)
		{
			m_primitives[i].reset(primitives[i]);
		}
	}

	void RenderState::BuildAccelerationStructure() const
	{
		m_bvh.Build(m_primitives);
	}
}