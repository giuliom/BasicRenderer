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

		static constexpr float MIN_RAYCAST_DIST = 0.0001f;	// Bias to avoid self-intersection
		static constexpr float MAX_RAYCAST_DIST = 999999.99f;

		FrameBuffer* m_fBuffer = nullptr;
		ShadingFunc m_shadingFunc;

		uint m_pixelSamples = 4;
		uint m_maxBounces = 3;

		Raytracer() {}

		void Render(FrameBuffer& fBuffer, const RenderState& scene, const ShadingFunc& Shading) override;
		
	protected:

		std::atomic<uint64_t> m_pixelsRendered = 0u;
		std::atomic<uint> m_nextRow = 0u;
		uint64_t m_totalPixels = 0u;

		size_t RenderJob(const RenderState& state);
		Color RayTrace(const Ray& ray, const RenderState& state, const ShadingFunc& shading, size_t& outTracedRays);
	};
}