#include "myglwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyGLWidget w;
    w.setWindowTitle("Ray Tracing");
    w.show();
    return a.exec();
}
