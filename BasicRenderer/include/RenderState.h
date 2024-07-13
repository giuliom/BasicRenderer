#pragma once

#include "World.h"
#include "AccelerationStructure.h"

namespace BasicRenderer
{
	struct RenderState
	{
		Camera m_camera;
		EnvironmentSettings m_environmentSettings;
		PrimitiveList m_primitives;

		RenderState()
			: m_camera(), m_environmentSettings(), m_primitives(), m_creationTime(), m_bvh() {}
		RenderState(const Camera& camera, const EnvironmentSettings& environmentSettings, std::vector<Primitive*>& primitives);
		~RenderState() {}

		const AccelerationStructure& GetAccelerationStructure()	const { return m_bvh; }
		void BuildAccelerationStructure() const;
		const TimePoint& CreationTime() const { return m_creationTime; }

	private:
		TimePoint m_creationTime;
		mutable AccelerationStructure m_bvh;

	};
}