// Copyright (C) 2023 Aditya Singh

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

static const interval empty(+infinity, -infinity);
static const interval universe(+infinity, -infinity);

#endif // INTERVAL_H
