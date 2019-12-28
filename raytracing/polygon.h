/*###################################################
##  文件说明：
##  多边形类定义库，定义多边形类及其撞击相关函数
#####################################################*/

#ifndef POLYGON_H
#define POLYGON_H

#include "utils.h"
#include "ray.h"
#include "geometry.h"
#include "hit.h"
#include "aabb.h"
#define vec2 glm::vec2

/*###################################################
##  polygon类
##  多边形类，记录多边形的相关信息
##  成员变量：
##  points：多边形的点坐标
##  ppoints：多边形的点在贴图上的坐标
##  light：光线
##  normal：法向量
##  mat_ptr：材质
#####################################################*/
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

/*###################################################
##  函数: bounding_box
##  函数描述： 求多边形的包围盒
##  参数描述：
##  t0：
##  t1：
##  box：包围盒
#####################################################*/
bool polygon::bounding_box(float t0, float t1, aabb &box) const{
    box = aabb(points[0], points[0]);
    for (int i=0; i<points.size(); ++i)
        for (int j=0; j<3; ++j)
            box._min[j]=ffmin(box._min[j], points[i][j]),
            box._max[j]=ffmax(box._max[j], points[i][j]);
    return true;
}

/*###################################################
##  函数: InPolygon
##  函数描述： 判断点P是否在多边形内
##  参数描述：
##  P: 交点坐标
#####################################################*/
bool polygon::InPolygon(vec3 P)const {
    int len=points.size();
    for(int i=1;i+1<len;i++)
        if(InTriangle(P, points[0], points[i], points[i+1]))
            return true;
    return false;
}
// check if point in polygon

/*###################################################
##  函数: swap
##  函数描述： 交换两个数字
##  参数描述：
##  a,b: 待交换的两个数字
#####################################################*/
void swap(double &a, double &b){
    double tmp=a;a=b;b=tmp;
}

/*###################################################
##  函数: getpos
##  函数描述： 根据多边形每个点的三维坐标和其对应的二维坐标获取
##  贴图上的映射坐标
##  参数描述：
##  points: 多边形点坐标
##  ppoints: 多边形点在贴图上对应坐标
##  P: 交点
#####################################################*/
vec2 getpos(std::vector<vec3> points, std::vector<vec2> ppoints, vec3 P){
    int len=points.size();
    vec2 res=ppoints[0];
    //return res;

    for(int k=1;k+1<len;k++)
    {
        if(!InTriangle(P, points[0], points[k], points[k+1]))continue;
        // 点不在三角形内
        const double eps=1e-6;
        vec3 P0=points[0];
        vec3 A=points[k]-P0, B=points[k+1]-P0, C=P-P0;
        double mat[3][3]={
            {A.x(), B.x(), C.x()},
            {A.y(), B.y(), C.y()},
            {A.z(), B.z(), C.z()}
        };

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
        // 高斯消元
        res+=(ppoints[k]-ppoints[0])*ans[0]+(ppoints[k+1]-ppoints[0])*ans[1];
        break;
    }
    return res;
}
// get pos in texture png by interpolation

/*###################################################
##  函数: hit
##  函数描述： 判断射线到多边形是否有交，若有交则记录撞击记录
##  参数描述：
##  ray: 入射光线
##  t_min: 最小撞击距离
##  t_max: 最大撞击距离
##  hit_record: 撞击记录
#####################################################*/
bool polygon::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 A = r.origin();
    vec3 B = points[0];
    vec3 D = r.direction();
    vec3 N = normal;
    //vec3 N = cross(points[1]-B, points[2]-B);
    //N.make_unit_vector();
    //if(dot(D, N)>0)N=-N;
    if(fabs(dot(D, N))<1e-5)return false; // parellel
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
