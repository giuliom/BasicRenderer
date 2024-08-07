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

		std::vector<std::thread> renderThreads;
		const uint threadCount = std::thread::hardware_concurrency() > 1u ? std::thread::hardware_concurrency() - 1u : 1u;
		const uint rowCount = fBuffer.GetHeight();
		const uint rowsPerThread = rowCount / threadCount;

		std::cout << "Progress: " << 0u << "% \r";

		for (uint i = 0u; i < threadCount; i++)
		{
			const uint startRowIndex = rowsPerThread * i;
			const uint endRowIndex = (startRowIndex + rowsPerThread) < rowCount ? startRowIndex + rowsPerThread : rowCount;

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
		const float camW = camera.GetViewportWidth();
		const float camH = camera.GetViewportHeight();
		const float aspectRatio = camera.GetAspectRatio();

		const uint width = fBuffer.GetWidth();
		const uint height = fBuffer.GetHeight();

		const float fwidth = static_cast<float>(width);
		const float fheight = static_cast<float>(height);

		const float inverseWidth = 1.0f / fwidth;
		const float inverseHeight = 1.0f / fheight;
		const float fInversePixelSameples = 1.0f / static_cast<float>(m_pixelSamples);

		const float rowPctg =  100.f / fheight;

		std::vector<const BVHnode*> dfsStack;
		dfsStack.reserve(state.GetAccelerationStructure().LevelsCount());

		//Top-left, drawing rows
		for (uint y = startRowIndex; y < endRowIndex; y++)
		{
			for (uint x = 0u; x < width; x++)
			{
				const float u = static_cast<float>(x) * inverseWidth;
				const float v = static_cast<float>(y) * inverseHeight;

				Ray r = camera.GetCameraRay(u, v);

				Color c;
				for (uint i = 0u; i < m_pixelSamples; i++)
				{
					c = c + RayTrace(r, state, dfsStack, m_shadingFunc);
				}

				c = c * fInversePixelSameples;
				c.r = c.r > 1.f ? 1.f : c.r;
				c.g = c.g > 1.f ? 1.f : c.g;
				c.b = c.b > 1.f ? 1.f : c.b;

				fBuffer.WriteToColor((int)(y * fwidth + x), c);
			}

#if !LIB_DEBUG && !LIB_RELEASE
			std::scoped_lock<std::mutex> lock(m_progressMtx);
			m_progress = m_progress + rowPctg;
			std::cout << "Progress: " << static_cast<uint>(std::roundf(m_progress)) << "% \r";
#endif
		}
	}

	Color Raytracer::RayTrace(const Ray& ray, const RenderState& state, std::vector<const BVHnode*>& dfsStack, const ShadingFunc& Shading)
	{
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
						Vector3 reflected = Vector3::Reflect(ray.direction, hitNormal);
						iterationRay = Ray(hitPosition, reflected + UniforSampleInHemisphere(hitNormal) * (1.f - mat.metallic));
						success = (Vector3::Dot(iterationRay.direction, hitNormal) > 0);
					}
					break;

					case Material::Type::DIELECTRIC:
					{
						Vector3 outNormal;
						Vector3 reflected = Vector3::Reflect(ray.direction, hitNormal);
						float ni_nt;
						Vector3 refracted;
						float reflectionProb;
						float cos;
						if (Vector3::Dot(ray.direction, hitNormal) > 0.f)
						{
							outNormal = hitNormal * -1.f;
							ni_nt = mat.refractiveIndex;
							cos = (Vector3::Dot(ray.direction, hitNormal) * mat.refractiveIndex); // / rayIn.direction.Length(); == 1.f
						}
						else
						{
							outNormal = hitNormal;
							ni_nt = 1.f / mat.refractiveIndex;
							cos = -(Vector3::Dot(ray.direction, hitNormal)); // / rayIn.direction.Length(); == 1.f
						}

						if (Material::Refract(ray.direction, outNormal, ni_nt, refracted))
						{
							reflectionProb = Material::Schlick(cos, mat.refractiveIndex);
						}
						else
						{
							reflectionProb = 1.f;
						}

						if (UnitRandf() < reflectionProb)
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
						Vector3 target = hitPosition + UniforSampleInHemisphere(hitNormal);
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
			}

			bounces++;

		} while (bounces <= m_maxBounces && success);

		return resultRadiance;
	}

}