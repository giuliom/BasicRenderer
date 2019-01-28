#include "Material.h"
#include <random>

bool Material::Scatter(const Ray & rayIn, const HitResult & hit, Vector3& attenuation, Ray & scattered, const World& scene, Color(Material::*shading)(const World& w, const Vector3& pos, const Vector3& nrml)) const
{
	/* Diffuse
	Vector3 target = hit.pos + hit.normal + RandomPointInUnitSphere();
	scattered = Ray(hit.pos, target - hit.pos);
	attenuation = diffuseColor;
	*/

	//Handle shading function in a different way because recursion is not correct for NormalShading

	Vector3 reflection = Vector3::Reflect(rayIn.direction.Normalize(), hit.normal);
	scattered = Ray(hit.pos, reflection);
	attenuation = (hit.material->*shading)(scene, hit.pos, hit.normal);
	return (Vector3::Dot(scattered.direction, hit.normal) > 0);
}

Color Material::NormalShading(const World & scene, const Vector3 & pos, const Vector3 & normal)
{
	return (normal + 1.0f) * 0.5f;
}

Color Material::LitShading(const World & scene, const Vector3 & pos, const Vector3 & normal)
{
	return diffuseColor * std::fmaxf(0.0f, Vector3::Dot(normal, scene.sun.GetDirection())) * scene.sun.intensity;
}

Vector3 RandomPointInUnitSphere()
{
	std::default_random_engine engine;
	std::normal_distribution<float> gaussian(0.5f, 1.0f);

	float x = gaussian(engine);
	float y = gaussian(engine);
	float z = gaussian(engine);

	if (x <= 0.f && y <= 0.f && z <= 0.f)
	{
		y = 1.0f;
	}

	return Vector3(x, y, z).Normalize();
}
