#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Transform.h"
#include "Vertex.h"
#include "Ray.h"
#include "SceneObject.h"

class Material;

//TODO World space assumed for all these primitives

struct Sphere : public Hitable
{
public:
	Vector3 pos;
	float radius;

	Sphere() {}
	Sphere(Vector3 pos_, float radius_) : pos(pos_), radius(radius_) {}
	Sphere(Vector3 pos_, float radius_, Material* mat) : Hitable(mat), pos(pos_), radius(radius_) {}

	virtual bool GetHit(const Ray& r, float tMin, float tMax, HitResult& result) const override;
};

class Plane : public Hitable
{
	Vector3 centre, normal;

public:
	Plane() = delete;
	Plane(const Vector3& centre, const Vector3& normal, Material* mat) : Hitable(mat), centre(centre), normal(normal.Normalize()) {}

	virtual bool GetHit(const Ray& r, float tMin, float tMax, HitResult& result) const override;

};

class Quad : public SceneObject
{
	Face f0 = Face(Vertex({ -0.5f, 0.5f, 0.f }, { 0.f, 0.f, 1.f }, {}), Vertex({ 0.5f, -0.5f, 0.f }, { 0.f, 0.f, 1.f }, {}), Vertex({ 0.5f, 0.5f, 0.f }, { 0.f, 0.f, 1.f }, {}));
	Face f1 = Face(Vertex({ -0.5f, -0.5f, 0.f }, { 0.f, 0.f, 1.f }, {}), Vertex({ 0.5f, -0.5f, 0.f }, { 0.f, 0.f, 1.f }, {}), Vertex({ -0.5f, 0.5f, 0.f }, { 0.f, 0.f, 1.f }, {}));

public:
	Quad() {}
	Quad(Material* mat) : SceneObject(mat) {}
	Quad(std::shared_ptr<Mesh> mesh_) = delete;
	Quad(std::shared_ptr<Mesh> mesh_, Material* mat) :SceneObject(mesh_, mat) {}
	Quad(const SceneObject& obj) = delete;
	Quad(const Quad& quad) : SceneObject(quad) {}
	virtual ~Quad() {}

	//virtual bool GetHit(const Ray& r, float tMin, float tMax, HitResult& result) const override;

};


struct Cube : public SceneObject
{
	Vector3 topN = { 0.f, 1.f, 0.f };
	Vector3 bottomN = { 0.f, -1.f, 0.f };
	Vector3 backN = { 0.f, 0.f, -1.f };
	Vector3 frontN = { 0.f, 0.f, 1.f };
	Vector3 leftN = { -1.f, 0.f, 0.f };
	Vector3 rightN = { 1.f, 0.f, 0.f };

	Vector3 topFrontLeft = { -0.5f, 0.5f, 0.5f };
	Vector3 topFrontRight = { 0.5f, 0.5f, 0.5f };
	Vector3 topBackLeft = { -0.5f, 0.5f, -0.5f };
	Vector3 topBackRight = { 0.5f, 0.5f, -0.5f };

	Vector3 bottomFrontLeft = { -0.5f, -0.5f, 0.5f };
	Vector3 bottomFrontRight = { 0.5f, -0.5f, 0.5f };
	Vector3 bottomBackLeft = { -0.5f, -0.5f, -0.5f };
	Vector3 bottomBackRight = { 0.5f, -0.5f, -0.5f };


	Face back_f0 = Face(Vertex(bottomBackRight, backN, {}), Vertex(topBackLeft, backN, {}), Vertex(topBackRight, backN, {}));
	Face back_f1 = Face(Vertex(bottomBackLeft, backN, {}), Vertex(bottomBackRight, backN, {}), Vertex(topBackLeft, backN, {}));

	Face front_f0 = Face(Vertex(topFrontLeft, frontN, {}), Vertex(topFrontRight, frontN, {}), Vertex(bottomFrontRight, frontN, {}));
	Face front_f1 = Face(Vertex(bottomFrontLeft, frontN, {}), Vertex(topFrontLeft, frontN, {}), Vertex(bottomFrontRight, frontN, {}));

	Face top_f0 = Face(Vertex(topBackLeft, topN, {}), Vertex(topFrontRight, topN, {}), Vertex(topBackRight, topN, {}));
	Face top_f1 = Face(Vertex(topFrontLeft, topN, {}), Vertex(topFrontRight, topN, {}), Vertex(topBackRight, topN, {}));

	Face bottom_f0 = Face(Vertex(bottomFrontLeft, bottomN, {}), Vertex(bottomBackLeft, bottomN, {}), Vertex(bottomFrontRight, bottomN, {}));
	Face bottom_f1 = Face(Vertex(bottomBackLeft, bottomN, {}), Vertex(bottomBackRight, bottomN, {}), Vertex(bottomFrontRight, bottomN, {}));

	Face left_f0 = Face(Vertex(topFrontLeft, leftN, {}), Vertex(bottomBackLeft, leftN, {}), Vertex(topBackLeft, leftN, {}));
	Face left_f1 = Face(Vertex(bottomFrontLeft, leftN, {}), Vertex(bottomBackLeft, leftN, {}), Vertex(topFrontLeft, leftN, {}));

	Face right_f0 = Face(Vertex(topFrontRight, rightN, {}), Vertex(bottomBackRight, rightN, {}), Vertex(topBackRight, rightN, {}));
	Face right_f1 = Face(Vertex(bottomBackRight, rightN, {}), Vertex(topFrontRight, rightN, {}), Vertex(bottomFrontRight, rightN, {}));

public:
	Cube(Material* mat);
	Cube(std::shared_ptr<Mesh> mesh_) = delete;
	Cube(std::shared_ptr<Mesh> mesh_, Material* mat) : SceneObject(mesh_, mat) {}
	Cube(const SceneObject& obj) = delete;
	Cube(const Cube& cube) : SceneObject(cube) {}
	virtual ~Cube() {}

	//virtual bool GetHit(const Ray& r, float tMin, float tMax, HitResult& result) const override;
};
