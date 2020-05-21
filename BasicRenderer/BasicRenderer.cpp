#include "BasicRenderer.h"
#include "World.h"
#include "Ray.h"
#include "Material.h"

namespace BasicRenderer
{
	const std::shared_ptr<const FrameBuffer> Renderer::Render(uint width, uint height, World& scene, RenderingMode mode, ShadingMode shading, uint samplesPerPixel, uint maxBounces)
	{
		assert(width > 0 && height > 0);

		if (width != m_width || height != m_height || m_fBuffer == nullptr)
		{
			m_fBuffer = std::make_shared<FrameBuffer>(width, height);

			m_width = width;
			m_height = height;

			m_camera.SetAspectRatio(width, height);
		}

		m_fBuffer->Fill(scene.ambientLightColor);

		//Should be done in update()
		scene.ProcessForRendering();

		auto shadingFunc = &Material::LitShading;

		switch (shading)
		{
		case ShadingMode::NORMAL:
			maxBounces = 1;
			shadingFunc = &Material::NormalShading;
			break;
		}


		// TODO rasterizer and raytracer should have the same parent and sue the same function
		switch (mode)
		{
		default:
		{
			m_rasterizer.Rasterize(*m_fBuffer, scene, m_camera, shadingFunc);
		}
		break;
		case RenderingMode::RAYTRACER:
		{

			m_raytracer.RayTracing(*m_fBuffer, scene, m_camera, samplesPerPixel, maxBounces, shadingFunc);
		}
		break;
		}

		// Post Processing
		m_fBuffer->CorrectGamma();

		return m_fBuffer;
	}

	

	

}