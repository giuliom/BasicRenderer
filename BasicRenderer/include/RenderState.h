#pragma once

#include "World.h"
#include "AccelerationStructure.h"

namespace BasicRenderer
{
	struct RenderState
	{
		Camera m_camera;
		EnvironmentSettings m_environmentSettings;
		InstanceList m_instances;

		RenderState()
			: m_camera(), m_environmentSettings(), m_instances(), m_creationTime(), m_bvh() {}
		RenderState(const Camera& camera, const EnvironmentSettings& environmentSettings, const InstanceList& instances);
		~RenderState() {}

		const AccelerationStructure& GetAccelerationStructure()	const { return m_bvh; }
		void BuildAccelerationStructure() const;
		const TimePoint& CreationTime() const { return m_creationTime; }

	private:
		TimePoint m_creationTime;
		mutable AccelerationStructure m_bvh;

	};
}