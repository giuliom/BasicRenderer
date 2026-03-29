#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <thread>
#include <future>
#include <chrono>
#include "Raytracer.h"
#include "BasicRenderer.h"
#include "FrameBuffer.h"
#include "World.h"
#include "Ray.h"
#include "Material.h"



namespace BasicRenderer
{


	void Raytracer::Render(FrameBuffer& fBuffer, const RenderState& state, const ShadingFunc& Shading)
	{
		m_fBuffer = &fBuffer;
		m_shadingFunc = Shading;
		m_pixelSamples = std::max(1u, m_pixelSamples);
		m_pixelsRendered = 0u;
		m_totalPixels = static_cast<uint64_t>(fBuffer.GetWidth()) * static_cast<uint64_t>(fBuffer.GetHeight());

		std::vector<std::future<size_t>> renderFutures;
		const uint threadCount = std::thread::hardware_concurrency() > 1u ? std::thread::hardware_concurrency() - 1u : 1u;
		const uint rowCount = fBuffer.GetHeight();
		const uint rowsPerThread = rowCount / threadCount;
		const uint remainderRows = rowCount % threadCount;

		std::cout << "Rendering with " << threadCount << " threads\n";
		std::cout << "Progress: " << 0u << "% \r";

		const auto beginTime = std::chrono::high_resolution_clock::now();

		uint currentRow = 0u;
		for (uint i = 0u; i < threadCount; i++)
		{
			const uint startRowIndex = currentRow;
			const uint extraRow = i < remainderRows ? 1u : 0u;
			const uint endRowIndex = startRowIndex + rowsPerThread + extraRow;
			currentRow = endRowIndex;

			auto future = std::async(std::launch::async, &Raytracer::RenderJob, this, std::cref(state), startRowIndex, endRowIndex);
			renderFutures.push_back(std::move(future));
		}

#if !LIB_DEBUG && !LIB_RELEASE
		while (true)
		{
			const uint64_t rendered = m_pixelsRendered.load(std::memory_order_relaxed);
			if (rendered >= m_totalPixels)
			{
				break;
			}
			const uint32_t percent = static_cast<uint32_t>((rendered * 100u) / m_totalPixels);
		std::cout << "Progress: " << percent << "% \r" << std::flush;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
#endif

		size_t totalRays = 0u;
		for (auto& f : renderFutures)
		{
			totalRays += f.get();
		}

		const auto endTime = std::chrono::high_resolution_clock::now();
		double ms = ConvertChronoDuration<double, std::chrono::milliseconds>(endTime - beginTime);

		std::cout << "Progress: " << 100u << "% \r";
		std::cout << "\nRendering completed";
		std::cout << std::fixed << std::setprecision(0);
		std::cout << std::endl << "Rendering Time: " << ms << " ms " << "\n";
		std::cout << std::setprecision(2) << "Rays traced: " << totalRays << ", " << (totalRays / (ms / 1000.0) / 1e6) << " Mrays/s\n\n";
		std::cout << std::scientific;
	}

	size_t Raytracer::RenderJob(const RenderState& state, const uint startRowIndex, const uint endRowIndex)
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

		std::vector<const BVHnode*> dfsStack;
		dfsStack.reserve(state.GetAccelerationStructure().LevelsCount());

		size_t tracedRays = 0;

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
					c += RayTrace(r, state, dfsStack, m_shadingFunc, tracedRays);
				}

				c *= fInversePixelSamples;
				c.r = std::min(c.r, 1.f);
				c.g = std::min(c.g, 1.f);
				c.b = std::min(c.b, 1.f);

				fBuffer.WriteToColor(y * width + x, c);
			}

#if !LIB_DEBUG && !LIB_RELEASE
			m_pixelsRendered.fetch_add(static_cast<uint64_t>(width), std::memory_order_relaxed);
#endif
		}

		return tracedRays;
	}

	Color Raytracer::RayTrace(const Ray& ray, const RenderState& state, std::vector<const BVHnode*>& dfsStack, const ShadingFunc& Shading, size_t& outTracedRays)
	{
		(void)Shading;
		Vector3 hitPosition, hitNormal;
		const Primitive* hitObject = nullptr;

		uint bounces = 0;
		bool success = false;
		Ray iterationRay = ray;
		Color throughput = { 1.f, 1.f, 1.f };
		Color resultRadiance = { 0.f, 0.f, 0.f };
		const AccelerationStructure& acc = state.GetAccelerationStructure();

		do
		{
			if ((hitObject = Raycast(acc, iterationRay, 0.0001f, 999999.99f, dfsStack, hitPosition, hitNormal)) != nullptr)
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
						Vector3 reflected = Vector3::Reflect(iterationRay.GetDirection(), hitNormal);
						iterationRay = Ray(hitPosition, reflected + UniformSampleInHemisphere(hitNormal) * (1.f - mat.metallic));
						success = (Vector3::Dot(iterationRay.GetDirection(), hitNormal) > 0);
					}
					break;

					case Material::Type::DIELECTRIC:
					{
						Vector3 outNormal;
						Vector3 reflected = Vector3::Reflect(iterationRay.GetDirection(), hitNormal);
						float ni_nt;
						Vector3 refracted;
						float reflectionProb;
						float cos;
						const auto dotResult = Vector3::Dot(iterationRay.GetDirection(), hitNormal);

						if (dotResult > 0.f)
						{
							outNormal = hitNormal * -1.f;
							ni_nt = mat.refractiveIndex;
							cos = dotResult * mat.refractiveIndex;
						}
						else
						{
							outNormal = hitNormal;
							ni_nt = 1.f / mat.refractiveIndex;
							cos = -dotResult;
						}

						if (Material::Refract(iterationRay, outNormal, ni_nt, refracted))
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
						iterationRay = Ray(hitPosition, UniformSampleInHemisphere(hitNormal));
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

		outTracedRays += bounces;
		return resultRadiance;
	}

}