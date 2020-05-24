#pragma once

#include "Global.h"
#include "RenderSystem.h"
#include "Vertex.h"
#include "Material.h"

namespace BasicRenderer
{
	class FrameBuffer;
	class World;
	class Camera;

	class Raytracer : public RenderSystem
	{

	public:

		uint pixelSamples = 4;
		uint maxBounces = 3;

		Raytracer() {}

		void Render(FrameBuffer& fBuffer, const World& scene, Color(Material::* shading)(const World& w, const Vector3& pos, const Vector3& nrml)) override;

	protected:
		Color RayTrace(const Ray& ray, const World& scene, uint bounces, Color(Material::* shading)(const World& w, const Vector3& pos, const Vector3& nrml));
	};
}