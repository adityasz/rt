/**
 * @file	rtweekend.h
 * @author	Aditya Singh
 * @copyright	Copyright (C) 2023 Aditya Singh
 * @date	December 2023
 */

#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>

const double infinity = std::numeric_limits<double>::infinity();
const double pi       = 3.141'592'653'589'793'238'462'643'383;

inline double degrees_to_radians(double degrees)
{
	return degrees * pi / 180.0;
}

#include "interval.h"
#include "ray.h"
#include "vec.h"

#endif //RTWEEKEND_H
