#include "RenderState.h"

namespace BasicRenderer
{
	RenderState::RenderState(const Camera& camera, const EnvironmentSettings& environmentSettings, const InstanceList& instances)
		: m_camera(camera)
		, m_environmentSettings(environmentSettings)
		, m_instances(instances)
		, m_creationTime(TimeClock::now())
		, m_bvh()
	{
	}

	void RenderState::BuildAccelerationStructure() const
	{
		m_bvh.Build(m_instances);
	}
}