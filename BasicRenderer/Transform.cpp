#include "Transform.h"
#include <math.h>


void Transform::UpdateTransform()
{
	Matrix4 S = GetScaleMatrix(scale);

	Matrix4 R = GetRotationMatrix(rotation);

	Matrix4 T = GetPositionMatrix(position);

	m = T * R * S;
}

Matrix4 Transform::GetPositionMatrix(Vector3 p)
{
	return Matrix4 (1.0f, 0.0f, 0.0f, p.x,
					0.0f, 1.0f, 0.0f, p.y,
					0.0f, 0.0f, 1.0f, p.z,
					0.0f, 0.0f, 1.0f, 1.0f
					);
}

Matrix4 Transform::GetScaleMatrix(Vector3 s)
{
	return Matrix4(	s.x, 0.0f, 0.0f, 0.0f,
					0.0f, s.y, 0.0f, 0.0f,
					0.0f, 0.0f, s.z, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
					);
}

// TODO implement full axis rotation
Matrix4 Transform::GetRotationMatrix(Vector3 r)
{
	return Matrix4( 1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
				);
}

Transform::~Transform()
{

}

void Transform::SetPosition(Vector3 position)
{
	this->position = position;
	UpdateTransform();
}

void Transform::SetScale(Vector3 scale)
{
	this->scale = scale;
	UpdateTransform();
}

void Transform::SetRotation(Vector3 rotation)
{
	this->rotation = rotation;
	UpdateTransform();
}

void Transform::SetPosition(float x, float y, float z)
{
	SetPosition(Vector3(x, y, z));
}

void Transform::SetScale(float x, float y, float z)
{
	SetScale(Vector3(x, y, z));
}

void Transform::SetRotation(float x, float y, float z)
{
	SetRotation(Vector3(x, y, z));
}

void Transform::Translate(Vector3 position)
{
	this->position = this->position + position;
	UpdateTransform();
}

void Transform::Scale(Vector3 scale)
{
	this->scale = this->scale + scale;
	UpdateTransform();
}

void Transform::Rotate(Vector3 rotation)
{
	this->rotation = this->rotation + rotation;
	UpdateTransform();
}

Matrix4 Transform::GetInverseMatrix()
{
	return m.Inverse();
}

Vector4 Transform::GetTransformVector(Vector4 & v)
{
	return m * v;
}
