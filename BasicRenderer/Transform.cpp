#include "Transform.h"
#include <math.h>



Matrix4 Transform::GetTranslationMatrix(const Vector3& _position)
{
	return Matrix4 (1.0f, 0.0f, 0.0f, _position.x,
					0.0f, 1.0f, 0.0f, _position.y,
					0.0f, 0.0f, 1.0f, _position.z,
					0.0f, 0.0f, 0.0f, 1.0f
					);
}

Matrix4 Transform::GetScaleMatrix(const Vector3& _scale)
{
	return Matrix4(	_scale.x,	0.0f,		0.0f,		0.0f,
					0.0f,		_scale.y,	0.0f,		0.0f,
					0.0f,		0.0f,		_scale.z,	0.0f,
					0.0f,		0.0f,		0.0f,		1.0f
					);
}

// Right-hand
Matrix4 Transform::GetRotationMatrix(const Vector3& _rotation)
{
	// X-axis
	Matrix4 roll(	1.0f, 0.0f,					0.0f,				0.0f,
					0.0f, cosf(_rotation.x),	-sinf(_rotation.x), 0.0f,
					0.0f, sinf(_rotation.x),	cosf(_rotation.x),	0.0f,
					0.0f, 0.0f,					0.0f,				1.0f
	);
	 
	 // Y-axis
	 Matrix4 yaw(	cosf(_rotation.y),	0.0f, sinf(_rotation.y),	0.0f,
					0.0f,				1.0f, 0.0f,					0.0f,
					-sinf(_rotation.y),	0.0f, cosf(_rotation.y),	0.0f,
					0.0f,				0.0f, 0.0f,					1.0f
	 );
	 
	 // Z-axis
	 Matrix4 pitch(	cosf(_rotation.z),	-sinf(_rotation.z),	0.0f, 0.0f,
					sinf(_rotation.z),	cosf(_rotation.z),	0.0f, 0.0f,
					0.0f,				0.0f,				1.0f, 0.0f,
					0.0f,				0.0f,				0.0f, 1.0f
	 );

	 return yaw * pitch * roll;
}

Transform::Transform(const Vector3 & pos, const Vector3 & scl, const Vector3 & rot, Transform * _parent, SceneObject * obj)
{
	position = pos;
	scale = scl;
	rotation = rot;
	parent = _parent;
	object = obj;

	UpdateTransform();
}

Transform & Transform::operator=(const Transform & t)
{
	parent = t.parent;
	m = t.m;
	position = t.position;
	scale = t.scale;
	rotation = t.rotation;
	object = t.object;
	return *this;
}

Transform & Transform::operator=(Transform && t)
{
	parent = t.parent;
	m = t.m;
	position = t.position;
	scale = t.scale;
	rotation = t.rotation;
	object = t.object;
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

void Transform::SetParent(Transform * par)
{
	parent = par;
	dirty = true;
}

Transform Transform::Combine(const Transform & other) const
{
	return Transform(position + other.position, rotation + other.rotation, scale + other.scale, other.parent, other.object);
}




