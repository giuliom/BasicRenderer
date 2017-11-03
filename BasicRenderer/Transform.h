#pragma once

#include "Matrix4.h"
#include "Vector3.h"

class Transform
{
protected:

	Vector3 position;
	Vector3 scale;
	Vector3 rotation;

	void UpdateTransform();

	Matrix4 GetPositionMatrix(Vector3 p);
	Matrix4 GetScaleMatrix(Vector3 s);
	Matrix4 GetRotationMatrix(Vector3 r);

public:
	Transform() : m(), position(Vector3::Zero()), scale(Vector3::One()), rotation(Vector3::Zero()) {}
	Transform(Matrix4 m) : m(m) {} //TODO pos, scale, rotation from Matrix4
	~Transform();

	Matrix4 m;

	Vector3 GetPosition() { return position; }
	Vector3 GetScale() { return scale; }
	Vector3 GetRotation() { return rotation; }

	void SetPosition(Vector3 position);
	void SetScale(Vector3 scale);
	void SetRotation(Vector3 rotation);
	void SetPosition(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	void Translate(Vector3 position);
	void Scale(Vector3 scale);
	void Rotate(Vector3 rotation);

	Matrix4 GetInverseMatrix();

	Vector4 GetTransformVector(Vector4 &v);
};

