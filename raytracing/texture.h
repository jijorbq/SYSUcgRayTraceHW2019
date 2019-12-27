#ifndef TEXTURE_H
#define TEXTURE_H

#include "utils.h"
#include "geometry.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

class texture{
public:
    texture() {}
    texture(std::string _fname):fname(_fname){
        buf = stbi_load(fname.c_str(), &w, &h, &c, 0);
    }
    vec3 getcolor(double a, double b){
        if (buf == NULL)
            return vec3(0,0,0);
        int pw = (int(a * w) % w + w) % w, ph = (int(b * h) % h + h) % h;
        int idx = ph * w * c + pw * c;
        double x = 1.0f*buf[idx + 0], y = 1.0f*buf[idx + 1], z = 1.0f*buf[idx + 2];
        return vec3(x, y, z)/ 255.0f;
    }
    // get color from png image
    std::string fname;
    unsigned char *buf;
    int w, h, c;
};

#endif // TEXTURE_H
