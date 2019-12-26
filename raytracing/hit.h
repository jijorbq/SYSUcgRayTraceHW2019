#ifndef HIT_H
#define HIT_H

#include<glm/glm.hpp>
#include<vector>
#include "ray.h"
#define vec3 glm::vec3
#define vec2 glm::vec2

class material;

struct hit_record {
    float t;
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
            const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

class material  {
    public:
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, vec3& attenuation,
            ray& scattered) const = 0;
};



#endif // HIT_H
