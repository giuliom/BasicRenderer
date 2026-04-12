#pragma once

#include <unordered_set>

#include "Global.h"
#include "Matrix4.h"
#include "Vector3.h"

namespace BasicRenderer
{
	class SceneObject;

	constexpr float PDF = 1.f / (2.f * PI);
	constexpr float TO_RADIANS = PI / 180.f;

	// TODO Quaternions
	class Transform
	{
		friend class SceneObject;

	protected:

		Vector3 m_position;
		Vector3 m_scale;
		Vector3 m_rotation;
		Transform* m_parent = nullptr;
		std::unordered_set<Transform*> m_children;
		SceneObject* m_object = nullptr;
		mutable bool m_dirty = true;

	public:

		Matrix4 m_matrix;
		Matrix4 m_inverse;
		mutable Matrix4 m_world;

		Vector3 m_forward;
		Vector3 m_right;
		Vector3 m_up;

	protected:

		Matrix4 GetTranslationMatrix(const Vector3& position) noexcept;
		Matrix4 GetScaleMatrix(const Vector3& scale) noexcept;
		Matrix4 GetRotationMatrix(const Vector3& rotation) noexcept;

		inline void UpdateTransform() noexcept
		{
			m_matrix = GetTranslationMatrix(m_position) * GetRotationMatrix(m_rotation) * GetScaleMatrix(m_scale);
			m_inverse = m_matrix.Inverse();
			m_forward = Vector3(m_matrix.x1, m_matrix.y1, m_matrix.z1).Normalize();
			m_up = Vector3(m_matrix.x2, m_matrix.y2, m_matrix.z2).Normalize();
			m_right = Vector3(-m_matrix.x3, -m_matrix.y3, -m_matrix.z3).Normalize();
			m_dirty = true;

			for (auto* child : m_children)
				child->m_dirty = true;
		}


	public:
		Transform() 
			: m_position(Vector3::Zero()), m_scale(Vector3::One()), m_rotation(Vector3::Zero()), m_object(nullptr), m_matrix(), m_inverse(), m_world() {}
		Transform(const Transform& t) 
			: m_position(t.m_position), m_scale(t.m_scale), m_rotation(t.m_rotation), m_object(t.m_object), m_matrix(t.m_matrix), m_inverse(t.m_inverse), m_world(t.m_world) { UpdateTransform();	}
		Transform(Transform&& t) noexcept 
			: m_position(t.m_position), m_scale(t.m_scale), m_rotation(t.m_rotation), m_object(t.m_object), m_matrix(t.m_matrix), m_inverse(t.m_inverse), m_world(t.m_world) { UpdateTransform();	}
		Transform(SceneObject* obj) 
			: m_position(Vector3::Zero()), m_scale(Vector3::One()), m_rotation(Vector3::Zero()), m_object(obj), m_matrix(), m_inverse(), m_world() {}
		Transform(const Vector3& pos, const Vector3& scl, const Vector3& rot, SceneObject* obj = nullptr);
		~Transform() {}

		Transform& operator=(const Transform& t) noexcept;
		Transform& operator=(Transform&& t) noexcept;

		inline const Vector3& GetPosition() const noexcept { return m_position; }
		inline const Vector3& GetScale()	const noexcept { return m_scale; }
		inline const Vector3& GetRotation() const noexcept { return m_rotation; }

		inline bool isDirty() const noexcept { return m_dirty; }
		inline void SetDirty(bool dirty) noexcept { m_dirty = dirty; }

		inline const Vector3& Forward() const noexcept { return m_forward; }
		inline const Vector3& Right()	const noexcept { return m_right; }
		inline const Vector3& Up()		const noexcept { return m_up; }

		void SetPosition(const Vector3& position) noexcept;
		void SetScale(const Vector3& scale) noexcept;
		void SetRotation(const Vector3& rotation) noexcept;
		void SetPosition(float x, float y, float z) noexcept;
		void SetScale(float x, float y, float z) noexcept;
		void SetRotation(float roll, float yaw, float pitch) noexcept;

		void Translate(const Vector3& position) noexcept;
		void Translate(float x, float y, float z) noexcept;
		void Scale(const Vector3& scale) noexcept;
		void Scale(float x, float y, float z) noexcept;
		void Rotate(const Vector3& radRotation) noexcept;
		void RotateDeg(const Vector3& rotation) noexcept;
		void Rotate(float radRoll, float radYaw, float radPitch) noexcept;
		void RotateDeg(float roll, float yaw, float pitch) noexcept;

		SceneObject* GetObject() noexcept { return m_object; }
		const SceneObject* GetObject() const noexcept { return m_object; }
		const std::unordered_set<Transform*>& GetChildren() const noexcept { return m_children; }
		const Transform* GetParent() const noexcept { return m_parent; }
		void AddChild(Transform& child);
		Transform Combine(const Transform& other) const;

		const Matrix4& GetWorldMatrix() const noexcept;
		Vector3 GetWorldPosition() const noexcept;

		inline const Matrix4& GetMatrix()						const noexcept { return m_matrix; }
		inline const Matrix4& GetInverseMatrix()				const noexcept { return m_inverse; }
		inline Vector4 GetTransformedVector(const Vector4& v)	const noexcept { return m_matrix * v; }

	};
}
