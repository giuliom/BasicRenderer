#pragma once

#include <memory>
#include <vector>
#include "FrameBuffer.h"
#include "Rasterizer.h"
#include "Raytracer.h"
#include "SceneObject.h"
#include "Camera.h"
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

		Camera m_camera;

		Rasterizer m_rasterizer;
		Raytracer m_raytracer;

	public:

		Renderer() : m_rasterizer(), m_raytracer() {}
		~Renderer() {}

		inline Camera& GetCamera() { return m_camera; }

		const std::shared_ptr<const FrameBuffer> Render(uint width, uint height, World& scene, RenderingMode mode = RenderingMode::RASTERIZER, ShadingMode shading = ShadingMode::LIT, uint samplesPerPixel = 4, uint bounces = 3);

	};
}