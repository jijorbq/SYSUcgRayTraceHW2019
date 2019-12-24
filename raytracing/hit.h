#ifndef HIT_H
#define HIT_H

#include<glm/glm.hpp>
#include<math.h>
#include "ray.h"
#define vec3 glm::vec3

class material;

struct hit_record {
    float t;
    vec3 p;
    vec3 normal;
    vec3 col;
    material *mat_ptr;
};

class hittable  {
    public:
        virtual bool hit(
            const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

class material  {
    public:
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, vec3& attenuation,
            ray& scattered) const = 0;
};



#endif // HIT_H
