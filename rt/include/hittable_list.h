#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

namespace rt
{
class hittable_list final : public hittable {
public:
	std::vector<std::shared_ptr<hittable>> objects;

	hittable_list() = default;

	template<typename... Args>
	explicit hittable_list(Args &&...args)
	{
		add(std::forward<Args>(args)...);
	}

	void clear()
	{
		objects.clear();
	}

	template<typename... Args>
	void add(Args &&...args)
	{
		(objects.push_back(std::forward<Args>(args)), ...);
	}

	bool hit(const ray &r, interval I, hit_record &rec) const override
	{
		hit_record tmp;
		bool       hit_anything = false;

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
}


#endif // HITTABLE_LIST_H
