#include "BasicRenderer.h"
#include "World.h"
#include "Ray.h"
#include "Material.h"

namespace BasicRenderer
{
	Renderer::Renderer() : m_rasterizer(), m_raytracer()
	{
		m_renderSystems.emplace_back(&m_rasterizer);
		m_renderSystems.emplace_back(&m_raytracer);
	}

	const std::shared_ptr<const FrameBuffer> Renderer::Render(uint width, uint height, World& scene, RenderingMode mode, ShadingMode shading)
	{
		assert(width > 0 && height > 0);

		if (width != m_width || height != m_height || m_fBuffer == nullptr)
		{
			m_fBuffer = std::make_shared<FrameBuffer>(width, height);

			m_width = width;
			m_height = height;

			scene.GetMainCamera().SetAspectRatio(width, height);
		}

		m_fBuffer->Fill(scene.ambientLightColor * scene.ambientLightIntensity);

		//Should be done in update()
		scene.ProcessForRendering();

		auto shadingFunc = &Material::LitShading;

		switch (shading)
		{
		case ShadingMode::NORMAL:
			shadingFunc = &Material::NormalShading;
			break;
		}


		// TODO rasterizer and raytracer should have the same parent and sue the same function
		switch (mode)
		{
		default:
		{
			m_rasterizer.Render(*m_fBuffer, scene, shadingFunc);
		}
		break;
		case RenderingMode::RAYTRACER:
		{
			m_raytracer.Render(*m_fBuffer, scene, shadingFunc);
		}
		break;
		}

		// Post Processing
		m_fBuffer->CorrectGamma();
		// TODO tonemapping

		return m_fBuffer;
	}

	

	

}