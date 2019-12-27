#ifndef POLYGON_H
#define POLYGON_H

#include "utils.h"
#include "ray.h"
#include "geometry.h"
#include "hit.h"
#include "aabb.h"
#define vec2 glm::vec2

class polygon: public hittable  {
    public:
        polygon() {}
        polygon(std::vector<vec3> pos, std::vector<vec2> ppos, vec3 nor, vec3 color,
                material *m0=NULL, material *m1=NULL, material *m2=NULL){
            points = pos; ppoints = ppos; normal = nor; light = color;
            mat_ptr[0] = m0; mat_ptr[1] = m1; mat_ptr[2] = m2;
        }
        virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
        virtual bool bounding_box(float t0, float t1, aabb& box) const;
        bool InPolygon(vec3 point) const;
        std::vector<vec3> points; // point in polygon
        std::vector<vec2> ppoints; // point in texture img
        vec3 light; // if is light
        vec3 normal; // normal
        material *mat_ptr[3]; // material
};

bool polygon::bounding_box(float t0, float t1, aabb &box) const{
    box = aabb(points[0], points[0]);
    for (int i=0; i<points.size(); ++i)
        for (int j=0; j<3; ++j)
            box._min[j]=ffmin(box._min[j], points[i][j]),
            box._max[j]=ffmax(box._max[j], points[i][j]);
    return true;
}

bool polygon::InPolygon(vec3 P)const {
    int len=points.size();
    for(int i=1;i+1<len;i++)
        if(InTriangle(P, points[0], points[i], points[i+1]))
            return true;
    return false;
}
// check if point in polygon

void swap(double &a, double &b){
    double tmp=a;a=b;b=tmp;
}

vec2 getpos(std::vector<vec3> points, std::vector<vec2> ppoints, vec3 P){
    int len=points.size();
    vec2 res=ppoints[0];

    for(int k=1;k+1<len;k++)
    {
        if(!InTriangle(P, points[0], points[k], points[k+1]))continue;
        double mat[3][3];
        vec3 P0=points[0];
        vec3 A=points[k]-P0, B=points[k+1]-P0, C=P-P0;
        const double eps=1e-6;
        mat[0][0]=A.x(); mat[0][1]=B.x(); mat[0][2]=C.x();
        mat[1][0]=A.y(); mat[1][1]=B.y(); mat[1][2]=C.y();
        mat[2][0]=A.z(); mat[2][1]=B.z(); mat[2][2]=C.z();

        for(int i=0;i<2;i++)
        {
            int pos=i;
            for(int j=i+1;j<3;j++)
                if(fabs(mat[i][j])>fabs(mat[pos][j]))pos=i;
            if(pos>i)
                for(int j=0;j<=len;j++)swap(mat[i][j],mat[pos][j]);
            if(fabs(mat[i][i])<eps){continue;}
            for(int j=i+1;j<3;j++)
            {
                double tmp=mat[j][i]/mat[i][i];
                for(int k=i;k<3;k++)mat[j][k]-=tmp*mat[i][k];
            }
        }
        float ans[2];int len=1;
        for(int i=1;i>=0;i--){
            if(fabs(mat[i][i])<eps)continue;
            double val=mat[i][2]/mat[i][i];
            for(int j=i-1;j>=0;j--)
                mat[j][2] -= val*mat[j][i];
            ans[len--]=float(val);
        }
        res+=(ppoints[k]-ppoints[0])*ans[0]+(ppoints[k+1]-ppoints[0])*ans[1];
        break;
    }
    return res;
}
// get pos in texture png by interpolation

bool polygon::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 A = r.origin();
    vec3 B = points[0];
    vec3 D = r.direction();
    vec3 N = normal;

    if(fabs(dot(D, N))<1e-8)return false; // parellel
    if(dot(D, N)>0)return false;
    double temp = dot(B-A, N)/ dot(D, N);
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
