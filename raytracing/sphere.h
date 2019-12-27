#ifndef SPHERE_H
#define SPHERE_H

#include "ray.h"
#include "hit.h"
#include "utils.h"
#include "geometry.h"
#include "aabb.h"

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

bool sphere::bounding_box(float t0, float t1, aabb& box) const {
    box = aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
    return true;
}

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
