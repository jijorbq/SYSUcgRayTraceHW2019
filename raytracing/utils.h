/*###################################################
##  文件描述：
##  将所有引用的库和宏都放在本库，方便使用
#####################################################*/


#ifndef UTILS_H
#define UTILS_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <iostream>
#include <vector>
#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <glm/glm.hpp>
#include <QApplication>
#include <map>
#include <fstream>
#include <sstream>
#include <functional>
#include <random>

#endif // UTILS_H
