#ifndef RAY_H
#define RAY_H

#include<glm/glm.hpp>
#define vec3 glm::vec3

class ray
{
    public:
        ray() {}
        ray(const vec3& a, const vec3& b) { origin_point = a; ray_direction = b; }
        vec3 origin() const{ return origin_point; }
        vec3 direction() const{ return ray_direction; }
        vec3 point_at_parameter(float t) const { return origin_point + t*ray_direction; }
        vec3 origin_point;
        vec3 ray_direction;
};

#endif // RAY_H
