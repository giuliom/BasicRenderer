#include <random>
#include <cassert>
#include "BasicRenderer.h"
#include "ObjLoader.h"


const std::shared_ptr<const FrameBuffer> BasicRenderer::Render(int width, int height, SceneObject& scene)
{
	assert(width > 0 && height > 0);

	if (width != this->width || height != this->height || fBuffer == nullptr)
	{
		fBuffer = std::make_shared<FrameBuffer>(width, height);
	}

	this->width = width;
	this->height = height;
	fwidth = (float)width;
	fheight = (float)height;
	
	fBuffer->Fill(Color(0.64f, 0.92f ,0.92f));
	camera.SetAspectRatio(width, height);

	//TODO move outside rendering
	scene.transform.SetScale(10.f, 10.f, 10.f);
	scene.transform.SetPosition(0.0f, -1.0f, -5.0f);
	//scene.transform.Rotate(0.0f, 0.01f, 0.0f);
	sun.direction = { 1.0f, -0.5f, 1.0f };

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

	for (int i = 0; i < nfaces; i++)
	{
		Face face = faces[i].ToMatrixSpace(mvp);

		Vector3 faceNormal = (Vector3::CrossProduct(face.v1.pos - face.v0.pos, face.v2.pos - face.v0.pos).Normalize() +1.0f) * 0.5;
		float faceDiffuse =  std::fmaxf(0.0f, Vector3::Dot(faceNormal, sun.direction.Normalize())) * sun.intensity;
		
		face = PerspectiveDivide(face);
		face = NormalizedToScreenSpace(face);

		std::vector<Face> clippedFaces = Clip(face);

		for (auto face : clippedFaces)
		{
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
						fBuffer->WriteToColor(index, Vector3::One() * faceDiffuse);
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

inline std::vector<Face> BasicRenderer::Clip(Face & f) const
{
	std::vector<Face> clippedFaces;

	if (f.v0.pos.w <= 0.0 && f.v1.pos.w <= 0.0 && f.v2.pos.w <= 0.0) {
		return clippedFaces;
	}

	if (f.v0.pos.w > 0.0 &&
		f.v1.pos.w > 0.0 &&
		f.v2.pos.w > 0.0 &&
		abs(f.v0.pos.z) < f.v0.pos.w &&
		abs(f.v1.pos.z) < f.v1.pos.w &&
		abs(f.v2.pos.z) < f.v2.pos.w)
	{
		return clippedFaces;
	}
	else 
	{
		std::vector<Vertex> vertices;
		ClipEdge(f.v0, f.v1, vertices);
		ClipEdge(f.v1, f.v2, vertices);
		ClipEdge(f.v2, f.v0, vertices);

		if (vertices.size() < 3) 
		{
			return clippedFaces;
		}
		if (vertices[vertices.size()-1] != vertices[0])
		{
			vertices.pop_back();
		}

		for (int i = 1; i < vertices.size() - 1; ++i) 
		{
			clippedFaces.push_back(Face(vertices[0], vertices[i], vertices[i + 1]));
		}
	}
	return clippedFaces;
}

inline void BasicRenderer::ClipEdge(Vertex & v0, Vertex & v1, std::vector<Vertex>& vertices) const
{
	Vertex n_v0 = v0;
	Vertex n_v1 = v1;

	bool v0Inside = v0.pos.w > 0.0 && v0.pos.z > -v0.pos.w;
	bool v1Inside = v1.pos.w > 0.0 && v1.pos.z > -v1.pos.w;

	if (v0Inside && v1Inside)
	{
	}
	else if (v0Inside || v1Inside)
	{
		float d0 = v0.pos.z + v0.pos.w;
		float d1 = v1.pos.z + v1.pos.w;
		float factor = 1.0 / (d1 - d0);

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
	else 
	{
		return;
	}


	if (vertices.size() == 0 || !(vertices[vertices.size()-1]!= n_v0))
	{
		vertices.push_back(n_v0);
	}
	vertices.push_back(n_v1);
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

