/*###################################################
##  文件描述：
##  纹理库，定义纹理类和获取颜色的函数
#####################################################*/

#ifndef TEXTURE_H
#define TEXTURE_H

#include "utils.h"
#include "geometry.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

/*###################################################
##  texture类
##  纹理类，记录纹理的文件名和像素信息
##  成员变量：
##  fname：贴图文件名
##  w, h, c：buf矩阵的size
##  buf：像素信息缓存
#####################################################*/
class texture{
public:
    texture() {}
    texture(std::string _fname):fname(_fname){
        buf = stbi_load(fname.c_str(), &w, &h, &c, 0);
    }

/*###################################################
##  函数: getcolor
##  函数描述： 从纹理中获取颜色
##  参数描述：
##  a,b：贴图上的二维坐标
#####################################################*/
    vec3 getcolor(double a, double b){
        if (buf == NULL)
            return vec3(0,0,0);
        int pw = (int(a * w) % w + w) % w, ph = (int(b * h) % h + h) % h;
        int idx = ph * w * c + pw * c;
        double x = 1.0*buf[idx + 0], y = 1.0*buf[idx + 1], z = 1.0*buf[idx + 2];
        return vec3(x, y, z)/ 255.0;
    }
    // get color from png image
    std::string fname;
    unsigned char *buf;
    int w, h, c;
};

#endif // TEXTURE_H
