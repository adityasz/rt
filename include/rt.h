/**
 * @file	rt.h
 * @author	Aditya Singh
 * @copyright	Copyright (C) 2023 Aditya Singh
 * @date	December 2023
 */

#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#define DEBUG_STATUS    1
#define DEBUG_FULL      2

#if DEBUG >= DEBUG_STATUS
#define STATUS_MSG(x)                                       \
        do {                                                \
                std::clog << x;                             \
        } while (0)
#define DEBUG_MSG(x)                                        \
        do {                                                \
                if constexpr (DEBUG >= DEBUG_FULL) {        \
                        std::cerr << x;                     \
                }                                           \
        } while (0)
#else
#define STATUS_MSG(x)	do {} while(0)
#define DEBUG_MSG(x)	do {} while(0)
#endif

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

inline double random_double()
{
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

#include "interval.h"
#include "ray.h"
#include "vec.h"

#endif //RTWEEKEND_H
