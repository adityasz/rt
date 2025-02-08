#include "vec.h"
#include "ray.h"
#include "sphere.h"

using namespace rt;

bool sphere::hit(const ray &r, interval t, hit_record &rec) const
{
	vec oc = center - r.origin();

	double a = dot(r.direction(), r.direction());
	double h = dot(r.direction(), oc);
	double c = dot(oc, oc) - radius * radius;

	double discriminant = h * h - a * c;
	if (discriminant < 0)
		return false;

	double sqrt_disc = std::sqrt(discriminant);
	double root      = (h - sqrt_disc) / a;
	if (!t.surrounds(root)) {
		root = (h + sqrt_disc) / a;
		if (!t.surrounds(root))
			return false;
	}

	rec.t              = root;
	rec.p              = r.at(root);
	rec.mat            = mat;
	vec outward_normal = (rec.p - center) / radius;
	rec.set_face_normal(r, outward_normal);

	return true;
}
