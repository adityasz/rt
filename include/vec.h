// Copyright (C) 2023 Aditya Singh

#ifndef VEC_H
#define VEC_H

#include <cmath>
#include <iostream>

/**
 * @brief Represents a 3D vector in Euclidean space.
 *
 * The `vec` class stores three double-precision floating-point values
 * representing the x, y, and z components of a vector. It provides various
 * operations for vector manipulation, including addition, subtraction, scalar
 * multiplication, dot product, cross product, length calculation, and unit
 * vector construction.
 */
class vec {
public:
	/// @brief The pixels of components of the vector.
	double v[3];

	/// @brief Default constructor, initializes all components to 0.
	vec() : v{0, 0, 0} {}

	/**
	 * @brief Constructor that takes individual components.
	 *
	 * @param x The x-component.
	 * @param y The y-component.
	 * @param z The z-component.
	 */
	vec(double x, double y, double z) : v{x, y, z} {}

	/**
	 * @brief Get the x-component.
	 *
	 * @return The x-component of the vector.
	 */
	double x() const { return v[0]; }

	/**
	 * @brief Get the y-component.
	 *
	 * @return The y-component of the vector.
	 */
	double y() const { return v[1]; }

	/**
	 * @brief Get the z-component.
	 *
	 * @return The z-component of the vector.
	 */
	double z() const { return v[2]; }

	/**
	 * @brief Unary minus operator.
	 *
	 * @return A new vector with all components negated.
	 */
	vec operator-() const { return {-v[0], -v[1], -v[2]}; }

	/**
	 * @brief Get the i-th component.
	 *
	 * @param i The index (0 for x, 1 for y, 2 for z).
	 * @return The corresponding component.
	 */
	double operator[](int i) const { return v[i]; }

	/**
	 * @brief Get a reference to the i-th component.
	 *
	 * @param i The index (0 for x, 1 for y, 2 for z).
	 * @return A reference to the corresponding component.
	 */
	double &operator[](int i) { return v[i]; }

	/**
	 * @brief In-place addition with another vector.
	 *
	 * @param u The vector to add.
	 * @return A reference to the modified vector.
	 */
	vec &operator+=(const vec &u)
	{
		v[0] += u.v[0];
		v[1] += u.v[1];
		v[2] += u.v[2];
		return *this;
	}

	/**
	 * @brief In-place multiplication with scalar.
	 *
	 * @param t The scalar to multiply with.
	 * @return A reference to the modified vector.
	 */
	vec &operator*=(double t)
	{
		v[0] *= t;
		v[1] *= t;
		v[2] *= t;
		return *this;
	}

	/**
	 * @brief In-place division with scalar.
	 *
	 * @param t The scalar to divide by.
	 * @return A reference to the modified vector.
	 */
	vec &operator/=(double t)
	{
		return *this *= 1/t;
	}

	/**
	 * @brief Get Euclidean length.
	 *
	 * @return The square root of the sum of squared components.
	 */
	double length() const
	{
		return std::sqrt(length_squared());
	}

	/**
	 * @brief Get square of the Euclidean length.
	 *
	 * @return The sum of squared components.
	 */
	double length_squared() const
	{
		return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
	}

	bool near_zero() const
	{
		double e = 1e-8;
		return (fabs(v[0]) < e) && (fabs(v[1]) < e) && (fabs(v[2]) < e);
	}

	/**
	 * @brief Dot product with another vector.
	 *
	 * @param u The other vector.
	 * @return The sum of component-wise products.
	 */
	double dot(const vec &u)
	{
		return v[0]*u[0] + v[1]*u[1] + v[2]*u[2];
	}

	/**
	 * @brief Cross product with another vector.
	 *
	 * @param u The other vector.
	 * @return The cross product of this vector with u.
	 */
	vec cross(const vec &u)
	{
		return {u.v[1] * v[2] - u.v[2] * v[1],
		        u.v[2] * v[0] - u.v[0] * v[2],
		        u.v[0] * v[1] - u.v[1] * v[0]};
	}

	static vec random(double min = 0.0, double max = 1.0)
	{
		return {random_double(min, max),
		        random_double(min, max),
		        random_double(min, max)};
	}
};

/**
 * @brief Represents a point in 3D space.
 *
 * The `point` alias is a convenient shorthand for representing points in 3D
 * space. It uses the same underlying data and operations as the `vec` class.
 */
using point = vec;

/**
 * @brief Overloads the `<<` operator for printing `vec` objects.
 *
 * This operator allows directly printing a `vec` object to an output stream.
 *
 * @param out The output stream (e.g., `std::cout`).
 * @param v The `vec` object to print.
 *
 * @return The output stream with the printed `vec` components.
 */
inline std::ostream &operator<<(std::ostream &out, const vec &v)
{
	return out << v.v[0] << ' ' << v.v[1] << ' ' << v.v[2];
}

/**
 * @brief Performs element-wise addition of two vectors.
 *
 * @param u The first operand vector.
 * @param v The second operand vector.
 *
 * @return A new `vec` object containing the sum of `u` and `v`.
 */
inline vec operator+(const vec &u, const vec &v)
{
	return {u.v[0] + v.v[0], u.v[1] + v.v[1], u.v[2] + v.v[2]};
}

/**
 * @brief Performs element-wise subtraction of two vectors.
 *
 * @param u the first operand vector.
 * @param v the second operand vector.
 *
 * @return A new `vec` object containing the difference of `u` and `v`.
 */
inline vec operator-(const vec &u, const vec &v)
{
	return {u.v[0] - v.v[0], u.v[1] - v.v[1], u.v[2] - v.v[2]};
}

/**
 * @brief Performs element-wise multiplication of two vectors.
 *
 * @param u the first operand vector.
 * @param v the second operand vector.
 *
 * @return A new `vec` object containing the element-wise product of
 * `u` and `v`.
 */
inline vec operator*(const vec &u, const vec &v)
{
	return {u.v[0] * v.v[0], u.v[1] * v.v[1], u.v[2] * v.v[2]};
}

/**
 * @brief Multiply vector by a scalar.
 *
 * @param t The scalar to multiply.
 * @param v The vector.
 * @return A new `vec` object t*v.
 */
inline vec operator*(double t, const vec &v)
{
	return {t * v.v[0], t * v.v[1], t * v.v[2]};
}

/**
 * @brief Multiply scalar by vector.
 *
 * @param v The scalar.
 * @param t The vector to multiply.
 * @return A new `vec` object v*t.
 */
inline vec operator*(const vec &v, double t)
{
	return t * v;
}

/**
 * @brief Divide vector by scalar.
 *
 * @param v The vector.
 * @param t The scalar to divide by.
 *
 * @return A new `vec` object v/t.
 */
inline vec operator/(vec v, double t)
{
	return (1 / t) * v;
}

/**
 * @brief Get dot product of two vectors.
 *
 * @param u The first operand vector.
 * @param v The second operand vector.
 *
 * @return The dot product of u and v.
 */
inline double dot(const vec &u, const vec &v)
{
	return u.v[0] * v.v[0] + u.v[1] * v.v[1] + u.v[2] * v.v[2];
}

/**
 * @brief Get cross product of two vectors.
 *
 * @param u The first operand vector.
 * @param v The second operand vector.
 *
 * @return A new `vec` object equal to the cross product of u and v.
 */
inline vec cross(const vec &u, const vec &v)
{
	return {u.v[1] * v.v[2] - u.v[2] * v.v[1],
	        u.v[2] * v.v[0] - u.v[0] * v.v[2],
	        u.v[0] * v.v[1] - u.v[1] * v.v[0]};
}

/**
 * @brief Get unit vector along a vector.
 *
 * @param v The vector.
 *
 * @return A new `vec` object of unit length point along v.
 */
inline vec unit_vector(vec v)
{
	return v / v.length();
}

/**
 * @return Get random vector in unit sphere.
 */
inline vec random_in_unit_sphere()
{
	while (true) {
		vec v = vec::random(-1, 1);
		if (v.length_squared() < 1)
			return v;
	}
}

/**
 * @brief Get a random unit vector.
 *
 * The reason we are normalising a random vector in a unit sphere instead of,
 * say, normalising an arbitrary random vector in a unit cube is because there
 * is a larger probability for a random vector to point along the corners of the
 * unit cube, and hence the random unit vector isn't uniformly random.
 *
 * @return A random unit vector.
 */
inline vec random_unit_vector()
{
	return unit_vector(random_in_unit_sphere());
}

/**
 * @brief Get a random unit vector on a hemisphere.
 * @param normal The normal at a point on the sphere.
 * @return A vector pointing away from the sphere from that point.
 */
inline vec random_on_hemisphere(const vec &normal)
{
	vec v = random_unit_vector();
	if (dot(v, normal) > 0)
		return v;
	return -v;
}

inline vec reflect(const vec &v, const vec &n)
{
	return v - 2 * dot(v, n) * n;
}

inline vec refract(const vec &v, const vec &n, double n1_over_n2)
{
	double cos_theta = fmin(dot(-v, n), 1.0);
	vec    r_perp    = n1_over_n2 * (v + cos_theta*n);
	vec    r_prll    = -std::sqrt(fabs(1.0 - r_perp.length_squared())) * n;
	return r_perp + r_prll;
}

#endif // VEC_H
