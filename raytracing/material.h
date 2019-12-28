#ifndef MATERIAL_H
#define MATERIAL_H

#include "utils.h"
#include "hit.h"
#include "random.h"
#include "geometry.h"
#include "texture.h"

/*###################################################
##  函数: reflect
##  函数描述： 求反射光线
##  参数描述： v：入射光线；n：法线
#####################################################*/
vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n)*n;
}

class metal : public material {
    public:
        metal(const vec3& a, texture * t=NULL, double f=0.0f) : albedo(a), tex(t) {
            fuzz =min(1,f);
        }
/*###################################################
##  函数: scatter
##  函数描述： 判断是否有反射光线
##  参数描述： r_in：入射光线，rec：存储每次碰撞发生的信息，attenuation：衰减，scattered：反射
#####################################################*/
        virtual bool scatter(const ray& r_in, const hit_record& rec,
                             vec3& attenuation, ray& scattered) const {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
            attenuation = albedo;
            if(tex!=NULL)
                attenuation += tex->getcolor(rec.pos.x, rec.pos.y);
            return (dot(scattered.direction(), rec.normal) > 0);
        }
        vec3 albedo;
        double fuzz;
        texture * tex;
};
// 镜面反射

class lambertian : public material {
    public:
/*###################################################
同上
#####################################################*/
        lambertian(const vec3& a, texture * t=NULL) : albedo(a), tex(t) {}
        virtual bool scatter(const ray& r_in, const hit_record& rec,
                             vec3& attenuation, ray& scattered) const {
            vec3 target = rec.p + rec.normal + random_in_unit_sphere();
            scattered = ray(rec.p, target-rec.p);
            attenuation = albedo;
            if(tex!=NULL)
                attenuation += tex->getcolor(rec.pos.x, rec.pos.y);
            return true;
        }
        vec3 albedo;
        texture * tex;
};
// 漫反射

/*###################################################
##  函数: refract
##  函数描述： 判断是否发生折射
##  参数描述： v:入射，n：法线，ni_over_nt：系数，refracted：折射光线
#####################################################*/
bool refract(const vec3& v, const vec3& n, double ni_over_nt, vec3& refracted) {
    vec3 uv = unit_vector(v);
    double dt = dot(uv, n);
    double discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
    if (discriminant > 0) {
        refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
        return true;
    }
    else
        return false;
}
/*###################################################
##  函数: schlick
##  函数描述： 近似计算折射方向
##  参数描述： cosine：入射方向，ref_idx：系数
#####################################################*/
double schlick(double cosine, double ref_idx) {
    double r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine),5);
}

class dielectric : public material {
    public:
        dielectric(double ri) : ref_idx(ri) {}
        virtual bool scatter(const ray& r_in, const hit_record& rec,
                             vec3& attenuation, ray& scattered) const {
            vec3 outward_normal;
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            double ni_over_nt;
            attenuation = vec3(1.0, 1.0, 1.0);
            vec3 refracted;

            double reflect_prob;
            double cosine;

            if (dot(r_in.direction(), rec.normal) > 0) {
                 outward_normal = -rec.normal;
                 ni_over_nt = ref_idx;
                 cosine = ref_idx * dot(r_in.direction(), rec.normal)
                        / r_in.direction().length();
            }
            else {
                 outward_normal = rec.normal;
                 ni_over_nt = 1.0 / ref_idx;
                 cosine = -dot(r_in.direction(), rec.normal)
                        / r_in.direction().length();
            }

            if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
               reflect_prob = schlick(cosine, ref_idx);
            }
            else {
               reflect_prob = 1.0;
            }

            if (random_double() < reflect_prob) {
               scattered = ray(rec.p, reflected);
            }
            else {
               scattered = ray(rec.p, refracted);
            }

            return true;
        }

        double ref_idx;
};
// 折射

#endif // MATERIAL_H
