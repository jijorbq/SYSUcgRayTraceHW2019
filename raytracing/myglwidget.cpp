#include "myglwidget.h"
#include "ray.h"
#include "hit.h"
#include "sphere.h"
#include "polygon.h"
#include "hitlist.h"
#include "geometry.h"
#include "material.h"
#include "random.h"
#include "camera.h"
#include <glm/glm.hpp>

#define MAXFLOAT 1000000000.0
#define vec3 glm::vec3

MyGLWidget::MyGLWidget(QWidget *parent)
    :QOpenGLWidget(parent)
{
}

MyGLWidget::~MyGLWidget()
{
}

void MyGLWidget::initializeGL()
{
	glViewport(0, 0, width(), height());
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glDisable(GL_DEPTH_TEST);
}


void MyGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	update();
}


std::vector<vec3>vArr;//存放点的二维数组
std::vector<vec3>vnArr;//存放法线的二维数组
std::vector<std::vector<GLfloat>>vtArr;//存放纹理的二维数组
std::vector<std::vector<int>>fvArr;//存放面顶点的二维数组
std::vector<std::vector<int>>ftArr;//存放面纹理的二维数组
std::vector<std::vector<int>>fnArr;//存放面法线的二维数组

void MyGLWidget::ObjLoader(std::string addrstr)
{

    std::string s1;
    GLfloat f2,f3,f4;

    std::ifstream infile(addrstr);
    std::string sline;//每一行
    if (! infile.is_open())
    {
        std::cout << "Error   opening file";
    }
    while(getline(infile,sline))
    {
        //std::cout<<sline<<std::endl;
        std::istringstream in(sline);
        std::string prefix;
        in>>prefix;
        if(prefix=="v")
        {
            GLfloat f1,f2,f3;
            in>>f1>>f2>>f3;
            vec3 temp=vec3(f1,f2,f3);
            vArr.push_back(temp);
        }
        else if(prefix=="vt")
        {
            GLfloat f1,f2;
            in>>f1>>f2;
            std::vector<GLfloat>temp={f1,f2};
            vtArr.push_back(temp);
        }
        else if(prefix=="vn")
        {
            GLfloat f1,f2,f3;
            in>>f1>>f2>>f3;
            vec3 temp=vec3(f1,f2,f3);
            vnArr.push_back(temp);
        }
        else if(prefix=="f")
        {
            std::cout<<sline<<std::endl;
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
        }

    }
}


vec3 color(const ray& r, hittable *world, int depth) {
    hit_record rec;
    vec3 col(0,0,0);
    if (world->hit(r, 0.001, MAXFLOAT, rec)) {
        ray scattered;
        vec3 attenuation;
        if (depth == 0) col += rec.col;
        if (depth < 5 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return col+attenuation*color(scattered, world, depth+1);
        }
        else {
            return vec3(0,0,0);
        }
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5*(unit_direction.y + 1.0);
        return (1.0f-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }
}
/*
hittable *random_scene() {
    int n = 500;
    hittable **list = new hittable*[n+1];
    list[0] =  new sphere(vec3(0,-1000,0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
    int i = 1;
    for (int a = -11; a < 11; a+=5) {
        for (int b = -11; b < 11; b+=5) {
            float choose_mat = random_double();
            vec3 center(a+0.9*random_double(),0.2,b+0.9*random_double());
            if ((center-vec3(4,0.2,0)).length() > 0.9) {
                if (choose_mat < 0.8) {  // diffuse
                    list[i++] = new sphere(center, 0.2,
                        new lambertian(vec3(random_double()*random_double(),
                                            random_double()*random_double(),
                                            random_double()*random_double())
                        )
                    );
                }
                else if (choose_mat < 0.95) { // metal
                    list[i++] = new sphere(center, 0.2,
                            new metal(vec3(0.5*(1 + random_double()),
                                           0.5*(1 + random_double()),
                                           0.5*(1 + random_double())),
                                      0.5*random_double()));
                }
                else {  // glass
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new hittable_list(list,i);
}
*/

hittable *load_scene()
{
    hittable **list = new hittable*[5];
    int len=0;
    vec3 p1(0.2, 0.2, -1);
    vec3 p2(0.5, 0.2, -1);
    vec3 p3(0.4, 0.6, -1);
    std::vector<vec3>points = {p1, p2, p3};
    vec3 normal(0, 0, 1);
    vec3 col(0.0, 0.0, 0.0);
    list[len++] = new polygon(points, normal, col, new lambertian(vec3(0.1, 0.2, 0.5)));
    return new hittable_list(list,len);
}

void MyGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glOrtho(0.0f, width(), 0.0f, height(), -1000.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    int W=width(), H=height(), S=1;

    /*
    hittable *list[4];
    list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
    list[1] = new sphere(vec3(0,-100.5,-1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.0));
    list[3] = new sphere(vec3(-1,0,-1), 0.5, new dielectric(1.5));
    hittable *world = new hittable_list(list,4);
    */

    //hittable *world = random_scene();

    //ObjLoader("./objs/pig");
    hittable *world = load_scene();

    vec3 lookfrom(0,0,0);
    vec3 lookat(0,0,-1);
    float dist_to_focus = (lookfrom-lookat).length();
    float aperture = 0.0;

    camera cam(lookfrom, lookat, vec3(0,1,0), 90.0,
               float(W)/float(H), aperture, dist_to_focus);

    printf("start!\n");
    for(int i = 0; i < W; i++) {
        for(int j = 0; j < H; j++) {
            vec3 col(0, 0, 0);
            for (int k = 0; k < S; k++) {
                //float u = float(i + random_double()) / float(W);
                //float v = float(j + random_double()) / float(H);
                float u = float(i) / float(W);
                float v = float(j) / float(H);
                ray r = cam.get_ray(u, v);
                col += color(r, world, 0);
            }
            col /= float(S);
            col = vec3(sqrt(col.x), sqrt(col.y), sqrt(col.z));

            glBegin(GL_POINTS);
            glVertex3i(i,j,0);
            glColor3f(col.x, col.y, col.z);
            glEnd();
        }
        if(i%20==0)printf("%d\n",i);
    }
}


