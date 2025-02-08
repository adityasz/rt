#include "material.h"
#include "color.h"

using namespace rt;

bool lambertian::scatter(const ray &r, const hit_record &rec,
                         color &attenuation, ray &scattered) const
{
	vec scatter_direction = rec.normal + random_unit_vector();
	if (scatter_direction.near_zero())
		scatter_direction = rec.normal;
	scattered   = ray(rec.p, scatter_direction);
	attenuation = albedo;
	return true;
}

bool metal::scatter(const ray &r, const hit_record &rec, color &attenuation,
                    ray &scattered) const
{
	vec reflected = reflect(r.direction(), rec.normal);
	reflected     = unit_vector(reflected) + fuzz * random_unit_vector();
	scattered     = ray(rec.p, reflected);
	attenuation   = albedo;
	return dot(scattered.direction(), rec.normal) > 0;
}

bool dielectric::scatter(const ray &r, const hit_record &rec,
                         color &attenuation, ray &scattered) const
{
	attenuation       = { 1.0, 1.0, 1.0 };
	double n1_over_n2 = rec.front_face ? 1.0 / n_rel : n_rel;

	vec    unit_dir  = unit_vector(r.direction());
	double cos_theta = fmin(dot(-unit_dir, rec.normal), 1.0);
	double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);
	bool   tir       = n1_over_n2 * sin_theta > 1.0;

	vec dir;
	if (tir || reflectance(cos_theta, n1_over_n2) > random_double())
		dir = reflect(unit_dir, rec.normal);
	else
		dir = refract(unit_dir, rec.normal, n1_over_n2);

	scattered = ray(rec.p, dir);
	return true;
}
