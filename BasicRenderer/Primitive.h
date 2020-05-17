#pragma once

#include "Vector3.h"
#include <memory>
#include "Ray.h"
#include "Matrix4.h"

namespace BasicRenderer
{
	class Material;

	class Primitive
	{
	protected:
		//TODO temporary implementation
		Material* material = nullptr;

	public:

		Primitive();
		Primitive(Material* mat) : material(mat) {}
		Primitive(const Primitive& other) : material(other.GetMaterial()) {}

		virtual void ProcessForRendering() = 0;
		virtual bool GetHit(const Ray& r, float tMin, float tMax, float& tHit, Vector3& normalHit) const = 0;

		inline Material* GetMaterial() const { return material; }

	};
}