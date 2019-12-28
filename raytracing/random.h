#ifndef RANDOM_H
#define RANDOM_H


#include "utils.h"
#include "geometry.h"

/*###################################################
##  函数名称: random_double
##  函数描述： 产生随机数
##  参数描述： 无
#####################################################*/

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    static std::function<double()> rand_generator =
      std::bind(distribution, generator);
    return rand_generator();
}


/*###################################################
##  函数名称: random_in_unit_sphere
##  函数描述： 在漫反射的时候随机产生反射光线
##  参数描述： 无
#####################################################*/
vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0f*vec3(random_double(), random_double(), random_double()) - vec3(1,1,1);
    } while (p.length() >= 1.0);
    return p;
}

#endif // RANDOM_H
