#ifndef POLYGON_H
#define POLYGON_H

#include<glm/glm.hpp>
#include<cmath>
#include<iostream>
#include<vector>
#include<cstdio>
#include "ray.h"
#include "geometry.h"
#include "hit.h"
#define vec3 glm::vec3
#define vec2 glm::vec2

class polygon: public hittable  {
    public:
        polygon() {}
        polygon(std::vector<vec3> pos, std::vector<vec2> ppos, vec3 nor, vec3 color,
                material *m0=NULL, material *m1=NULL, material *m2=NULL){
            points = pos; ppoints = ppos; normal = nor; light = color;
            mat_ptr[0] = m0; mat_ptr[1] = m1; mat_ptr[2] = m2;
        }
        virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
        bool InPolygon(vec3 point) const;
        std::vector<vec3> points; // point in polygon
        std::vector<vec2> ppoints; // point in texture img
        vec3 light; // if is light
        vec3 normal; // normal
        material *mat_ptr[3]; // material
};

bool polygon::InPolygon(vec3 P)const {
    int len=points.size();
    for(int i=1;i+1<len;i++)
        if(InTriangle(P, points[0], points[i], points[i+1]))
            return true;
    return false;
}
// check if point in polygon

void swap(float &a, float &b){
    float tmp=a;a=b;b=tmp;
}

vec2 getpos(std::vector<vec3> points, std::vector<vec2> ppoints, vec3 P){
    int len=points.size();
    vec2 res(0,0);
    float mat[len+2][len+2];
    const float eps=1e-6;

    for(int i=0;i<len;i++)mat[0][i]=points[i].x;
    for(int i=0;i<len;i++)mat[1][i]=points[i].y;
    for(int i=0;i<len;i++)mat[2][i]=points[i].z;
    mat[0][len]=P.x; mat[1][len]=P.y; mat[2][len]=P.z;
    // init

    for(int i=0;i<3;i++)
    {
        int pos=i;
        for(int j=i+1;j<3;j++)
            if(fabs(mat[i][j])>fabs(mat[pos][j]))pos=i;
        if(pos>i)
            for(int j=0;j<=len;j++)swap(mat[i][j],mat[pos][j]);
        if(fabs(mat[i][i])<eps)continue;
        for(int j=i+1;j<3;j++)
        {
            float tmp=mat[j][i]/mat[i][i];
            for(int k=i;k<=len;k++)mat[j][k]-=tmp*mat[i][k];
        }
    }
    for(int i=2;i>=0;i--){
        float ans=mat[i][len]/mat[i][i];
        for(int j=i-1;j>=0;j--)
            mat[j][len] -= ans*mat[j][i];
        res+=ppoints[i]*ans;
    }
    // gauss
    return res;
}
// get pos in texture png by interpolation

bool polygon::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 A = r.origin();
    vec3 B = points[0];
    vec3 D = r.direction();
    vec3 N = normal;

    if(fabs(dot(D, N))<1e-6)return false; // parellel
    float temp = dot(B-A, N)/ dot(D, N);
    vec3 P = r.point_at_parameter(temp);

    if(InPolygon(P))
    {
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.normal = N;
            rec.p = P;
            rec.light = light;
            rec.pos = getpos(points, ppoints, P);
            for(int i=0;i<3;i++)
                rec.mat_ptr[i] = mat_ptr[i];
            return true;
        }
    }
    return false;
}

#endif // POLYGON_H
