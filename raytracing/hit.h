#ifndef HIT_H
#define HIT_H

#include "utils.h"
#include "ray.h"
#include "geometry.h"
#include "aabb.h"
#define vec2 glm::vec2

class material;

struct hit_record {
    double t;
    vec3 p;
    vec3 normal;
    vec3 light;
    vec3 color;
    vec2 pos;
    material *mat_ptr[3];
};

class hittable  {
    public:
        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
        virtual bool bounding_box(float t0, float t1, aabb& box) const=0;
};

class material  {
    public:
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, vec3& attenuation,
            ray& scattered) const = 0;
};



#endif // HIT_H
