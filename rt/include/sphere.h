// Copyright (C) 2023 Aditya Singh

#ifndef SPHERE_H
#define SPHERE_H

#include "vec.h"
#include "ray.h"
#include "hittable.h"

namespace rt
{
/// @brief This class represents a sphere.
class sphere final : public hittable {
	point  center;
	double radius;
	std::shared_ptr<material> mat;

public:
	/**
	 * @brief Constructor that takes center and radius.
	 *
	 * @param center A `point` object representing the center.
	 * @param radius The radius.
	 * @param mat
	 */
	sphere(const point &center, double radius, const std::shared_ptr<material> &mat)
		: center(center), radius(radius), mat(mat) {}

	/**
	 * @brief Find whether a ray hits the sphere.
	 *
	 * @param r The ray.
	 * @param t The interval (t_min, t_max) in which the hit is counted.
	 * @param rec The hit record.
	 * @return True if the ray hits the sphere, false otherwise.
	 */
	bool hit(const ray &r, interval t, hit_record &rec) const override;
};
}


#endif // SPHERE_H
