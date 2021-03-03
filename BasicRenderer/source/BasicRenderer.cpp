#include "BasicRenderer.h"
#include "World.h"
#include "Ray.h"
#include "Material.h"

namespace BasicRenderer
{
	std::vector<std::string> Renderer::renderingModeStrings =
	{
		"Rasterizer",
		"Raytracer"
	};

	std::vector<std::string> Renderer::shadingModeStrings =
	{
		"Lit",
		"Normal"
	};

	Renderer::Renderer() : m_rasterizer(), m_raytracer()
	{
		m_renderSystems.emplace_back(&m_rasterizer);
		m_renderSystems.emplace_back(&m_raytracer);
	}

	const FrameBuffer* Renderer::Render(const RenderState& state, uint width, uint height, RenderingMode mode, ShadingMode shading, const float deltaTime)
	{
		assert(width > 0 && height > 0);

		if (width != m_width || height != m_height || m_fBuffer == nullptr)
		{
			m_fBuffer = std::make_shared<FrameBuffer>(width, height);

			m_width = width;
			m_height = height;
			
		}

		const EnvironmentSettings& environment = state.m_environmentSettings;
		m_fBuffer->Fill(environment.m_ambientLightColor * environment.m_ambientLightIntensity);

		auto shadingFunc = &Material::LitShading;

		switch (shading)
		{
		case ShadingMode::NORMAL:
			shadingFunc = &Material::NormalShading;
			break;
		}

		// TODO partial rebuilding of the bvh?
		state.BuildAccelerationStructure();

		switch (mode)
		{
			default:
			{
				m_rasterizer.Render(*m_fBuffer, state, shadingFunc);
			}
			break;
			case RenderingMode::RAYTRACER:
			{
				m_raytracer.Render(*m_fBuffer, state, shadingFunc);
			}
			break;
		}

		// Post Processing
		m_fBuffer->CorrectGamma();
		// TODO tonemapping

		return m_fBuffer.get();
	}

	std::string Renderer::GenerateFilename(const std::string& customPrefix, uint imageHeight, RenderingMode mode, ShadingMode shading, uint pixelSamples, uint bounces)
	{
		std::string rtInfo = mode == RenderingMode::RAYTRACER 
							? "_" + std::to_string(pixelSamples) + "ps_" + std::to_string(bounces) + "b"
							: "";

			return customPrefix + "_"
			+ renderingModeStrings[std::underlying_type_t<RenderingMode>(mode)] + "_"
			+ shadingModeStrings[std::underlying_type_t<RenderingMode>(shading)] + "_"
			+ std::to_string(imageHeight) + "p" + rtInfo;
	}

	

}