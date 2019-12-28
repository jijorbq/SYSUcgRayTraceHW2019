#ifndef HITLIST_H
#define HITLIST_H

#include "utils.h"
#include "hit.h"
#include "geometry.h"

/*###################################################
##  hittable_list
##  一个列表，继承hittable类，存储所有图形
#####################################################*/
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

/*###################################################
##  函数: hit
##  函数描述： 暴力寻找撞击点，按顺序枚举所有图元找到最近的撞击点
##  参数描述：
##  ray: 入射光线
##  t_min: 最小撞击距离
##  t_max: 最大撞击距离
##  hit_record: 撞击记录
#####################################################*/
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
