#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "ray.h"
#include "vec.h"
#include "hittable.h"

namespace rt
{
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
	virtual bool scatter(const ray &r, const hit_record &rec, color &attenuation,
	                     ray &scattered) const = 0;
};

class lambertian : public material {
	color albedo;

public:
	/**
	 * @brief Constructor that takes albedo.
	 *
	 * @param albedo The albedo (fraction of light reflected).
	 */
	explicit lambertian(const color &albedo) : albedo(albedo) {}

	bool scatter(const ray &r, const hit_record &rec, color &attenuation,
	             ray &scattered) const override;
};

class metal : public material {
	color  albedo;
	double fuzz;

public:
	/**
	 * @brief Constructor that takes albedo and fuzziness.
	 *
	 * @param albedo The albedo (fraction of light reflected).
	 * @param fuzz The fuzziness.
	 */
	metal(const color &albedo, double fuzz)
	    : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

	bool scatter(const ray &r, const hit_record &rec, color &attenuation,
	             ray &scattered) const override;
};

class dielectric : public material {
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

public:
	/**
	 * @brief Constructor that takes the relative refractive index.
	 *
	 * @param n The relative refractive index.
	 */
	explicit dielectric(double n) : n_rel(n) {}

	bool scatter(const ray &r, const hit_record &rec, color &attenuation,
	             ray &scattered) const override;
};
}

#endif // MATERIAL_H
