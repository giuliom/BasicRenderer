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
		RenderState(const Camera& camera, const EnvironmentSettings& environmentSettings, InstanceList instances);
		~RenderState() {}

		RenderState(const RenderState&) = delete;
		RenderState& operator=(const RenderState&) = delete;
		RenderState(RenderState&&) = default;
		RenderState& operator=(RenderState&&) = delete;

		const AccelerationStructure& GetAccelerationStructure()	const noexcept { return m_bvh; }
		void BuildAccelerationStructure() const;
		const TimePoint& CreationTime() const noexcept { return m_creationTime; }

	private:
		TimePoint m_creationTime;
		mutable AccelerationStructure m_bvh;

	};
}