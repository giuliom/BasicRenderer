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
	std::mutex Raytracer::m_progressMtx;

	void Raytracer::Render(FrameBuffer& fBuffer, const World& scene, const ShadingFunc& Shading)
	{
		m_fBuffer = &fBuffer;
		m_scene = &scene;
		m_shadingFunc = Shading;

		std::vector<std::thread> renderThreads;
		const uint threadCount = std::thread::hardware_concurrency() > 1u ? std::thread::hardware_concurrency()  - 1u : 1u;
		const uint rowCount = fBuffer.GetHeight();
		const uint rowsPerThread = rowCount / threadCount;

		std::cout << "Progress: " << 0u << "% \r";

		for (uint i = 0u; i < threadCount; i++)
		{
			const uint startRowIndex = rowsPerThread * i;
			const uint endRowIndex = (startRowIndex + rowsPerThread) < rowCount ? startRowIndex + rowsPerThread : rowCount;

			auto thread = std::thread(&Raytracer::RenderJob, this, startRowIndex, endRowIndex);
			renderThreads.push_back(std::move(thread));
		}

		for (auto& t : renderThreads)
		{
			t.join();
		}

		std::cout << "Progress: " << 100u << "% \r";
		std::cout << "\nRendering completed";
	}

	void Raytracer::RenderJob(const uint startRowIndex, const uint endRowIndex)
	{
		const World& scene = *m_scene;
		FrameBuffer& fBuffer = *m_fBuffer;

		const Camera& camera = scene.GetMainCamera();
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
					c = c + RayTrace(r, scene, m_shadingFunc);
				}

				c = c * fInversePixelSameples;
				c.x = c.x > 1.f ? 1.f : c.x;
				c.y = c.y > 1.f ? 1.f : c.y;
				c.z = c.z > 1.f ? 1.f : c.z;

				fBuffer.WriteToColor((int)(y * fwidth + x), c);
			}

			const std::lock_guard<std::mutex> lock(m_progressMtx);
			m_progress = m_progress + rowPctg;
			std::cout << "Progress: " << static_cast<uint>(std::roundf(m_progress)) << "% \r";
		}
	}

	Color Raytracer::RayTrace(const Ray& ray, const World& scene, const ShadingFunc& Shading)
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
			if ((hitObject = scene.Raycast(iterationRay, 0.0001f, 999999.99f, hitPosition, hitNormal)) != nullptr)
			{
				if (hitObject->GetMaterial() == nullptr && bounces == 0u)
				{
					throughput = Material::MissingMaterialColor;
				}

				success = false;
				const Material& mat = *hitObject->GetMaterial();

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
			else // No hit
			{
				throughput *= scene.ambientLightColor;
				resultRadiance += throughput * scene.ambientLightIntensity;
			}

			bounces++;

		} while (bounces <= m_maxBounces && success);

		return resultRadiance;
	}

}