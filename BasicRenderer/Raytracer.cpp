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

		uint thread_count = std::thread::hardware_concurrency();
		std::vector<std::thread> renderThreads;

		for (uint i = 0u; i < thread_count; i++)
		{
			auto thread = std::thread(&Raytracer::RenderJob, this, i, thread_count);
			renderThreads.push_back(std::move(thread));
		}

		for (auto& t : renderThreads)
		{
			t.join();
		}

		std::cout << "\nRendering completed";
	}

	void Raytracer::RenderJob(const uint index, const uint totThreads)
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

		const float rowPctg = 100.f / fheight;

		//Top-left, drawing rows
		for (uint y = index; y < height; y += totThreads)
		{
			for (uint x = 0u; x < width; x++)
			{
				const float u = static_cast<float>(x) * inverseWidth;
				const float v = static_cast<float>(y) * inverseHeight;

				Ray r = camera.GetCameraRay(u, v);

				Color c;
				for (uint i = 0u; i < m_pixelSamples; i++)
				{
					c = c + RayTrace(r, scene, m_maxBounces, m_shadingFunc);
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

	Color Raytracer::RayTrace(const Ray& ray, const World& scene, uint bounces, const ShadingFunc& Shading)
	{
		Vector3 hitPosition, hitNormal;
		const Primitive* hitObject = nullptr;
		if ((hitObject = scene.Raycast(ray, 0.0001f, 999999.99f, hitPosition, hitNormal)) != nullptr)
		{
			if (hitObject->GetMaterial() == nullptr)
			{
				return Material::MissingMaterialColor;
			}

			Ray scattered;
			const Material mat = *hitObject->GetMaterial();
			Color albedo;
			bool success = false;
			//return hit.normal; // TODO to show normals
			if (bounces > 0)
			{
				switch (mat.type)
				{
				case Material::Type::METALLIC:
				{
					Vector3 reflected = Vector3::Reflect(ray.direction, hitNormal);
					scattered = Ray(hitPosition, reflected + UniforSampleInHemisphere(hitNormal) * (1.f - mat.metallic));
					albedo = mat.baseColor;
					success = (Vector3::Dot(scattered.direction, hitNormal) > 0);
				}
				break;

				case Material::Type::DIELECTRIC:
				{
					Vector3 outNormal;
					Vector3 reflected = Vector3::Reflect(ray.direction, hitNormal);
					float ni_nt;
					albedo = mat.baseColor;
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
						scattered = Ray(hitPosition, reflected);
					}
					else
					{
						scattered = Ray(hitPosition, refracted);
					}

					success = true;
				}
				break;

				default:
				{
					Vector3 target = hitPosition + UniforSampleInHemisphere(hitNormal);
					scattered = Ray(hitPosition, target - hitPosition);
					albedo = mat.baseColor;
					success = true;
				}
				break;
				}
			}

			if (success)
			{
				return RayTrace(scattered, scene, bounces - 1, Shading) * albedo + mat.emissive;
			}
			else
			{
				return Vector3(mat.emissive, mat.emissive, mat.emissive);
			}
		}
		else
		{
			return scene.ambientLightColor * scene.ambientLightIntensity;
		}
	}

}