#pragma once

#include "Global.h"
#include <functional>
#include "FrameBuffer.h"
#include "SceneObject.h"
#include "RenderState.h"

namespace BasicRenderer
{
	typedef std::function<Color(const Material&, const RenderState& rs, const Vector3& pos, const Vector3& nrml)> ShadingFunc;

	class RenderSystem
	{
		virtual void Render(FrameBuffer& fBuffer, const RenderState& state, const ShadingFunc& Shading) = 0;
	};
}