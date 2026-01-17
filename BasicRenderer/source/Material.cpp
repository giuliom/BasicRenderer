#include "Global.h"
#include "Material.h"
#include "PrimitiveTypes.h"

namespace BasicRenderer
{
	const Color Material::MissingMaterialColor = { 1.0f, 0.0f, 1.0f };

	//TODO Use variadic functions for shading?
	Color Material::NormalShading(const RenderState& state, const Vector3& pos, const Vector3& normal) const
	{
		(void)state;
		(void)pos;
		return {(normal + 1.0f) * 0.5f};
	}

	Color Material::LitShading(const RenderState& state, const Vector3& pos, const Vector3& normal) const
	{ 
		(void)pos;
		const EnvironmentSettings& environment = state.m_environmentSettings;
		return baseColor * (std::fmaxf(0.0f, Vector3::Dot(normal, environment.m_sun.GetDirection() * -1.f)) + emissive + environment.m_ambientLightIntensity);
	}

	Vector3 UniformSampleInHemisphere(const Vector3& normal)
	{
		const Vector3 nt = std::fabs(normal.x) > std::fabs(normal.y) 
			? Vector3(normal.z, 0.f, -normal.x) / sqrtf(normal.x * normal.x + normal.z * normal.z)
			: Vector3(0.f, -normal.z, normal.y) / sqrtf(normal.y * normal.y + normal.z * normal.z);

		const Vector3 nb = Vector3::CrossProduct(normal, nt);

		const float r1 = UniformDist();
		const float r2 = UniformDist();
		const float sinTheta = sqrtf(1.f - r1 * r1);
		const float phi = TWO_PI * r2;
		const float x = sinTheta * cosf(phi);
		const float z = sinTheta * sinf(phi);

		return {
			x * nb.x + r1 * normal.x + z * nt.x,
			x * nb.y + r1 * normal.y + z * nt.y,
			x * nb.z + r1 * normal.z + z * nt.z
		};
	}
}