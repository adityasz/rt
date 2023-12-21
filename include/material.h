/**
 * @file      material.h
 * @author    Aditya Singh
 * @copyright Copyright (C) 2023 Aditya Singh
 * @date      December 2023
 */

#ifndef MATERIAL_H
#define MATERIAL_H

#include "rt.h"
#include "color.h"
#include "hittable.h"

class hit_record;

class material {
public:
	virtual ~material() = default;

	virtual bool scatter(const ray &r, const hit_record &rec,
			     color &attenuation, ray &scattered) const = 0;
};

class lambertian : public material {
public:
	lambertian(const color &a) : albedo(a) {}

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
	metal(const color &a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	bool scatter(const ray &r, const hit_record &rec,
		     color &attenuation, ray &scattered) const override
	{
		vec reflected = reflect(unit_vector(r.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz*random_unit_vector());
		attenuation = albedo;
		return true;
	}

private:
	color  albedo;
	double fuzz;
};

// FIXME: Refractions not good
class dielectric : public material {
public:
	dielectric(double n) : n_rel(n) {}

	bool scatter(const ray &r, const hit_record &rec,
	             color &attenuation, ray &scattered) const override
	{
		attenuation = {1.0, 1.0, 1.0};
		double n1_over_n2 = rec.front_face ? 1.0 / n_rel : n_rel;

		vec    v         = unit_vector(r.direction());
		double cos_theta = fmin(dot(-v, rec.normal), 1.0);
		double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);
		bool   tir       = n1_over_n2 * sin_theta > 1.0;

		if (tir || reflectance(cos_theta, n1_over_n2) > random_double())
			v = reflect(v, rec.normal);
		else
			v = refract(v, rec.normal, n1_over_n2);

		scattered = ray(rec.p, v);
		return true;
	}

private:
	double n_rel;
	static double reflectance(double cosine, double n_rel)
	{
		double r0 = (1 - n_rel) / (1 + n_rel);
		r0 = r0 * r0;
		return r0 + (1 - r0) * std::pow((1 - cosine), 5);
	}
};

#endif //MATERIAL_H
