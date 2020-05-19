#pragma once

#include "Global.h"
#include "Vertex.h"
#include "Material.h"

namespace BasicRenderer
{
	class FrameBuffer;
	class World;
	class Camera;

	class Raytracer
	{

	public:
		Raytracer() {}

		void RayTracing(FrameBuffer& fBuffer, const World& scene, const Camera& camera, uint pixelSamples, uint bounces, Color(Material::* shading)(const World& w, const Vector3& pos, const Vector3& nrml));

	protected:
		Color RayTrace(const Ray& ray, const World& scene, uint bounces, Color(Material::* shading)(const World& w, const Vector3& pos, const Vector3& nrml));
	};
}