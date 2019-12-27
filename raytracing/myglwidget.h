#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include "utils.h"

class MyGLWidget : public QOpenGLWidget{
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void ObjLoader(std::string addrstr);
};
#endif // MYGLWIDGET_H
