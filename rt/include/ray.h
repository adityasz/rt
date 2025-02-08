#ifndef RAY_H
#define RAY_H

#include "vec.h"

namespace rt
{
/**
 * @brief Represents a ray in 3D space.
 */
class ray {
	point orig;
	vec   dir;

public:
	/// @brief Default constructor, initializes origin and direction to 0.
	ray(): orig(0, 0, 0), dir(0, 0, 0) {}

	/**
	 * @brief Constructor that takes point of origin and direction.
	 *
	 * @param origin The point where the ray starts.
	 * @param direction The direction of the ray.
	 */
	ray(const point &origin, const vec &direction) : orig(origin), dir(direction) {}

	/**
	 * @brief Get the point of origin of the ray.
	 *
	 * @return The point of origin.
	 */
	[[nodiscard]] point origin() const { return orig; }

	/**
	 * @brief Get the direction of the ray.
	 *
	 * @return A `vec` object along the ray.
	 */
	[[nodiscard]] vec direction() const { return dir; }

	/**
	 * @brief Get coordinates of point at a distance from origin.
	 *
	 * @param t The distance from point of origin.
	 * @return A `point` object with the coordinates.
	 */
	[[nodiscard]] point at(double t) const { return orig + t * dir; }
};
}

#endif // RAY_H
