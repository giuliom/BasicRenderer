#include "RenderState.h"

namespace BasicRenderer
{
	RenderState::RenderState(const Camera& camera, const EnvironmentSettings& environmentSettings, DrawableInstanceList instances, FaceBuffer faceBuffer)
		: m_camera(camera)
		, m_environmentSettings(environmentSettings)
		, m_instances(std::move(instances))
		, m_faceBuffer(std::move(faceBuffer))
		, m_creationTime(TimeClock::now())
		, m_bvh()
	{
	}

	void RenderState::BuildAccelerationStructure() const
	{
		m_bvh.Build(m_instances, m_faceBuffer);
	}
}