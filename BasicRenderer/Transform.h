#pragma once

#include "Matrix4.h"
#include "Vector3.h"

class SceneObject;

// TODO Quaternions
class Transform
{
	friend class SceneObject;

protected:

	Vector3 position;
	Vector3 scale;
	Vector3 rotation;
	Transform* parent = nullptr; //TODO
	SceneObject* object = nullptr;
	mutable bool dirty = true;

public:

	Matrix4 m;

	Vector3 forward;
	Vector3 right;
	Vector3 up;

protected:

	inline Matrix4 GetTranslationMatrix(const Vector3& _position);
	inline Matrix4 GetScaleMatrix(const Vector3& _scale);
	inline Matrix4 GetRotationMatrix(const Vector3& _rotation);

	inline void UpdateTransform()
	{
		m = GetTranslationMatrix(position) * GetRotationMatrix(rotation) * GetScaleMatrix(scale);
		forward = Vector3(m.x1, m.y1, m.z1).Normalize();
		up = Vector3(m.x2, m.y2, m.z2).Normalize();
		right = Vector3(-m.x3, -m.y3, -m.z3).Normalize();
		dirty = true;
	}


public:
	Transform() : m(), position(Vector3::Zero()), scale(Vector3::One()), rotation(Vector3::Zero()), parent(nullptr), object(nullptr) {}
	Transform(const Transform& t) : m(t.m), position(t.position), scale(t.scale), rotation(t.rotation), parent(t.parent), object(t.object) {}
	Transform(Transform&& t) : m(t.m), position(t.position), scale(t.scale), rotation(t.rotation), parent(t.parent), object(t.object) {}
	Transform(SceneObject* obj) : m(), position(Vector3::Zero()), scale(Vector3::One()), rotation(Vector3::Zero()), parent(nullptr), object(obj) {}
	Transform(const Vector3& pos, const Vector3& scl, const Vector3& rot, Transform* parent = nullptr, SceneObject* obj = nullptr);
	~Transform() {}

	Transform& operator=(const Transform& t);
	Transform& operator=(Transform&& t);

	inline Vector3 GetPosition() const { return position; }
	inline Vector3 GetScale() const { return scale; }
	inline Vector3 GetRotation() const { return rotation; }

	void SetPosition(const Vector3& position);
	void SetScale(const Vector3& scale);
	void SetRotation(const Vector3& rotation);
	void SetPosition(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetRotation(float roll, float yaw, float pitch);

	void Translate(const Vector3& position_);
	void Translate(float x, float y, float z);
	void Scale(const Vector3& scale_);
	void Scale(float x, float y, float z);
	void Rotate(const Vector3& rotation_);
	void Rotate(float roll, float yaw, float pitch);

	Transform* GetParent() const { return parent; }
	void SetParent(Transform* par);
	Transform Combine(const Transform& other) const;

	inline Matrix4 GetInverseMatrix() { return m.Inverse(); }
	inline Vector4 GetTransformVector(const Vector4 &v)	{ return m * v; }

};

