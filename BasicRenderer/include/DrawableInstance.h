#pragma once

#include <memory>
#include <vector>
#include "Global.h"
#include "Drawable.h"
#include "Face.h"

namespace BasicRenderer
{
	class Material;

	class DrawableInstance
	{
		Drawable m_drawable;
		std::shared_ptr<Material> m_material;
		uint32_t m_firstFace = 0u;	// Face range and bounding box are only valid on
		uint32_t m_faceCount = 0u;	// the instances returned by ProcessForRendering
		AxisAlignedBoundingBox m_boundingBox;

	public:

		DrawableInstance() = delete;
		DrawableInstance(Drawable drawable, std::shared_ptr<Material> material = nullptr)
			: m_drawable(std::move(drawable)), m_material(std::move(material)) {}
		DrawableInstance(const DrawableInstance& other) = default;
		DrawableInstance(DrawableInstance&& other) noexcept = default;

		DrawableInstance& operator=(const DrawableInstance& other) = default;
		DrawableInstance& operator=(DrawableInstance&& other) noexcept = default;

		// Returns the world-space copy of this instance, appending the faces of a mesh to outFaceBuffer
		DrawableInstance ProcessForRendering(const Transform& transform, FaceBuffer& outFaceBuffer) const;

		// Number of faces ProcessForRendering will append to a FaceBuffer
		size_t NumSourceFaces() const noexcept;

		// Analytic geometry only: mesh instances are hit through the faces they emitted
		inline bool GetHit(const Ray& r, float tMin, float tMax, HitResult& outHit) const noexcept
		{
			if (const Sphere* sphere = std::get_if<Sphere>(&m_drawable))
			{
				return sphere->GetHit(r, tMin, tMax, outHit);
			}
			if (const Plane* plane = std::get_if<Plane>(&m_drawable))
			{
				return plane->GetHit(r, tMin, tMax, outHit);
			}
			return false;
		}

		inline const Drawable& GetDrawable() const noexcept { return m_drawable; }
		inline Material* GetMaterial() const noexcept { return m_material.get(); }
		inline bool IsMesh() const noexcept { return std::holds_alternative<MeshRef>(m_drawable); }
		inline uint32_t GetFirstFace() const noexcept { return m_firstFace; }
		inline uint32_t NumFaces() const noexcept { return m_faceCount; }
		inline const AxisAlignedBoundingBox& GetAxisAlignedBoundingBox() const noexcept { return m_boundingBox; }
	};

	using DrawableInstanceList = std::vector<DrawableInstance>;
}
