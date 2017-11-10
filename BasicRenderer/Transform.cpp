#include "Transform.h"
#include <math.h>



Matrix4 Transform::GetTranslationMatrix()
{
	return Matrix4 (1.0f, 0.0f, 0.0f, position.x,
					0.0f, 1.0f, 0.0f, position.y,
					0.0f, 0.0f, 1.0f, position.z,
					0.0f, 0.0f, 0.0f, 1.0f
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

// Right-hand
Matrix4 Transform::GetRotationMatrix()
{
	// X-axis
	Matrix4 roll(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosf(rotation.x), -sinf(rotation.x), 0.0f,
		0.0f, sinf(rotation.x), cosf(rotation.x), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	 
	 // Y-axis
	 Matrix4 yaw(	cosf(rotation.y),	0.0f, sinf(rotation.y), 0.0f,
					0.0f,				1.0f, 0.0f,				0.0f,
					-sinf(rotation.y),	0.0f, cosf(rotation.y),	0.0f,
					0.0f,				0.0f, 0.0f,				1.0f
	 );
	 
	 // Z-axis
	 Matrix4 pitch(cosf(rotation.z), -sinf(rotation.z), 0.0f, 0.0f,
		 sinf(rotation.z), cosf(rotation.z), 0.0f, 0.0f,
		 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f, 0.0f, 0.0f, 1.0f
	 );

	 return yaw * pitch * roll;
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

void Transform::SetRotation(float roll, float yaw, float pitch)
{
	SetRotation(Vector3(roll, yaw, pitch));
}

void Transform::Translate(const Vector3& position_)
{
	position = position + position_;
	UpdateTransform();
}

void Transform::Translate(float x, float y, float z)
{
	Translate(Vector3(x, y, z));
}

void Transform::Scale(const Vector3& scale_)
{
	scale = scale + scale_;
	UpdateTransform();
}

void Transform::Scale(float x, float y, float z)
{
	Scale(Vector3(x, y, z));
}

void Transform::Rotate(const Vector3& rotation_)
{
	rotation = rotation + rotation_;
	UpdateTransform();
}

void Transform::Rotate(float roll, float yaw, float pitch)
{
	Rotate(Vector3(roll, yaw, pitch));
}




