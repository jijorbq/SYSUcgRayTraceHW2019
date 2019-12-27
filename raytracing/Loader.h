#ifndef LOADER_H
#define LOADER_H

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include "hitlist.h"
#include "polygon.h"
#include "material.h"
#include "myglwidget.h"
#include "sphere.h"
#include "texture.h"
#include<glm/glm.hpp>

#define vec3 glm::vec3
#define vec2 glm::vec2
#define MAXLEN 100000

std::vector<std::vector<float>>tomap;//需要映射的二维数组


//###########################
//###MAT:material,用于存放mtl文件中的不同
//###
//###########################
struct MAT
{
    std::vector<float>Ka;//环境反射
    std::vector<float>Kd;//环境反射
    std::vector<float>Ks;//环境反射
    //不同的mtl不同，有些mtl文件没有下面所说的三种变量，所以给了初始值。
    float Ns=0;//反射指数
    float Ni=0;//折射率
    float d=0;//衰退率
    int illum=0;
    texture *Tex;
};

void translate(float *matrix, float *poi)
{
    float tmp[3];
    for(int i=0;i<3;i++){
        tmp[i]=poi[i];poi[i]=0;
    }
    for(int i=0;i<3;i++)
        for(int j=0;j<4;j++)
            poi[i]+=matrix[i+4*j]*tmp[i];
}
// translate by matrix

hittable *random_scene() {
    int n = 500;
    hittable **list = new hittable*[n+1];
    list[0] =  new sphere(vec3(0,-1000,0), 1000, vec3(0, 0, 0),  new lambertian(vec3(0.5, 0.5, 0.5)));
    int i = 1;
    for (int a = -11; a < 11; a+=1) {
        for (int b = -11; b < 11; b+=1) {
            float choose_mat = random_double();
            vec3 center(a+0.9*random_double(),0.2,b+0.9*random_double());
            if ((center-vec3(4,0.2,0)).length() > 0.9) {
                if (choose_mat < 0.8) {  // diffuse
                    list[i++] = new sphere(center, 0.2, vec3(0, 0, 0),
                        new lambertian(vec3(random_double()*random_double(),
                                            random_double()*random_double(),
                                            random_double()*random_double()), NULL
                        )
                    );
                }
                else if (choose_mat < 0.95) { // metal
                    list[i++] = new sphere(center, 0.2, vec3(0, 0, 0),
                            new metal(vec3(0.5*(1 + random_double()),
                                           0.5*(1 + random_double()),
                                           0.5*(1 + random_double())),
                                           NULL, 0.5*random_double())
                                     );
                }
                else {  // glass
                    list[i++] = new sphere(center, 0.2, vec3(0, 0, 0), new dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new sphere(vec3(0, 1, 0), 1.0, vec3(0, 0, 0), new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, vec3(0, 0, 0), new lambertian(vec3(0.4, 0.2, 0.1), NULL));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, vec3(0, 0, 0), new metal(vec3(0.7, 0.6, 0.5), NULL, 0.0));

    return new hittable_list(list,i);
}


hittable *combine(std::string objfile,std::string mtlfile, std::string basicfile, float matrix[])
{
    int len=0;
    hittable **list = new hittable*[MAXLEN];

    std::cout<<objfile<<std::endl;

    std::map<std::string, MAT>Materials;
    std::ifstream infile(mtlfile);
    std::string sline;//每一行
    if (! infile.is_open())
    {
        std::cout << "Error   opening file";
    }
    std::string nowmaterial;
    float t1,t2,t3;
    int t4;
    while(getline(infile,sline))
    {
        std::istringstream in(sline);
        std::string prefix, fname;
        in>>prefix;
        if(prefix=="newmtl")
        {
            in>>nowmaterial;
        }
        else if(prefix=="Ns")
        {
            in>>t1;
            Materials[nowmaterial].Ns=t1;
        }
        else if(prefix=="Ni")
        {
            in>>t1;
            Materials[nowmaterial].Ni=t1;
        }
        else if(prefix=="d")
        {
            in>>t1;
            Materials[nowmaterial].d=t1;
        }
        else if(prefix=="illum")
        {
            in>>t4;
            Materials[nowmaterial].illum=t4;
        }
        else if(prefix=="Ka")
        {
            in>>t1>>t2>>t3;
            Materials[nowmaterial].Ka={t1,t2,t3};
        }
        else if(prefix=="Kd")
        {
            in>>t1>>t2>>t3;
            Materials[nowmaterial].Kd={t1,t2,t3};
        }
        else if(prefix=="Ks"||prefix=="Tf")
        {
            in>>t1>>t2>>t3;
            Materials[nowmaterial].Ks={t1,t2,t3};
        }
        else if(prefix=="map_Kd")
        {
            in>>fname;
            std::string file_name = basicfile+'/'+fname;
            std::cout<<file_name<<std::endl;
            Materials[nowmaterial].Tex = new texture(file_name);
        }
    }


    //obj-loader
    std::vector<std::vector<float>>vArr;//存放点的二维数组
    std::vector<std::vector<float>>vnArr;//存放法线的二维数组
    std::vector<std::vector<float>>vtArr;//存放纹理的二维数组
    std::vector<std::vector<int>>fvArr;//存放面顶点的二维数组
    std::vector<std::vector<int>>ftArr;//存放面纹理的二维数组
    std::vector<std::vector<int>>fnArr;//存放面法线的二维数组


    std::ifstream file(objfile);
    if (! infile.is_open())
    {
        std::cout << "Error   opening file"<<std::endl;
    }
    while(getline(file,sline))
    {
        //std::cout<<sline<<std::endl;
        std::istringstream in(sline);
        std::string prefix;
        in>>prefix;
        if(prefix=="v")
        {
            float f1,f2,f3;
            in>>f1>>f2>>f3;
            std::vector<float>temp={f1,f2,f3};
            vArr.push_back(temp);
        }
        else if(prefix=="vt")
        {
            float f1,f2;
            in>>f1>>f2;
            std::vector<float>temp={f1,f2};
            vtArr.push_back(temp);
        }
        else if(prefix=="vn")
        {
            float f1,f2,f3;
            in>>f1>>f2>>f3;
            std::vector<float>temp={f1,f2,f3};
            vnArr.push_back(temp);

        }
    }

    std::cout<<"!!!!!"<<vArr.size()<<std::endl;
    std::cout<<"!!!!!"<<vnArr.size()<<std::endl;
    std::cout<<"!!!!!"<<vtArr.size()<<std::endl;

    std::ifstream newfile(objfile);
    if (! infile.is_open())
    {
        std::cout << "Error   opening file"<<std::endl;
    }
    while(getline(newfile,sline))
    {

        std::istringstream in(sline);
        std::string prefix;
        in>>prefix;
        if(prefix=="usemtl")
        {
            in>>nowmaterial;
        }
        else if(prefix=="f")
        {
            //std::cout<<sline<<std::endl;
            std::vector<int>vt;
            std::vector<int>tt;
            std::vector<int>nt;
            while(1)
            {
                std::string m;
                in>>m;
                if(m=="")
                    break;
                int a=0;
                int k=0;
                while(m[k]!='/')
                {
                    a=a*10+m[k]-'0';
                    k++;
                }
                vt.push_back(a);
                a=0;
                ++k;
                while(m[k]!='/')
                {
                    a=a*10+m[k]-'0';
                    k++;
                }
                tt.push_back(a);
                a=0;
                ++k;
                while(m[k])
                {
                    a=a*10+m[k]-'0';
                    k++;
                }
                nt.push_back(a);
            }
            fvArr.push_back(vt);
            ftArr.push_back(tt);
            fnArr.push_back(nt);

            std::vector<float>Ka = Materials[nowmaterial].Ka;
            std::vector<float>Kd = Materials[nowmaterial].Kd;
            std::vector<float>Ks = Materials[nowmaterial].Ks;
            vec3 light(0,0,0);

            std::vector<vec3>points;
            {
                tomap=vArr;
                int index=(int)fvArr.size();
                for(int i=0;i<fvArr[index-1].size();++i)
                {
                    int ind=fvArr[index-1][i]-1;
                    float poi[] = {tomap[ind][0], tomap[ind][1], tomap[ind][2]};
                    translate(matrix, poi);
                    points.push_back(vec3(poi[0], poi[1], poi[2]));
                }
            }
            // polygon point
            vec3 normal(0,0,0);
            {
                tomap=vnArr;
                int index=(int)fnArr.size();
                for(int i=0;i<fnArr[index-1].size();++i)
                {
                    int ind=fnArr[index-1][i]-1;
                    float poi[] = {tomap[ind][0], tomap[ind][1], tomap[ind][2]};
                    normal += vec3(poi[0], poi[1], poi[2]);
                }
                normal /= (1.0f*fnArr[index-1].size());
            }
            // normal
            std::vector<vec2>ppoints;
            {
                tomap=vtArr;
                int index=(int)ftArr.size();
                for(int i=0;i<ftArr[index-1].size();++i)
                {
                    int ind=ftArr[index-1][i]-1;
                    float poi[] = {tomap[ind][0], tomap[ind][1]};
                    ppoints.push_back(vec2(poi[0], poi[1]));
                }
            }
            // texture color

            list[len++] = new polygon(
                                points,
                                ppoints,
                                normal,
                                light,
                                new metal(vec3(Kd[0], Kd[1], Kd[2]), Materials[nowmaterial].Tex)
                          );
        }
    }

    {
        list[len++] = new sphere(
                            vec3(0,-1000,0), 1000,
                            vec3(0.0, 0.0, 0.0),
                            new lambertian(vec3(0.5, 0.5, 0.5)));

    }
    // add floor

    printf("Polygon num: %d\n",len);

    return new hittable_list(list,len);
}

hittable *load_scene(std::string fname, float *matrix)
{
    std::string basic="D:/CG_homework/raytracing/objs/";
    std::string objfile=basic+fname+"/"+fname+".obj";
    std::string mtlfile=basic+fname+"/"+fname+".mtl";
    return combine(objfile, mtlfile, basic+fname, matrix);
}


#endif // LOADER_H
