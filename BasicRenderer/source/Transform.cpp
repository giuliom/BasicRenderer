#include "Transform.h"
#include <math.h>

namespace BasicRenderer
{
	Matrix4 Transform::GetTranslationMatrix(const Vector3& position)
	{
		return Matrix4(1.0f, 0.0f, 0.0f, position.x,
			0.0f, 1.0f, 0.0f, position.y,
			0.0f, 0.0f, 1.0f, position.z,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	Matrix4 Transform::GetScaleMatrix(const Vector3& scale)
	{
		return Matrix4(scale.x, 0.0f, 0.0f, 0.0f,
			0.0f, scale.y, 0.0f, 0.0f,
			0.0f, 0.0f, scale.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	// Right-hand
	Matrix4 Transform::GetRotationMatrix(const Vector3& rotation)
	{
		// X-axis
		Matrix4 roll(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, cosf(rotation.x), -sinf(rotation.x), 0.0f,
			0.0f, sinf(rotation.x), cosf(rotation.x), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		// Y-axis
		Matrix4 yaw(cosf(rotation.y), 0.0f, sinf(rotation.y), 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			-sinf(rotation.y), 0.0f, cosf(rotation.y), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		// Z-axis
		Matrix4 pitch(cosf(rotation.z), -sinf(rotation.z), 0.0f, 0.0f,
			sinf(rotation.z), cosf(rotation.z), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		return yaw * pitch * roll;
	}

	Transform::Transform(const Vector3& pos, const Vector3& scl, const Vector3& rot, Transform* _parent, SceneObject* obj)
	{
		m_position = pos;
		m_scale = scl;
		m_rotation = rot;
		m_parent = _parent;
		m_object = obj;

		UpdateTransform();
	}

	Transform& Transform::operator=(const Transform& t)
	{
		m_parent = t.m_parent;
		m_matrix = t.m_matrix;
		m_position = t.m_position;
		m_scale = t.m_scale;
		m_rotation = t.m_rotation;
		m_object = t.m_object;
		UpdateTransform();
		return *this;
	}

	Transform& Transform::operator=(Transform&& t)
	{
		m_parent = t.m_parent;
		m_matrix = t.m_matrix;
		m_position = t.m_position;
		m_scale = t.m_scale;
		m_rotation = t.m_rotation;
		m_object = t.m_object;
		UpdateTransform();
		return *this;
	}

	void Transform::SetPosition(const Vector3& position)
	{
		m_position = position;
		UpdateTransform();
	}

	void Transform::SetScale(const Vector3& scale)
	{
		m_scale = scale;
		UpdateTransform();
	}

	void Transform::SetRotation(const Vector3& rotation)
	{
		m_rotation = rotation;
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

	void Transform::Translate(const Vector3& position)
	{
		m_position = m_position + position;
		UpdateTransform();
	}

	void Transform::Translate(float x, float y, float z)
	{
		Translate(Vector3(x, y, z));
	}

	void Transform::Scale(const Vector3& scale)
	{
		m_scale = m_scale + scale;
		UpdateTransform();
	}

	void Transform::Scale(float x, float y, float z)
	{
		Scale(Vector3(x, y, z));
	}

	void Transform::Rotate(const Vector3& radRotation)
	{
		m_rotation = m_rotation + radRotation;
		UpdateTransform();
	}

	void Transform::RotateDeg(const Vector3& rotation_)
	{
		Rotate(rotation_ * TO_RADIANS);
	}

	void Transform::Rotate(float radRoll, float radYaw, float radPitch)
	{
		Rotate(Vector3(radRoll, radYaw, radPitch));
	}

	void Transform::RotateDeg(float roll, float yaw, float pitch)
	{
		Rotate(Vector3(roll, yaw, pitch) * TO_RADIANS);
	}

	void Transform::SetParent(Transform* parent)
	{
		m_parent = parent;
		m_dirty = true;
	}

	Transform Transform::Combine(const Transform& other) const
	{
		return Transform(m_position + other.m_position, m_rotation + other.m_rotation, m_scale + other.m_scale, other.m_parent, other.m_object);
	}
}



