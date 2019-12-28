/*###################################################
##  文件描述：
##  定义球类库和撞击相关函数
#####################################################*/

#ifndef SPHERE_H
#define SPHERE_H

#include "ray.h"
#include "hit.h"
#include "utils.h"
#include "geometry.h"
#include "aabb.h"

/*###################################################
##  sphere类
##  球体类，记录球的相关信息
##  成员变量：
##  center：球心
##  radius：半径
##  color：光线
##  mat_ptr：材质
#####################################################*/
class sphere: public hittable  {
    public:
        sphere() {}
        sphere(vec3 cen, double r, vec3 col=vec3(0,0,0), material *m0=NULL, material *m1=NULL, material *m2=NULL){
            center = cen; radius = r; color = col; mat_ptr[0] = m0; mat_ptr[1] = m1; mat_ptr[2] = m2;
        }
        virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
        virtual bool bounding_box(float t0, float t1, aabb& box) const;
        vec3 center;
        vec3 color;
        double radius;
        material *mat_ptr[3];
};

/*###################################################
##  函数: bounding_box
##  函数描述： 求球的包围盒
##  参数描述：
##  t0：
##  t1：
##  box：包围盒
#####################################################*/
bool sphere::bounding_box(float t0, float t1, aabb& box) const {
    box = aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
    return true;
}

/*###################################################
##  函数: hit
##  函数描述： 判断射线到球是否有交，若有交则记录撞击记录
##  参数描述：
##  ray: 入射光线
##  t_min: 最小撞击距离
##  t_max: 最大撞击距离
##  hit_record: 撞击记录
#####################################################*/
bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    double a = dot(r.direction(), r.direction());
    double b = dot(oc, r.direction());
    double c = dot(oc, oc) - radius*radius;
    double discriminant = b*b - a*c;
    if (discriminant > 0) {
        double temp = (-b - sqrt(discriminant))/a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            for(int i=0;i<3;i++)
                rec.mat_ptr[i] = mat_ptr[i];
            rec.light = vec3(0,0,0);
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            for(int i=0;i<3;i++)
                rec.mat_ptr[i] = mat_ptr[i];
            rec.light = vec3(0,0,0);
            return true;
        }
    }
    return false;
}

#endif // SPHERE_H
