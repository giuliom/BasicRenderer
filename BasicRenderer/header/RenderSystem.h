#pragma once

#include "Global.h"
#include <functional>
#include "FrameBuffer.h"
#include "SceneObject.h"
#include "World.h"

namespace BasicRenderer
{
	typedef std::function<Color(const Material&, const World& w, const Vector3& pos, const Vector3& nrml)> ShadingFunc;

	class RenderSystem
	{
		virtual void Render(FrameBuffer& fBuffer, const World& scene, const ShadingFunc& Shading) = 0;
	};
}