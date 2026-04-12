#include "Transform.h"
#include <cmath>
#include "SceneObject.h"

namespace BasicRenderer
{
	Matrix4 Transform::GetTranslationMatrix(const Vector3& position) noexcept
	{
		return Matrix4(1.0f, 0.0f, 0.0f, position.x,
			0.0f, 1.0f, 0.0f, position.y,
			0.0f, 0.0f, 1.0f, position.z,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	Matrix4 Transform::GetScaleMatrix(const Vector3& scale) noexcept
	{
		return Matrix4(scale.x, 0.0f, 0.0f, 0.0f,
			0.0f, scale.y, 0.0f, 0.0f,
			0.0f, 0.0f, scale.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	// Right-hand
	Matrix4 Transform::GetRotationMatrix(const Vector3& rotation) noexcept
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

	Transform::Transform(const Vector3& pos, const Vector3& scl, const Vector3& rot, SceneObject* obj)
	{
		m_position = pos;
		m_scale = scl;
		m_rotation = rot;
		m_object = obj;

		UpdateTransform();
	}

	Transform& Transform::operator=(const Transform& t) noexcept
	{
		m_children = t.m_children;
		m_matrix = t.m_matrix;
		m_position = t.m_position;
		m_scale = t.m_scale;
		m_rotation = t.m_rotation;
		m_object = t.m_object;
		UpdateTransform();
		return *this;
	}

	Transform& Transform::operator=(Transform&& t) noexcept
	{
		m_children = t.m_children;
		m_matrix = t.m_matrix;
		m_position = t.m_position;
		m_scale = t.m_scale;
		m_rotation = t.m_rotation;
		m_object = t.m_object;
		UpdateTransform();
		return *this;
	}

	void Transform::SetPosition(const Vector3& position) noexcept
	{
		m_position = position;
		UpdateTransform();
	}

	void Transform::SetScale(const Vector3& scale) noexcept
	{
		m_scale = scale;
		UpdateTransform();
	}

	void Transform::SetRotation(const Vector3& rotation) noexcept
	{
		m_rotation = rotation;
		UpdateTransform();
	}

	void Transform::SetPosition(float x, float y, float z) noexcept
	{
		SetPosition(Vector3(x, y, z));
	}

	void Transform::SetScale(float x, float y, float z) noexcept
	{
		SetScale(Vector3(x, y, z));
	}

	void Transform::SetRotation(float roll, float yaw, float pitch) noexcept
	{
		SetRotation(Vector3(roll, yaw, pitch));
	}

	void Transform::Translate(const Vector3& position) noexcept
	{
		m_position = m_position + position;
		UpdateTransform();
	}

	void Transform::Translate(float x, float y, float z) noexcept
	{
		Translate(Vector3(x, y, z));
	}

	void Transform::Scale(const Vector3& scale) noexcept
	{
		m_scale = m_scale + scale;
		UpdateTransform();
	}

	void Transform::Scale(float x, float y, float z) noexcept
	{
		Scale(Vector3(x, y, z));
	}

	void Transform::Rotate(const Vector3& radRotation) noexcept
	{
		m_rotation = m_rotation + radRotation;
		UpdateTransform();
	}

	void Transform::RotateDeg(const Vector3& rotation_) noexcept
	{
		Rotate(rotation_ * TO_RADIANS);
	}

	void Transform::Rotate(float radRoll, float radYaw, float radPitch) noexcept
	{
		Rotate(Vector3(radRoll, radYaw, radPitch));
	}

	void Transform::RotateDeg(float roll, float yaw, float pitch) noexcept
	{
		Rotate(Vector3(roll, yaw, pitch) * TO_RADIANS);
	}

	void Transform::AddChild(Transform& child)
	{
		m_children.insert(&child);
		child.m_parent = this;
	}

	Transform Transform::Combine(const Transform& other) const
	{
		return Transform(m_position + other.m_position, m_scale + other.m_scale, m_rotation + other.m_rotation, other.m_object);
	}

	const Matrix4& Transform::GetWorldMatrix() const noexcept
	{
		if (m_parent)
		{
			m_world = m_parent->GetWorldMatrix() * m_matrix;
		}
		else
		{
			m_world = m_matrix;
		}
		return m_world;
	}

	Vector3 Transform::GetWorldPosition() const noexcept
	{
		const Matrix4& wm = GetWorldMatrix();
		return Vector3(wm.x4, wm.y4, wm.z4);
	}
}



