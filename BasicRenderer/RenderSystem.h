#pragma once

#include "Global.h"
#include "FrameBuffer.h"
#include "SceneObject.h"
#include "World.h"

namespace BasicRenderer
{
	class RenderSystem
	{
		virtual void Render(FrameBuffer& fBuffer, const World& scene, Color(Material::* shading)(const World& w, const Vector3& pos, const Vector3& nrml)) = 0;
	};
}