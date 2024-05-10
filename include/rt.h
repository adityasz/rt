// Copyright (C) 2023 Aditya Singh

#ifndef RT_H
#define RT_H

#include <cmath>
#include <limits>
#include <memory>
#include <random>

const double infinity = std::numeric_limits<double>::infinity();
const double pi       = 3.141'592'653'589'793'238'462'643'383;

inline double degrees_to_radians(double degrees)
{
	return degrees * pi / 180.0;
}

inline double random_double(double min = 0.0, double max = 1.0)
{
	std::uniform_real_distribution<double> distribution(min, max);
	thread_local static std::mt19937 generator;
	return distribution(generator);
}

#include "interval.h"
#include "ray.h"
#include "vec.h"
#include "log_levels.h"
#include "print.h"

#endif // RT_H
