/**
 * @file      interval.h
 * @author    Aditya Singh
 * @copyright Copyright (C) 2023 Aditya Singh
 * @date      December 2023
 */

#ifndef INTERVAL_H
#define INTERVAL_H

#include "rt.h"

class interval {
public:
	double min;
	double max;

	interval() : min(+infinity), max(-infinity) {}
	interval(double min, double max) : min(min), max(max) {}

	bool contains(double x) const
	{
		return min <= x && x <= max;
	}

	bool surrounds(double x) const
	{
		return min < x && x < max;
	}

	double clamp(double x) const
	{
		if (x < min)
			return min;
		if (x > max)
			return max;
		return x;
	}

	static const interval empty;
	static const interval universe;
};

const static interval empty(+infinity, -infinity);
const static interval universe(+infinity, -infinity);

#endif //INTERVAL_H
