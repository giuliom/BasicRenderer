#include <cassert>
#include <cmath>
#include <iostream>
#include "BasicRenderer.h"
#include "ObjLoader.h"
#include "Ray.h"
#include "World.h"
#include "Material.h"

const std::shared_ptr<const FrameBuffer> BasicRenderer::Render(int width, int height, World& scene, RenderingMode mode, ShadingMode shading, int samplesPerPixel, int maxBounces)
{
	assert(width > 0 && height > 0);

	if (width != this->width || height != this->height || fBuffer == nullptr)
	{
		fBuffer = std::make_shared<FrameBuffer>(width, height);

		this->width = width;
		this->height = height;
		fwidth = (float)width;
		fheight = (float)height;

		camera.SetAspectRatio(width, height);
	}

	fBuffer->Fill(scene.ambientLightColor);

	scene.ProcessForRendering();

	auto shadingFunc = &Material::LitShading;

	switch (shading)
	{
	case ShadingMode::NORMAL:
		maxBounces = 1;
		shadingFunc = &Material::NormalShading;
		break;
	}

	switch (mode)
	{
	default:
	{
		for (const std::shared_ptr<Primitive> obj : scene.GetHierarchy())
		{
			DrawObject(obj.get(), scene, shadingFunc);
		}
	}
		break;
	case RenderingMode::RAYTRACER:
	{
		
		return RayTracing(width, height, scene, samplesPerPixel, maxBounces, shadingFunc);
	}
		break;
	}
	

	return fBuffer;
}

const std::shared_ptr<const FrameBuffer> BasicRenderer::RayTracing(int width, int height, World & scene, int pixelSamples, int bounces, Color (Material::*shading)(const World& w, const Vector3& pos, const Vector3& nrml))
{
	const float camW = camera.GetHalfWidth() * 2.f;
	const float camH = camera.GetHalfHeight() * 2.f;
	const float aspectRatio = camera.GetAspectRatio();

	const float inverseWidth = 1.0f / fwidth;
	const float inverseHeight = 1.0f / fheight;
	const float fInversePixelSameples = 1.0f / (float)pixelSamples;

	//Top-left
	for (float y = 0.f; y < fheight; y++)
	{
		std::cout << "Progress: " << static_cast<int>((y / fheight) * 100.f + 1.f) << "% \r";

		for (float x = 0.f; x < fwidth; x++)
		{
			const float u = x * inverseWidth;
			const float v = y * inverseHeight;

			Ray r = camera.GetCameraRay(u, v);
			
			Color c;
			for (int i = 0; i < pixelSamples; i++)
			{
				c = c + RayTrace(r, scene, bounces, shading);
			}

			c = c * fInversePixelSameples;
			c.x = c.x > 1.f ? 1.f : c.x;
			c.y = c.y > 1.f ? 1.f : c.y;
			c.z = c.z > 1.f ? 1.f : c.z;
			c = Color(std::powf(c.x, gammaEncoding), std::powf(c.y, gammaEncoding), std::powf(c.z, gammaEncoding));

			fBuffer->WriteToColor((int) (y * fwidth + x), c);
			
		}
	}



	return fBuffer;
}

Color BasicRenderer::RayTrace(const Ray & ray, World& scene, int bounces, Color(Material::*shading)(const World& w, const Vector3& pos, const Vector3& nrml))
{
	Vector3 hitPosition, hitNormal;
	const Primitive* hitObject = nullptr;
	if ((hitObject = scene.Raycast(ray, 0.0001f, 999999.99f, hitPosition, hitNormal)) != nullptr)
	{
		if (hitObject->GetMaterial() == nullptr)
		{
			return missingMaterialColor;
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
					albedo = { 1.f, 1.f, 1.f }; //TODO placeholder
					success = (Vector3::Dot(scattered.direction, hitNormal) > 0);
				}
				break;

				case Material::Type::DIELECTRIC:
				{
					Vector3 outNormal;
					Vector3 reflected = Vector3::Reflect(ray.direction, hitNormal);
					float ni_nt;
					albedo = { 1.f, 1.f, 1.f }; //TODO placeholder
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
			return RayTrace(scattered, scene, bounces - 1, shading) * albedo + mat.emissive;
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

void BasicRenderer::DrawObject(const Primitive* primitive, const World& scene, Color(Material::*shading)(const World& w, const Vector3& pos, const Vector3& nrml))
{
	const SceneObject* obj = dynamic_cast<const SceneObject*>(primitive);

	if (obj != nullptr)
	{
		Material* mat = obj->GetMaterial();
		Color c = missingMaterialColor;
		const Matrix4 mvp = camera.GetViewMatrix() * camera.GetProjectionMatrix();

		for (uint i = 0; i < obj->NumFaces(); i++)
		{
			Face f = obj->GetTransformedFace(i);

			if (mat)
			{
				c = (obj->GetMaterial()->*shading)(scene, Vector3::Zero(), f.normal);
			}
			c = Color(std::powf(c.x, gammaEncoding), std::powf(c.y, gammaEncoding), std::powf(c.z, gammaEncoding));

			ToMatrixSpace(f, mvp);

			PerspectiveDivide(f);
			NormalizedToScreenSpace(f);

			Face clippedFaces[4];
			int nClippedFaces = Clip(f, clippedFaces);

			for (int j = 0; j < nClippedFaces; j++)
			{
				f = clippedFaces[j];

				if (CullFace(f)) continue;

				Vector4 bbox = BoundingBox(f);
				int bbz = (int)bbox.z;
				int bbw = (int)bbox.w;

				for (int x = (int)bbox.x; x < bbz; ++x)
				{
					for (int y = (int)bbox.y; y < bbw; ++y)
					{
						Vector3 bary = Barycentre((float)x, (float)y, f);

						if (bary.x < 0.0f || bary.y < 0.0f || bary.z < 0.0f) continue;

						float z = f.v0.pos.z * bary.x + f.v1.pos.z * bary.y + f.v2.pos.z * bary.z;

						//Because of the Pinhole model
						int index = width * (height - y - 1) + x;

						if (z < fBuffer->GetDepth(index))
						{
							fBuffer->WriteToColor(index, c);
							fBuffer->WriteToDepth(index, z);
						}

					}
				}
			}
		}
	}
}

inline void BasicRenderer::PerspectiveDivide(Face& f) const
{
	float v0w = 1.0f / f.v0.pos.w;
	float v1w = 1.0f / f.v1.pos.w;
	float v2w = 1.0f / f.v2.pos.w;
	Vector4 v0 = Vector4(f.v0.pos.x * v0w,
						 f.v0.pos.y * v0w,
						 f.v0.pos.z * v0w,
						 f.v0.pos.w);
	Vector4 v1 = Vector4(f.v1.pos.x * v1w,
						 f.v1.pos.y * v1w,
						 f.v1.pos.z * v1w,
						 f.v1.pos.w);
	Vector4 v2 = Vector4(f.v2.pos.x * v2w,
						 f.v2.pos.y * v2w,
						 f.v2.pos.z * v2w,
						 f.v2.pos.w);

	f = Face(v0, v1, v2, f); // HERE
}

inline void BasicRenderer::NormalizedToScreenSpace(Face& f) const
{
	Vector4 v0 = Vector4(floorf(0.5f * fwidth  * (f.v0.pos.x + 1.0f)),
						 floorf(0.5f * fheight * (f.v0.pos.y + 1.0f)),
						 f.v0.pos.z,
						 f.v0.pos.w);
	Vector4 v1 = Vector4(floorf(0.5f * fwidth  * (f.v1.pos.x + 1.0f)),
						 floorf(0.5f * fheight * (f.v1.pos.y + 1.0f)),
						 f.v1.pos.z,
						 f.v1.pos.w);
	Vector4 v2 = Vector4(floorf(0.5f * fwidth  * (f.v2.pos.x + 1.0f)),
						 floorf(0.5f * fheight * (f.v2.pos.y + 1.0f)),
						 f.v2.pos.z,
						 f.v2.pos.w);

	f = Face(v0, v1, v2, f); // HERE
}

//TODO use custom dynamic array
inline int BasicRenderer::Clip(const Face& f, Face(&clippedFaces)[4]) const
{
	int nfaces = 0;

	if (f.v0.pos.w <= 0.0 && f.v1.pos.w <= 0.0 && f.v2.pos.w <= 0.0) {
		return 0;
	}

	if (f.v0.pos.w > 0.0 &&
		f.v1.pos.w > 0.0 &&
		f.v2.pos.w > 0.0 &&
		abs(f.v0.pos.z) < f.v0.pos.w &&
		abs(f.v1.pos.z) < f.v1.pos.w &&
		abs(f.v2.pos.z) < f.v2.pos.w)
	{
		return 0;
	}
	else 
	{
		Vertex vertices[6];
		int size = 0;
		size = ClipEdge(f.v0, f.v1, vertices, size);
		size = ClipEdge(f.v1, f.v2, vertices, size);
		size = ClipEdge(f.v2, f.v0, vertices, size);

		// max size() is 6
		if (size < 3)
		{
			return 0;
		}
		if (vertices[size -1] != vertices[0])
		{
			--size;
		}

		for (int i = 1; i < size - 1; ++i)
		{
			clippedFaces[i - 1] = Face(vertices[0], vertices[i], vertices[i + 1]);
			++nfaces;
		}
	}
	return nfaces;
}

inline int BasicRenderer::ClipEdge(const Vertex & v0, const Vertex & v1, Vertex (&vertices)[6], int index) const
{
	assert(index < 5);

	int size = index;
	Vertex n_v0 = v0;
	Vertex n_v1 = v1;

	bool v0Inside = v0.pos.w > 0.0 && v0.pos.z > -v0.pos.w;
	bool v1Inside = v1.pos.w > 0.0 && v1.pos.z > -v1.pos.w;

	if (!v0Inside && !v1Inside)
	{
		return 0;
	}
	else if (v0Inside != v1Inside)
	{
		float d0 = v0.pos.z + v0.pos.w;
		float d1 = v1.pos.z + v1.pos.w;
		float factor = 1.0f / (d1 - d0);

		Vertex nVertex = Vertex((v0.pos * d1 - v1.pos * d0) * factor, 
								(v0.nrml * d1 - v1.nrml * d0) * factor, 
								(v0.uv * d1 - v1.uv * d0) * factor);
		if (v0Inside)
		{
			n_v1 = nVertex;
		}
		else {
			n_v0 = nVertex;
		}
	}

	if ((size == 0 || !(vertices[size - 1] != n_v0)) && size < 6)
	{
		vertices[size] = n_v0;
		++size;
	}
	vertices[size] = n_v1;
	return ++size;
}

inline bool BasicRenderer::CullFace(const Face& f) const
{
	float d = (f.v1.pos.x - f.v0.pos.x) *
		(f.v2.pos.y - f.v0.pos.y) -
		(f.v1.pos.y - f.v0.pos.y) *
		(f.v2.pos.x - f.v0.pos.x);
	return d < 0.0f;
}

inline Vector4 BasicRenderer::BoundingBox(const Face& f) const
{
	Vector2 v0 = Vector2(f.v0.pos.x, f.v0.pos.y);
	Vector2 v1 = Vector2(f.v1.pos.x, f.v1.pos.y);
	Vector2 v2 = Vector2(f.v2.pos.x, f.v2.pos.y);

	Vector2 mini = Vector2::Min(Vector2::Min(v0, v1), v2);
	Vector2 maxi = Vector2::Max(Vector2::Max(v0, v1), v2);

	Vector2 lim = Vector2(fwidth - 1.0f, fheight - 1.0f);

	Vector2 finalMin = Clamp(Vector2::Min(mini, maxi), Vector2(0.0f, 0.0f), lim);
	Vector2 finalMax = Clamp(Vector2::Max(mini, maxi), Vector2(0.0f, 0.0f), lim);

	return Vector4(finalMin.x, finalMin.y, finalMax.x, finalMax.y);
}

