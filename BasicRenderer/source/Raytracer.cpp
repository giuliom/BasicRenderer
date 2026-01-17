#include <algorithm>
#include <cmath>
#include <iostream>
#include <thread>
#include "Raytracer.h"
#include "BasicRenderer.h"
#include "FrameBuffer.h"
#include "World.h"
#include "Ray.h"
#include "Material.h"



namespace BasicRenderer
{
	// Mutexes
	
	std::mutex Raytracer::m_progressMtx;


	void Raytracer::Render(FrameBuffer& fBuffer, const RenderState& state, const ShadingFunc& Shading)
	{
		m_fBuffer = &fBuffer;
		m_shadingFunc = Shading;
		m_progress = 0.f;
		m_pixelSamples = std::max(1u, m_pixelSamples);

		std::vector<std::thread> renderThreads;
		const uint threadCount = std::thread::hardware_concurrency() > 1u ? std::thread::hardware_concurrency() - 1u : 1u;
		const uint rowCount = fBuffer.GetHeight();
		const uint rowsPerThread = rowCount / threadCount;
		const uint remainderRows = rowCount % threadCount;

		std::cout << "\nRendering with " << threadCount << " threads\n";
		std::cout << "Progress: " << 0u << "% \r";

		uint currentRow = 0u;
		for (uint i = 0u; i < threadCount; i++)
		{
			const uint startRowIndex = currentRow;
			const uint extraRow = i < remainderRows ? 1u : 0u;
			const uint endRowIndex = startRowIndex + rowsPerThread + extraRow;
			currentRow = endRowIndex;

			auto thread = std::thread(&Raytracer::RenderJob, this, std::cref(state), startRowIndex, endRowIndex);
			renderThreads.push_back(std::move(thread));
		}

		for (auto& t : renderThreads)
		{
			t.join();
		}

		std::cout << "Progress: " << 100u << "% \r";
		std::cout << "\nRendering completed";
	}

	void Raytracer::RenderJob(const RenderState& state, const uint startRowIndex, const uint endRowIndex)
	{
		FrameBuffer& fBuffer = *m_fBuffer;

		const Camera& camera = state.m_camera;

		const uint width = fBuffer.GetWidth();
		const uint height = fBuffer.GetHeight();

		const float fwidth = static_cast<float>(width);
		const float fheight = static_cast<float>(height);

		const float inverseWidth = 1.0f / fwidth;
		const float inverseHeight = 1.0f / fheight;
		const float fInversePixelSamples = 1.0f / static_cast<float>(m_pixelSamples);

		const float pixelPctg =  100.f / (fheight * fwidth);

		std::vector<const BVHnode*> dfsStack;
		dfsStack.reserve(state.GetAccelerationStructure().LevelsCount());

		//Top-left, drawing rows
		for (uint y = startRowIndex; y < endRowIndex; y++)
		{
			for (uint x = 0u; x < width; x++)
			{
				Color c;
				for (uint i = 0u; i < m_pixelSamples; i++)
				{
					const float jitterX = UniformDist() - 0.5f;
					const float jitterY = UniformDist() - 0.5f;
					const float u = (static_cast<float>(x) + 0.5f + jitterX) * inverseWidth;
					const float v = (static_cast<float>(y) + 0.5f + jitterY) * inverseHeight;

					Ray r = camera.GetCameraRay(u, v);
					r.direction = r.direction.Normalize();
					c = c + RayTrace(r, state, dfsStack, m_shadingFunc);
				}

				c = c * fInversePixelSamples;
				c.r = std::min(c.r, 1.f);
				c.g = std::min(c.g, 1.f);
				c.b = std::min(c.b, 1.f);

				fBuffer.WriteToColor(y * width + x, c);

#if !LIB_DEBUG && !LIB_RELEASE
				std::scoped_lock<std::mutex> lock(m_progressMtx);
				m_progress = m_progress + pixelPctg;
				std::cout << "Progress: " << static_cast<uint>(std::roundf(m_progress)) << "% \r";
#endif
			}
		}
	}

	Color Raytracer::RayTrace(const Ray& ray, const RenderState& state, std::vector<const BVHnode*>& dfsStack, const ShadingFunc& Shading)
	{
		(void)Shading;
		Vector3 hitPosition, hitNormal;
		const Primitive* hitObject = nullptr;

		uint bounces = 0;
		bool success = false;
		Ray iterationRay = ray;
		Color throughput = { 1.f, 1.f, 1.f };
		Color resultRadiance = { 0.f, 0.f, 0.f };

		do
		{
			if ((hitObject = Raycast(state.GetAccelerationStructure(), iterationRay, 0.0001f, 999999.99f, dfsStack, hitPosition, hitNormal)) != nullptr)
			{
				const Material* material = hitObject->GetMaterial();

				success = false;
				
				if (material != nullptr)
				{
					const Material& mat = *material;

					switch (mat.type)
					{
					case Material::Type::METALLIC:
					{
						Vector3 reflected = Vector3::Reflect(iterationRay.direction, hitNormal);
						iterationRay = Ray(hitPosition, reflected + UniformSampleInHemisphere(hitNormal) * (1.f - mat.metallic));
						success = (Vector3::Dot(iterationRay.direction, hitNormal) > 0);
					}
					break;

					case Material::Type::DIELECTRIC:
					{
						Vector3 outNormal;
						Vector3 reflected = Vector3::Reflect(iterationRay.direction, hitNormal);
						float ni_nt;
						Vector3 refracted;
						float reflectionProb;
						float cos;
						if (Vector3::Dot(iterationRay.direction, hitNormal) > 0.f)
						{
							outNormal = hitNormal * -1.f;
							ni_nt = mat.refractiveIndex;
							cos = (Vector3::Dot(iterationRay.direction, hitNormal) * mat.refractiveIndex); // / rayIn.direction.Length(); == 1.f
						}
						else
						{
							outNormal = hitNormal;
							ni_nt = 1.f / mat.refractiveIndex;
							cos = -(Vector3::Dot(iterationRay.direction, hitNormal)); // / rayIn.direction.Length(); == 1.f
						}

						if (Material::Refract(iterationRay.direction, outNormal, ni_nt, refracted))
						{
							reflectionProb = Material::Schlick(cos, mat.refractiveIndex);
						}
						else
						{
							reflectionProb = 1.f;
						}

						if (UniformDist() < reflectionProb)
						{
							iterationRay = Ray(hitPosition, reflected);
						}
						else
						{
							iterationRay = Ray(hitPosition, refracted);
						}

						success = true;
					}
					break;

					default:
					{
						Vector3 target = hitPosition + UniformSampleInHemisphere(hitNormal);
						iterationRay = Ray(hitPosition, target - hitPosition);
						success = true;
					}
					break;
					}

					if (success)
					{
						resultRadiance += throughput * mat.emissive;
						throughput *= mat.baseColor;

					}
				}
				else if (bounces == 0u)
				{
					resultRadiance = Material::MissingMaterialColor;
					break;
				}
			}
			else // No hit
			{
				throughput *= state.m_environmentSettings.m_ambientLightColor;
				resultRadiance += throughput * state.m_environmentSettings.m_ambientLightIntensity;
				success = false;
			}

			bounces++;

		} while (bounces <= m_maxBounces && success);

		return resultRadiance;
	}

}