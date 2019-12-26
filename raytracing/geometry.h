#ifndef GEOMETRY_H
#define GEOMETRY_H

#include<glm/glm.hpp>
#include<math.h>
#include<iostream>
#define vec3 glm::vec3

inline double length(vec3 v)
{
    return sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
}

inline vec3 unit_vector(vec3 v) {
    return v/float(length(v));
}

inline double Area2(vec3 P0, vec3 P1, vec3 P2){
    return length(cross(P1-P0, P2-P0));
}

inline bool InTriangle(vec3 P, vec3 P0, vec3 P1, vec3 P2){
    double area1 = Area2(P, P0, P1);
    double area2 = Area2(P, P1, P2);
    double area3 = Area2(P, P2, P0);
    return fabs(area1+area2+area3-Area2(P0, P1, P2))<5e-3;
}

inline double ang(vec3 A, vec3 B){
    return acos(dot(A,B)/length(A)/length(B));
}


#endif // GEOMETRY_H
