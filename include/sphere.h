// Copyright (C) 2023 Aditya Singh

#ifndef SPHERE_H
#define SPHERE_H

#include "vec.h"
#include "ray.h"
#include "hittable.h"

/// @brief This class represents a sphere.
class sphere : public hittable {
public:
	/**
	 * @brief Constructor that takes center and radius.
	 *
	 * @param center A `point` object representing the center.
	 * @param radius The radius.
	 */
	sphere(point center, double radius,const std::shared_ptr<material> &mat)
		: center(center), radius(radius), mat(mat) {}

	/**
	 * @brief Find whether a ray hits the sphere.
	 *
	 * @param r The ray.
	 * @param t The interval (t_min, t_max) in which the hit is counted.
	 * @param rec The hit record.
	 * @return True if the ray hits the sphere, false otherwise.
	 */
	bool hit(const ray &r, interval t, hit_record &rec) const override
	{
		vec oc = r.origin() - center;

		double a = dot(r.direction(), r.direction());
		double b = dot(r.direction(), oc);
		double c = dot(oc, oc) - radius*radius;
		double discriminant = b*b - a*c;

		if (discriminant < 0)
			return false;

		double sqrt_disc = std::sqrt(discriminant);
		double root = (-b - sqrt_disc) / a;
		if (!t.surrounds(root)) {
			root = (-b + sqrt_disc) / a;
			if (!t.surrounds(root))
				return false;
		}

		rec.t   = root;
		rec.p   = r.at(root);
		rec.mat = mat;
		vec outward_normal = (rec.p - center) / radius;
		rec.set_face_normal(r, outward_normal);

		return true;
	}

private:
	point  center;
	double radius;
	std::shared_ptr<material> mat;
};


#endif // SPHERE_H
