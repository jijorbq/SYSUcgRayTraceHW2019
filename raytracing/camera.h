#ifndef CAMERA_H
#define CAMERA_H

#include<glm/glm.hpp>
#include<math.h>
#include"ray.h"
#include"random.h"
#include"geometry.h"
#define vec3 glm::vec3

vec3 random_in_unit_disk() {
    vec3 p;
    do {
        p = 2.0f*vec3(random_double(),random_double(),0) - vec3(1,1,0);
    } while (dot(p,p) >= 1.0);
    return p;
}

class camera {
    public:
        camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect,
               float aperture, float focus_dist) {
            lens_radius = aperture / 2;
            float theta = vfov*M_PI/180;
            float half_height = tan(theta/2);
            float half_width = aspect * half_height;
            origin = lookfrom;
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);
            lower_left_corner = origin
                              - half_width * focus_dist * u
                              - half_height * focus_dist * v
                              - focus_dist * w;
            horizontal = 2*half_width*focus_dist*u;
            vertical = 2*half_height*focus_dist*v;


            printf("--- %f %f %f\n", w.x, w.y, w.z);
            printf("--- %f %f %f\n", u.x, u.y, u.z);
            printf("--- %f %f %f\n", v.x, v.y, v.z);
            printf("%f %f %f %f %f\n",theta, half_height, half_width, focus_dist, lens_radius);

            printf("--- %f %f %f\n", origin.x, origin.y, origin.z);
            printf("--- %f %f %f\n", lower_left_corner.x, lower_left_corner.y, lower_left_corner.z);
            printf("--- %f %f %f\n", horizontal.x, horizontal.y, horizontal.z);
            printf("--- %f %f %f\n", vertical.x, vertical.y, vertical.z);
        }

        ray get_ray(float s, float t) {
            vec3 rd = lens_radius*random_in_unit_disk();
            vec3 offset = u * rd.x + v * rd.y;
            //printf("+++ %f %f %f\n", rd.x, rd.y, rd.z);
            //printf("+++ %f %f %f\n", offset.x, offset.y, offset.z);
            return ray(origin + offset,
                       lower_left_corner + s*horizontal + t*vertical
                           - origin - offset);
        }

        vec3 origin;
        vec3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        float lens_radius;
};

#endif // CAMERA_H
