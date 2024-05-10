// Copyright (C) 2023 Aditya Singh

#ifndef HITTABLE_H
#define HITTABLE_H

#include "vec.h"
#include "ray.h"

class material;

/// @brief This class represents hits.
class hit_record {
public:
	point                     p;
	vec                       normal;
	std::shared_ptr<material> mat;
	double                    t;
	bool                      front_face;

	/**
	 * @brief Sets the hit record normal vector.
	 *
	 * @param r The incident ray.
	 * @param outward_normal Must have unit length.
	 */
	void set_face_normal(const ray &r, const vec &outward_normal)
	{
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

/// @brief This class represents anything that a ray might hit.
class hittable {
public:
	virtual ~hittable() = default;

	virtual bool hit(const ray &r, interval t, hit_record &rec) const = 0;
};


#endif // HITTABLE_H
