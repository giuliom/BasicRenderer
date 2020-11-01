#pragma once

#include <memory>
#include <vector>
#include "FrameBuffer.h"
#include "Rasterizer.h"
#include "Raytracer.h"
#include "InputManager.h"
#include "SceneObject.h"
#include "World.h"

namespace BasicRenderer
{
	class Ray;
	class Hitable;

	class Renderer
	{
	public:
		enum class RenderingMode
		{
			RASTERIZER,
			RAYTRACER
		};

		enum class ShadingMode
		{
			LIT,
			NORMAL
		};

	protected:
		uint m_width = 640;
		uint m_height = 480;
		std::shared_ptr<FrameBuffer> m_fBuffer;

		Rasterizer m_rasterizer;
		Raytracer m_raytracer;
		std::vector<RenderSystem*> m_renderSystems;

		InputManager m_inputMgr;

	public:

		Renderer();
		~Renderer() {}

		Rasterizer& GetRasterizer() { return m_rasterizer; }
		Raytracer& GetRaytracer() { return m_raytracer; }
		InputManager& GetInputManager() { return m_inputMgr; }

		const FrameBuffer* Render(uint width, uint height, World& scene, RenderingMode mode = RenderingMode::RASTERIZER, ShadingMode shading = ShadingMode::LIT, const float deltaTime = 0.f);

		void ProcessInput(InputManager& inputMgr, World& scene, const float deltaTime);
		void ProcessButtonInput(const ButtonInputEvent& input, World& scene, const float deltaTime);
		void ProcessCursorInput(const InputManager& inputMgr, const CursorInputEvent& input, World& scene, const float deltaTime);
	};
}