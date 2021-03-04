#pragma once

#include "Matrix4.h"
#include "Vector3.h"

namespace BasicRenderer
{
	class SceneObject;

	constexpr float PI = 3.1415926f;
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
		Transform* m_parent = nullptr; //TODO
		SceneObject* m_object = nullptr;
		mutable bool m_dirty = true;

	public:

		Matrix4 m_matrix;
		Matrix4 m_inverse;

		Vector3 m_forward;
		Vector3 m_right;
		Vector3 m_up;

	protected:

		inline Matrix4 GetTranslationMatrix(const Vector3& position);
		inline Matrix4 GetScaleMatrix(const Vector3& scale);
		inline Matrix4 GetRotationMatrix(const Vector3& rotation);

		inline void UpdateTransform()
		{
			m_matrix = GetTranslationMatrix(m_position) * GetRotationMatrix(m_rotation) * GetScaleMatrix(m_scale);
			m_inverse = m_matrix.Inverse();
			m_forward = Vector3(m_matrix.x1, m_matrix.y1, m_matrix.z1).Normalize();
			m_up = Vector3(m_matrix.x2, m_matrix.y2, m_matrix.z2).Normalize();
			m_right = Vector3(-m_matrix.x3, -m_matrix.y3, -m_matrix.z3).Normalize();
			m_dirty = true;
		}


	public:
		Transform() 
			: m_matrix(), m_position(Vector3::Zero()), m_scale(Vector3::One()), m_rotation(Vector3::Zero()), m_parent(nullptr), m_object(nullptr) {}
		Transform(const Transform& t) 
			: m_matrix(t.m_matrix), m_position(t.m_position), m_scale(t.m_scale), m_rotation(t.m_rotation), m_parent(t.m_parent), m_object(t.m_object) { UpdateTransform();	}
		Transform(Transform&& t) noexcept 
			: m_matrix(t.m_matrix), m_position(t.m_position), m_scale(t.m_scale), m_rotation(t.m_rotation), m_parent(t.m_parent), m_object(t.m_object) { UpdateTransform();	}
		Transform(SceneObject* obj) 
			: m_matrix(), m_position(Vector3::Zero()), m_scale(Vector3::One()), m_rotation(Vector3::Zero()), m_parent(nullptr), m_object(obj) {}
		Transform(const Vector3& pos, const Vector3& scl, const Vector3& rot, Transform* parent = nullptr, SceneObject* obj = nullptr);
		~Transform() {}

		Transform& operator=(const Transform& t);
		Transform& operator=(Transform&& t);

		inline const Vector3& GetPosition() const { return m_position; }
		inline const Vector3& GetScale()	const { return m_scale; }
		inline const Vector3& GetRotation() const { return m_rotation; }

		inline bool isDirty() const { return m_dirty; }
		inline void SetDirty(bool dirty) { m_dirty = dirty; }

		inline const Vector3& Forward() const { return m_forward; }
		inline const Vector3& Right()	const { return m_right; }
		inline const Vector3& Up()		const { return m_up; }

		void SetPosition(const Vector3& position);
		void SetScale(const Vector3& scale);
		void SetRotation(const Vector3& rotation);
		void SetPosition(float x, float y, float z);
		void SetScale(float x, float y, float z);
		void SetRotation(float roll, float yaw, float pitch);

		void Translate(const Vector3& position);
		void Translate(float x, float y, float z);
		void Scale(const Vector3& scale);
		void Scale(float x, float y, float z);
		void Rotate(const Vector3& radRotation);
		void RotateDeg(const Vector3& rotation);
		void Rotate(float radRoll, float radYaw, float radPitch);
		void RotateDeg(float roll, float yaw, float pitch);

		Transform* GetParent() const { return m_parent; }
		void SetParent(Transform* par);
		Transform Combine(const Transform& other) const;

		inline const Matrix4& GetMatrix()						const { return m_matrix; }
		inline const Matrix4& GetInverseMatrix()				const { return m_inverse; }
		inline Vector4 GetTransformedVector(const Vector4& v)	const { return m_matrix * v; }

	};
}
