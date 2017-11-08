#include "Transform.h"
#include <math.h>



Matrix4 Transform::GetPositionMatrix()
{
	return Matrix4 (1.0f, 0.0f, 0.0f, position.x,
					0.0f, 1.0f, 0.0f, position.y,
					0.0f, 0.0f, 1.0f, position.z,
					0.0f, 0.0f, 1.0f, 1.0f
					);
}

Matrix4 Transform::GetScaleMatrix()
{
	return Matrix4(	scale.x,0.0f,	0.0f,	0.0f,
					0.0f,	scale.y,0.0f,	0.0f,
					0.0f,	0.0f,	scale.z,0.0f,
					0.0f,	0.0f,	 0.0f,	1.0f
					);
}

// TODO implement full axis rotation
Matrix4 Transform::GetRotationMatrix()
{
	return Matrix4( 1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
				);
}

Transform & Transform::operator=(const Transform & t)
{
	parent = t.parent;
	m = t.m;
	position = t.position;
	scale = t.scale;
	rotation = t.rotation;
	return *this;
}

Transform & Transform::operator=(Transform && t)
{
	parent = t.parent;
	m = t.m;
	position = t.position;
	scale = t.scale;
	rotation = t.rotation;
	return *this;
}

void Transform::SetPosition(const Vector3& position)
{
	this->position = position;
	UpdateTransform();
}

void Transform::SetScale(const Vector3& scale)
{
	this->scale = scale;
	UpdateTransform();
}

void Transform::SetRotation(const Vector3& rotation)
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

void Transform::Translate(const Vector3& position)
{
	this->position = this->position + position;
	UpdateTransform();
}

void Transform::Scale(const Vector3& scale)
{
	this->scale = this->scale + scale;
	UpdateTransform();
}

void Transform::Rotate(const Vector3& rotation)
{
	this->rotation = this->rotation + rotation;
	UpdateTransform();
}




