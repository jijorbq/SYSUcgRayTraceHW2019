#ifndef RAY_H
#define RAY_H

#include "geometry.h"

class ray
{
public:
        ray() {}
        ray(const vec3 &a, const vec3 &b)
        {
                origin_point = a;
                ray_direction = b;
        }
        vec3 origin() const { return origin_point; }
        vec3 direction() const { return ray_direction; }
/*###################################################
##  函数名称: point_at_parameter
##  函数描述： 反射光线
##  参数描述： t：幅度
#####################################################*/
        vec3 point_at_parameter(float t) const { return origin_point + t * ray_direction; }
        vec3 origin_point;
        vec3 ray_direction;
};

#endif // RAY_H
