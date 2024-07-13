#pragma once

#include <memory>
#include <vector>
#include "Global.h"
#include "FrameBuffer.h"
#include "Rasterizer.h"
#include "Raytracer.h"
#include "SceneObject.h"
#include "Model.h"
#include "RenderState.h"

namespace BasicRenderer
{
	class Ray;
	class Hitable;

	class Renderer
	{
	public:
		enum class RenderingMode : uint
		{
			RASTERIZER,
			RAYTRACER
		};

		static std::vector<std::string> renderingModeStrings;

		enum class ShadingMode : uint
		{
			LIT,
			NORMAL
		};

		static std::vector<std::string> shadingModeStrings;

	protected:
		uint m_width = 640;
		uint m_height = 480;
		std::shared_ptr<FrameBuffer> m_fBuffer;

		Rasterizer m_rasterizer;
		Raytracer m_raytracer;
		std::vector<RenderSystem*> m_renderSystems;

	public:

		Renderer();
		virtual ~Renderer() {}

		Rasterizer& GetRasterizer() { return m_rasterizer; }
		Raytracer& GetRaytracer() { return m_raytracer; }

		const FrameBuffer* Render(const RenderState& scene, uint width, uint height, RenderingMode mode = RenderingMode::RASTERIZER, ShadingMode shading = ShadingMode::LIT, const float deltaTime = 0.f);

		static std::string GenerateFilename(const std::string& customPrefix, uint imageHeight, RenderingMode mode, ShadingMode shading, uint pixelSamples = 1u, uint bounces = 0u);
	};
}