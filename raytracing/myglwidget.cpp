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
#include "Loader.h"
#include "texture.h"
#include "utils.h"

#define MAXFLOAT 1000000000.0

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

bool FLAG=0;
void MyGLWidget::resizeGL(int width, int height)
{
    //FLAG=0;
	glViewport(0, 0, width, height);
	update();
}

double* trans1()
{
    double *matrix = new double[16];
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //glTranslatef(-0.2f, 0.5f, 0.0f);
    //glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    //glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
    glScaled(0.7, 0.7, 0.7);
    glGetDoublev(GL_MODELVIEW_MATRIX, matrix);
    glPopMatrix();
    return matrix;
}


vec3 color(const ray& r, hittable *world, int depth, vec3 remain, int X, int Y) {
    hit_record rec;

    if (world->hit(r, 1e-2, MAXFLOAT, rec)) {
        ray scattered;
        vec3 attenuation;
        vec3 col = vec3(0,0,0);
        if(depth>=20 || remain.length()<1e-5){
            return vec3(0,0,0);
        }
        // 20 can be change

        for(int i=0;i<3;i++){
            if (rec.mat_ptr[i]==NULL)continue;
            if (rec.mat_ptr[i]->scatter(r, rec, attenuation, scattered)) {
                col += rec.light + attenuation*color(scattered, world, depth+1, remain*attenuation, X, Y);
            }
        }
        return col;
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        double t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }
}

void MyGLWidget::paintGL()
{
    if(!FLAG){FLAG=1;return;}
    if(FLAG>1)return;

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glOrtho(0.0, width(), 0.0, height(), -1000.0, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    int W=width(), H=height(), S=1; // 抗锯齿

    hittable *world = random_scene();
    //hittable *world = load_scene("pig", trans1());
    // load scene

    vec3 lookfrom(13,2,3);
    vec3 lookat(0,0,0);
    double dist_to_focus = 10.0;
    double aperture = 0.1;
    camera cam(lookfrom, lookat, vec3(0,1,0), 20, double(W)/double(H), aperture, dist_to_focus);


    FILE* fp=fopen("D:/CG_homework/raytracing/pic.txt","w+");

    fprintf(fp, "%d %d\n", W, H);

    printf("start!\n");
    for(int i = 0; i < W; i++) {
        for(int j = 0; j < H; j++) {
            vec3 col(0, 0, 0);
            int ti=clock();
            for (int k = 0; k < S; k++) {
                //float u = float(i + random_double()) / float(W);
                //float v = float(j + random_double()) / float(H);
                float u = float(i) / float(W);
                float v = float(j) / float(H);
                // use this if S==1
                ray r = cam.get_ray(u, v);
                col += color(r, world, 0, vec3(1.0, 1.0, 1.0), i, j);
            }
            col /= float(S);
            col = vec3(sqrt(col.x()), sqrt(col.y()), sqrt(col.z()));
            fprintf(fp, "%f %f %f\n",col.x(),col.y(),col.z());

            glBegin(GL_POINTS);
            glVertex3i(i,j,0);
            glColor3d(col.x(), col.y(), col.z());
            glEnd();

            if(j==H-1) std::cout<<"time for pixel ("<<i<<','<<j<<"): "<<clock()-ti<<std::endl;
        }
    }
    fclose(fp);

    FLAG=2;
}


