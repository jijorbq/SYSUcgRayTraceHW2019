#ifndef HITLIST_H
#define HITLIST_H

#include "utils.h"
#include "hit.h"
#include "geometry.h"

class hittable_list: public hittable {
    public:
        hittable_list() {}
        hittable_list(hittable **l, int n) {list = l; list_size = n; }
        virtual bool hit(
            const ray& r, double tmin, double tmax, hit_record& rec) const;
        virtual bool bounding_box(float t0, float t1, aabb& box) const{return true;}
        hittable **list;
        int list_size;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max,
                        hit_record& rec) const {

    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;
    for (int i = 0; i < list_size; i++) {
        if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

#endif // HITLIST_H
