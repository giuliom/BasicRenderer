#include "Material.h"
#include <random>
#include "PrimitiveTypes.h"

namespace BasicRenderer
{
	const Color Material::MissingMaterialColor = { 1.0f, 0.0f, 1.0f };

	//TODO Use variadic functions for shading?
	Color Material::NormalShading(const World& scene, const Vector3& pos, const Vector3& normal) const
	{
		return (normal + 1.0f) * 0.5f;
	}

	Color Material::LitShading(const World& scene, const Vector3& pos, const Vector3& normal) const
	{ 
		return baseColor * (std::fmaxf(0.0f, Vector3::Dot(normal, scene.GetSun().GetDirection() * -1.f)) + emissive + scene.GetAmbientLightIntensity());
	}

	std::random_device rd;
	std::default_random_engine engine(rd());
	std::uniform_real_distribution<float> uniformDist(0, 1);

	Vector3 UniforSampleInHemisphere(const Vector3& normal)
	{
		const Vector3 nt = std::fabs(normal.x) > std::fabs(normal.y) 
			? Vector3(normal.z, 0.f, -normal.x) / sqrtf(normal.x * normal.x + normal.z * normal.z)
			: Vector3(0.f, -normal.z, normal.y) / sqrtf(normal.y * normal.y + normal.z * normal.z);

		const Vector3 nb = Vector3::CrossProduct(normal, nt);

		const float r1 = uniformDist(engine);
		const float r2 = uniformDist(engine);
		const float sinTheta = sqrtf(1.f - r1 * r1);
		const float phi = 2.f * PI * r2;
		const float x = sinTheta * cosf(phi);
		const float z = sinTheta * sinf(phi);
		const Vector3 sample(x, r1, z);

		return {
			sample.x * nb.x + sample.y * normal.x + sample.z * nt.x,
			sample.x * nb.y + sample.y * normal.y + sample.z * nt.y,
			sample.x * nb.z + sample.y * normal.z + sample.z * nt.z
		};
	}
}