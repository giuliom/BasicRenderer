#include <cassert>
#include <cmath>
#include "BasicRenderer.h"
#include "ObjLoader.h"
#include "Ray.h"
#include "World.h"
#include "Material.h"

const std::shared_ptr<const FrameBuffer> BasicRenderer::Render(int width, int height, World& scene, RenderingMode mode, ShadingMode shading)
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

	auto shadingFunc = &Material::LitShading;
	int bounces = maxBounces;

	switch (shading)
	{
	case ShadingMode::NORMAL:
		bounces = 1;
		shadingFunc = &Material::NormalShading;
		break;
	}

	switch (mode)
	{
	default:
		for (auto& obj : scene.hierarchy)
		{
			SceneObject* sceneObj = dynamic_cast<SceneObject*>(obj);
			if (sceneObj != nullptr && sceneObj->GetMesh() != nullptr)
			{
				DrawObject(*sceneObj, scene, shadingFunc);
			}
		}
		break;
	case RenderingMode::RAYTRACER:
		RayTracing(width, height, scene,bounces, shadingFunc);
		break;
	}
	

	return fBuffer;
}

const std::shared_ptr<const FrameBuffer> BasicRenderer::RayTracing(int width, int height, World & scene, int bounces, Color (Material::*shading)(const World& w, const Vector3& pos, const Vector3& nrml))
{
	const float camW = camera.GetHalfWidth() * 2.f;
	const float camH = camera.GetHalfHeight() * 2.f;
	const float aspectRatio = camera.GetAspectRatio();

	//Top-left
	for (float y = 0.f; y < fheight; y++)
	{
		for (float x = 0.f; x < fwidth; x++)
		{
			float u = x / fwidth;
			float v = y / fheight;

			Ray r = camera.GetCameraRay(u, v);
			
			Color c;
			for (int i = 0; i < pixelSamples; i++)
			{
				c = c + RayTrace(r, scene, bounces, shading);
			}

			c = c / (float) pixelSamples;
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
	HitResult hit;
	if (scene.GetHit(ray, 0.0001f, 999999.99f, hit))
	{
		if (hit.material == nullptr)
		{
			return missingMaterialColor;
		}

		Ray scattered;
		Material mat = *hit.material;
		Color albedo;
		bool success = false;
		//return hit.normal; // TODO to show normals
		if (bounces > 0)
		{
			switch (mat.type)
			{
				case Material::Type::METALLIC:
				{
					Vector3 reflected = Vector3::Reflect(ray.direction, hit.normal);
					scattered = Ray(hit.pos, reflected + UniforSampleInHemisphere(hit.normal) * (1.f - mat.metallic));
					albedo = { 1.f, 1.f, 1.f }; //TODO placeholder
					success = (Vector3::Dot(scattered.direction, hit.normal) > 0);
				}
				break;

				case Material::Type::DIELECTRIC:
				{
					Vector3 outNormal;
					Vector3 reflected = Vector3::Reflect(ray.direction, hit.normal);
					float ni_nt;
					albedo = { 1.f, 1.f, 1.f }; //TODO placeholder
					Vector3 refracted;
					float reflectionProb;
					float cos;
					if (Vector3::Dot(ray.direction, hit.normal) > 0.f)
					{
						outNormal = hit.normal * -1.f;
						ni_nt = mat.refractiveIndex;
						cos = (Vector3::Dot(ray.direction, hit.normal) * mat.refractiveIndex); // / rayIn.direction.Length(); == 1.f
					}
					else
					{
						outNormal = hit.normal;
						ni_nt = 1.f / mat.refractiveIndex;
						cos = -(Vector3::Dot(ray.direction, hit.normal)); // / rayIn.direction.Length(); == 1.f
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
						scattered = Ray(hit.pos, reflected);
					}
					else
					{
						scattered = Ray(hit.pos, refracted);
					}

					success = true;
				}
				break;

				default:
				{
					Vector3 target = hit.pos + UniforSampleInHemisphere(hit.normal);
					scattered = Ray(hit.pos, target - hit.pos);
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

void BasicRenderer::DrawObject(const SceneObject& obj, const World& scene, Color(Material::*shading)(const World& w, const Vector3& pos, const Vector3& nrml))
{
	Matrix4 view = camera.GetViewMatrix();
	Matrix4 projection = camera.GetProjectionMatrix();

	Matrix4 mvp = projection * view * obj.UpdateWorldTransform().m;

	const int nfaces = obj.GetMesh()->GetFacesCount();
	const auto faces = obj.GetMesh()->GetFaces();

	for (int i = 0; i < nfaces; i++)
	{
		Face face = faces[i].ToMatrixSpace(mvp);

		Vector3 faceNormal = Vector3::CrossProduct(face.v1.pos - face.v0.pos, face.v2.pos - face.v0.pos).Normalize();

		Material* mat = obj.GetMaterial();
		Color c;
		if (mat)
		{
			c = (obj.GetMaterial()->*shading)(scene, Vector3::Zero(), faceNormal);
		}
		else
		{
			c = missingMaterialColor;
		}

		c = Color(std::powf(c.x, gammaEncoding), std::powf(c.y, gammaEncoding), std::powf(c.z, gammaEncoding));

		face = PerspectiveDivide(face);
		face = NormalizedToScreenSpace(face);

		Face clippedFaces[4];
		int nClippedFaces = Clip(face, clippedFaces);

		for (int j = 0; j < nClippedFaces; j++)
		{
			face = clippedFaces[j];

			if (CullFace(face)) continue;

			Vector4 bbox = BoundingBox(face);
			int bbz = (int)bbox.z;
			int bbw = (int)bbox.w;

			for (int x = (int)bbox.x; x < bbz; ++x)
			{
				for (int y = (int)bbox.y; y < bbw; ++y)
				{
					Vector3 bary = Barycentre((float)x, (float)y, face);

					if (bary.x < 0.0f || bary.y < 0.0f || bary.z < 0.0f) continue;

					float z = face.v0.pos.z * bary.x + face.v1.pos.z * bary.y + face.v2.pos.z * bary.z;

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

inline Face BasicRenderer::PerspectiveDivide(Face& f) const
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

	return Face(v0, v1, v2, f);
}

inline Face BasicRenderer::NormalizedToScreenSpace(Face& f) const
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

	return Face(v0, v1, v2, f);
}

//TODO use custom dynamic array
inline int BasicRenderer::Clip(Face & f, Face (&clippedFaces)[4]) const
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

inline int BasicRenderer::ClipEdge(Vertex & v0, Vertex & v1, Vertex (&vertices)[6], int index) const
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

