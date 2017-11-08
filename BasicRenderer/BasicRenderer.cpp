#include <random>
#include <cassert>
#include "BasicRenderer.h"
#include "ObjLoader.h"



BasicRenderer::~BasicRenderer()
{
}

const FrameBuffer* BasicRenderer::Render(int width, int height, SceneObject& scene)
{
	assert(width > 0 && height > 0);

	if (width != this->width || height != this->height || fBuffer == nullptr)
	{
		if (fBuffer != nullptr) delete fBuffer;
		fBuffer = new FrameBuffer(width, height);
	}

	this->width = width;
	this->height = height;
	fwidth = (float)width;
	fheight = (float)height;
	
	fBuffer->Fill(Color(0.64f, 0.92f ,0.92f));
	camera.SetAspectRatio(width, height);

	scene.transform.SetScale(10.f, 10.f, 10.f);
	scene.transform.SetPosition(Vector3(0.0f, 0.0f, 2.0f));

	if (scene.mesh != nullptr)
	{
		DrawObject(scene);
	}

	return fBuffer;
}

void BasicRenderer::DrawObject(const SceneObject& obj)
{
	Matrix4 view = camera.GetViewMatrix();
	Matrix4 projection = camera.GetProjectionMatrix();

	Matrix4 mvp = projection * view * obj.transform.m;

	const int nfaces = obj.mesh->GetFacesCount();
	const auto faces = obj.mesh->GetFaces();

	//TODO frustum culling
	for (int i = 0; i < nfaces; i++)
	{
		Face face = faces[i].ToMatrixSpace(mvp);

		Vector3 faceNormal = (Vector3::CrossProduct(face.v1.pos - face.v0.pos, face.v2.pos - face.v0.pos).Normalize() +1.0f) * 0.5;
		
		face = PerspectiveDivide(face);
		face = NormalizedToScreenSpace(face);

		if (!CullFace(face))
		{
			Vector4 bbox = BoundingBox(face);
			int bbz = (int)bbox.z;
			int bbw = (int)bbox.w;

			for (int x = (int)bbox.x; x < bbz; ++x)
			{
				for (int y = (int)bbox.y; y < bbw; ++y)
				{
					Vector3 bary = Barycentre((float) x, (float) y, face);

					if (bary.x < 0.0f || bary.y < 0.0f || bary.z < 0.0f) continue;

					float z = face.v0.pos.z * bary.x + face.v1.pos.z * bary.y + face.v2.pos.z * bary.z;

					int index = width * y + x;

					if (z < fBuffer->GetDepth(index))
					{
						fBuffer->WriteToColor(index, faceNormal);
						fBuffer->WriteToDepth(index, z);
					}

				}
			}
		}
	} 
	
}

inline Face BasicRenderer::PerspectiveDivide(Face& f) const
{
	Vector4 v0 = Vector4(f.v0.pos.x / f.v0.pos.w,
						 f.v0.pos.y / f.v0.pos.w,
						 f.v0.pos.z / f.v0.pos.w,
						 f.v0.pos.w);
	Vector4 v1 = Vector4(f.v1.pos.x / f.v1.pos.w,
						 f.v1.pos.y / f.v1.pos.w,
						 f.v1.pos.z / f.v1.pos.w,
						 f.v1.pos.w);
	Vector4 v2 = Vector4(f.v2.pos.x / f.v2.pos.w,
						 f.v2.pos.y / f.v2.pos.w,
						 f.v2.pos.z / f.v2.pos.w,
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

inline Vector3 BasicRenderer::Barycentre(const float x, const float y, const Face& f) const
{
	
	Vector4 ab = f.v1.pos - f.v0.pos;
	Vector4 ac = f.v2.pos - f.v0.pos;
	Vector2 pa = Vector2(f.v0.pos.x - x, f.v0.pos.y -y );
	
	Vector3 uv1 = Vector3::CrossProduct(Vector3(ac.x, ab.x, pa.x), Vector3(ac.y, ab.y, pa.y));
		
		if (abs(uv1.z) < 1e-2f) 
		{
			return Vector3(-1.0f, 1.0f, 1.0f);
		}

		return Vector3((uv1.z - (uv1.x + uv1.y)) * (1.0f / uv1.z), uv1.y * (1.0f / uv1.z), uv1.x * (1.0f / uv1.z));
}

inline Vector2 BasicRenderer::Clamp(Vector2 v, const Vector2 min, const Vector2 max) const
{
	if (v.x < min.x)
	{
		v.x = min.x;
	}
	else if (v.x > max.x)
	{
		v.x = max.x;
	}
	if (v.y < min.y)
	{
		v.y = min.y;
	}
	else if (v.y > max.y)
	{
		v.y = max.y;
	}
	return v;
}
