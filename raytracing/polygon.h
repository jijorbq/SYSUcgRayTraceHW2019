#ifndef POLYGON_H
#define POLYGON_H

#include<glm/glm.hpp>
#include<cmath>
#include<vector>
#include<cstdio>
#include "ray.h"
#include "geometry.h"
#include "hit.h"
#define vec3 glm::vec3

class polygon: public hittable  {
    public:
        polygon() {}
        polygon(std::vector<vec3> pos, vec3 nor, vec3 col, material *m)
                    : points(pos), normal(nor), color(col), mat_ptr(m)  {}
        virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
        bool InPolygon(vec3 point) const;
        std::vector<vec3> points;
        vec3 normal;
        vec3 color;
        material *mat_ptr;
};

bool polygon::InPolygon(vec3 P)const {
    int len=points.size();
    for(int i=1;i+1<len;i++)
        if(InTriangle(P, points[0], points[i], points[i+1]))
            return true;
    return false;
}

bool polygon::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 B = points[0];
    vec3 A = r.origin();
    vec3 N = normal;
    vec3 D = r.direction();

    float temp = dot(B-A, N)/ dot(D, N);
    vec3 P = r.point_at_parameter(temp);

    //printf("%f %f %f\n",A.x, A.y, A.z);
    //printf("%f %f %f\n",D.x, D.y, D.z);
    //printf("%f %f %f\n",P.x, P.y, P.z);
    //system("pause");

    if(InPolygon(P))
    {
        printf("IN!");
        printf("%f %f %f\n",P.x, P.y, P.z);
        for(int i=0;i<points.size();i++)
            printf("%f %f %f\n",points[i].x, points[i].y, points[i].z);
        system("pause");
        if (temp < t_max && temp > t_min)
        rec.t = temp;
        rec.normal = N;
        rec.p = r.point_at_parameter(rec.t);
        rec.mat_ptr = mat_ptr;
        rec.col = color;
        return true;
    }
    return false;
}

#endif // POLYGON_H
