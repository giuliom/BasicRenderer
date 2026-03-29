#include "RenderState.h"

namespace BasicRenderer
{
	RenderState::RenderState(const Camera& camera, const EnvironmentSettings& environmentSettings, InstanceList instances)
		: m_camera(camera)
		, m_environmentSettings(environmentSettings)
		, m_instances(std::move(instances))
		, m_creationTime(TimeClock::now())
		, m_bvh()
	{
	}

	void RenderState::BuildAccelerationStructure() const
	{
		m_bvh.Build(m_instances);
	}
}