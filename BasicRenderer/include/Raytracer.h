#pragma once

#include "Global.h"
#include <atomic>
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

		FrameBuffer* m_fBuffer = nullptr;
		ShadingFunc m_shadingFunc;

		uint m_pixelSamples = 4;
		uint m_maxBounces = 3;

		Raytracer() {}

		void Render(FrameBuffer& fBuffer, const RenderState& scene, const ShadingFunc& Shading) override;
		
	protected:

		std::atomic<uint64_t> m_pixelsRendered = 0u;
		uint64_t m_totalPixels = 0u;

		void RenderJob(const RenderState& state, const uint startRowIndex, const uint endRowIndex);
		Color RayTrace(const Ray& ray, const RenderState& state, std::vector<const BVHnode*>& dfsStack, const ShadingFunc& shading);
	};
}