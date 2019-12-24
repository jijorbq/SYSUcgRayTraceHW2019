#ifndef GEOMETRY_H
#define GEOMETRY_H

#include<glm/glm.hpp>
#include<math.h>
#define vec3 glm::vec3

inline float length(vec3 v)
{
    return sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
}

inline vec3 unit_vector(vec3 v) {
    return v/length(v);
}

inline float Area2(vec3 P0, vec3 P1, vec3 P2){
    return length(cross(P1-P0, P2-P0));
}

inline bool InTriangle(vec3 P, vec3 P0, vec3 P1, vec3 P2){
    float area1 = Area2(P, P0, P1);
    float area2 = Area2(P, P1, P2);
    float area3 = Area2(P, P2, P0);
    return fabs(area1+area2+area3-Area2(P0, P1, P2))<1e-6;
}

#endif // GEOMETRY_H
