// Copyright (C) 2023 Aditya Singh

#ifndef MATERIAL_H
#define MATERIAL_H

#include "rt.h"
#include "color.h"
#include "hittable.h"

class hit_record;

class material {
public:
	virtual ~material() = default;

	/**
	 * @brief Scatter a ray.
	 *
	 * @param r The incident ray.
	 * @param rec The hit record.
	 * @param attenuation The attenuation.
	 * @param scattered Will be set to the scattered ray.
	 */
	virtual bool scatter(const ray &r, const hit_record &rec,
	                     color &attenuation, ray &scattered) const = 0;
};

class lambertian : public material {
public:
	/**
	 * @brief Constructor that takes albedo.
	 *
	 * @param albedo The albedo (fraction of light reflected).
	 */
	lambertian(const color &albedo) : albedo(albedo) {}

	bool scatter(const ray &r, const hit_record &rec,
	             color &attenuation, ray &scattered) const override
	{
		vec scatter_direction = rec.normal + random_unit_vector();
		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;
		scattered   = ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}

private:
	color albedo;
};

class metal : public material {
public:
	/**
	 * @brief Constructor that takes albedo and fuzziness.
	 *
	 * @param albedo The albedo (fraction of light reflected).
	 * @param fuzz The fuzziness.
	 */
	metal(const color &albedo, double fuzz)
	    : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

	bool scatter(const ray &r, const hit_record &rec,
		     color &attenuation, ray &scattered) const override
	{
		vec reflected = reflect(r.direction(), rec.normal);
		reflected = unit_vector(reflected) + fuzz * random_unit_vector();
		scattered = ray(rec.p, reflected);
		attenuation = albedo;
		return dot(scattered.direction(), rec.normal) > 0;
	}

private:
	color  albedo;
	double fuzz;
};

class dielectric : public material {
public:
	/**
	 * @brief Constructor that takes the relative refractive index.
	 *
	 * @param n The relative refractive index.
	 */
	dielectric(double n) : n_rel(n) {}

	bool scatter(const ray &r, const hit_record &rec,
	             color &attenuation, ray &scattered) const override
	{
		attenuation = {1.0, 1.0, 1.0};
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

private:
	double n_rel;
	/**
	 * @brief Schlick's approximation for reflectance.
	 *
	 * @param cosine The cosine of the angle between the incident ray
	 *               and the normal.
	 * @param n_rel The relative refractive index.
	 * @return The reflectance.
	 */
	static double reflectance(double cosine, double n_rel)
	{
		double r0 = (1 - n_rel) / (1 + n_rel);
		r0 = r0 * r0;
		return r0 + (1 - r0) * std::pow((1 - cosine), 5);
	}
};

#endif // MATERIAL_H
