#pragma once

#include "Matrix4.h"
#include "Vector3.h"

class Transform
{
protected:

	Transform* parent = nullptr; //TODO
	Vector3 position;
	Vector3 scale;
	Vector3 rotation;

public:

	Matrix4 m;

protected:

	inline Matrix4 GetTranslationMatrix();
	inline Matrix4 GetScaleMatrix();
	inline Matrix4 GetRotationMatrix();

	inline void UpdateTransform()
	{
		m = GetTranslationMatrix() * GetRotationMatrix() * GetScaleMatrix();
	}


public:
	Transform() : m(), parent(nullptr), position(Vector3::Zero()), scale(Vector3::One()), rotation(Vector3::Zero()) {}
	Transform(const Transform& t) : m(t.m), parent(t.parent), position(t.position), scale(t.scale), rotation(t.rotation) {}
	Transform(Transform&& t) : m(t.m), parent(t.parent), position(t.position), scale(t.scale), rotation(t.rotation) {}
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
	void SetRotation(float x, float y, float z);

	void Translate(const Vector3& position);
	void Scale(const Vector3& scale);
	void Rotate(const Vector3& rotation);


	inline Matrix4 GetInverseMatrix() { return m.Inverse(); }
	inline Vector4 GetTransformVector(const Vector4 &v)	{ return m * v; }

};

