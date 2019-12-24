#ifndef RANDOM_H
#define RANDOM_H

#include <functional>
#include <random>
#include<glm/glm.hpp>
#define vec3 glm::vec3

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    static std::function<double()> rand_generator =
      std::bind(distribution, generator);
    return rand_generator();
}

vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0f*vec3(random_double(), random_double(), random_double()) - vec3(1,1,1);
    } while (length(p) >= 1.0);
    return p;
}

#endif // RANDOM_H
