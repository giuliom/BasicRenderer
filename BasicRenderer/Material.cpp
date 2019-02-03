#include "Material.h"
#include <random>

bool Material::Scatter(const Ray & rayIn, const HitResult & hit, Vector3& attenuation, Ray & scattered, const World& scene, Color(Material::*shading)(const World& w, const Vector3& pos, const Vector3& nrml)) const
{
	switch (type)
	{
	case Material::Type::METALLIC:
	{
		Vector3 reflected = Vector3::Reflect(rayIn.direction, hit.normal);
		scattered = Ray(hit.pos, reflected + RandomPointInUnitSphere() * (1.f - metallic));
		attenuation = (hit.material->*shading)(scene, hit.pos, hit.normal);
		return (Vector3::Dot(scattered.direction, hit.normal) > 0);
	}
		break;
	case Material::Type::DIELECTRIC:
	{
		Vector3 outNormal;
		Vector3 reflected = Vector3::Reflect(rayIn.direction, hit.normal);
		float ni_nt;
		attenuation = { 1.f, 1.f, 1.f };
		Vector3 refracted;
		float reflectionProb;
		float cos;
		if (Vector3::Dot(rayIn.direction, hit.normal) > 0.f)
		{
			outNormal = hit.normal * -1.f;
			ni_nt = refractiveIndex;
			cos = (Vector3::Dot(rayIn.direction, hit.normal) * refractiveIndex); // / rayIn.direction.Length(); == 1.f
		}
		else
		{
			outNormal = hit.normal;
			ni_nt = 1.f / refractiveIndex;
			cos = -(Vector3::Dot(rayIn.direction, hit.normal)); // / rayIn.direction.Length(); == 1.f
		}

		if (Refract(rayIn.direction, outNormal, ni_nt, refracted))
		{
			reflectionProb = Schlick(cos);
		}
		else
		{
			reflectionProb = 1.f;
		}

		if (UnitRandf() < reflectionProb)
		{
			scattered = Ray(hit.pos, reflected);
		}
		else
		{
			scattered = Ray(hit.pos, refracted);
		}

		return true;
	}
		break;
	default:
	{
		//TODO fix proper attenuation and color model to avoid canceling colors
		Vector3 target = hit.pos + hit.normal + RandomPointInUnitSphere();
		scattered = Ray(hit.pos, target - hit.pos);
		attenuation = (hit.material->*shading)(scene, hit.pos, hit.normal);
		return true;
	}
		break;
	}
}
bool Material::Refract(const Vector3 & v, const Vector3 & normal, float ni_nt, Vector3 & refracted) const
{
	Vector3 uv = v.Normalize();
	float dt = Vector3::Dot(uv, normal);
	float discriminant = 1.f - ni_nt * ni_nt * (1.f - dt * dt);
	if (discriminant > 0.f)
	{
		refracted = (uv - normal * dt) * ni_nt - normal * sqrtf(discriminant);
		return true;
	}
	return false;
}
//TODO Use variadic functions for shading?
Color Material::NormalShading(const World & scene, const Vector3 & pos, const Vector3 & normal)
{
	return (normal + 1.0f) * 0.5f;
}

Color Material::LitShading(const World & scene, const Vector3 & pos, const Vector3 & normal)
{ //TODO normals are flipped compared to rasterizer and check proper lighting formula
	return baseColor;// *std::fmaxf(0.01f, Vector3::Dot(normal, scene.sun.GetDirection() * -1.f)) * scene.sun.intensity;
}

std::random_device rd;
std::default_random_engine engine(rd());
std::normal_distribution<float> gaussian(0.5f, 1.0f);

Vector3 RandomPointInUnitSphere()
{
	float x = gaussian(engine);
	float y = gaussian(engine);
	float z = gaussian(engine);

	if (x <= 0.f && y <= 0.f && z <= 0.f)
	{
		y = 1.0f;
	}

	return Vector3(x, y, z).Normalize();
}
