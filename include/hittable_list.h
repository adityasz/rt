/**
 * @file	hittable_list.h
 * @author	Aditya Singh
 * @copyright	Copyright (C) 2023 Aditya Singh
 * @date	December 2023
 */

#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <memory>
#include <vector>

class hittable_list : public hittable {
public:
	std::vector<std::shared_ptr<hittable>> objects;

	hittable_list() = default;
	explicit hittable_list(const std::shared_ptr<hittable> &object)
	{
		add(object);
	}

	void clear()
	{
		objects.clear();
	}

	void add(const std::shared_ptr<hittable> &object)
	{
		objects.push_back(object);
	}

	bool hit(const ray &r, interval I, hit_record &rec) const override
	{
		hit_record tmp;
		bool       hit_anything   = false;

		for (auto &object : objects) {
			if (object->hit(r, I, tmp)) {
				hit_anything = true;
				I.max = tmp.t;
				rec = tmp;
			}
		}

		return hit_anything;
	}
};


#endif //HITTABLE_LIST_H
