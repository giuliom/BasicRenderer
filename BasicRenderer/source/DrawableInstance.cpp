#include "DrawableInstance.h"
#include "Mesh.h"
#include "Material.h"

namespace BasicRenderer
{
	namespace
	{
		// Appends the mesh faces transformed by worldMatrix, returning their combined bounds
		AxisAlignedBoundingBox AppendWorldSpaceFaces(const Mesh& mesh, const Matrix4& worldMatrix, FaceBuffer& outFaceBuffer)
		{
			if (mesh.NumFaces() == 0u)
			{
				return AxisAlignedBoundingBox();
			}

			Vector3 boundsMin;
			Vector3 boundsMax;
			bool hasBounds = false;

			for (const Face& original : mesh.GetFaces())
			{
				Face& face = outFaceBuffer.emplace_back(original);
				ToMatrixSpace(face, worldMatrix);
				const AxisAlignedBoundingBox faceBounds = face.UpdateAxisAlignedBoundingBox();

				if (!hasBounds)
				{
					boundsMin = faceBounds.GetMinimum();
					boundsMax = faceBounds.GetMaximum();
					hasBounds = true;
				}
				else
				{
					boundsMin = Vector3::Min(boundsMin, faceBounds.GetMinimum());
					boundsMax = Vector3::Max(boundsMax, faceBounds.GetMaximum());
				}
			}

			return AxisAlignedBoundingBox(boundsMin, boundsMax);
		}
	}

	DrawableInstance DrawableInstance::ProcessForRendering(const Transform& transform, FaceBuffer& outFaceBuffer) const
	{
		DrawableInstance processed(*this);

		if (const Sphere* sphere = std::get_if<Sphere>(&m_drawable))
		{
			Sphere worldSphere = BasicRenderer::ProcessForRendering(*sphere, transform);
			processed.m_boundingBox = worldSphere.GetAxisAlignedBoundingBox();
			processed.m_drawable = std::move(worldSphere);
		}
		else if (const Plane* plane = std::get_if<Plane>(&m_drawable))
		{
			Plane worldPlane = BasicRenderer::ProcessForRendering(*plane, transform.GetWorldMatrix());
			processed.m_boundingBox = worldPlane.GetAxisAlignedBoundingBox();
			processed.m_drawable = std::move(worldPlane);
		}
		else if (const MeshRef* mesh = std::get_if<MeshRef>(&m_drawable); mesh != nullptr && *mesh != nullptr)
		{
			processed.m_firstFace = static_cast<uint32_t>(outFaceBuffer.size());
			processed.m_faceCount = static_cast<uint32_t>((*mesh)->NumFaces());
			processed.m_boundingBox = AppendWorldSpaceFaces(**mesh, transform.GetWorldMatrix(), outFaceBuffer);
		}

		return processed;
	}

	size_t DrawableInstance::NumSourceFaces() const noexcept
	{
		const MeshRef* mesh = std::get_if<MeshRef>(&m_drawable);
		return (mesh != nullptr && *mesh != nullptr) ? (*mesh)->NumFaces() : 0u;
	}
}
