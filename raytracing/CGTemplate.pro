QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console qt c++11

DEFINES += QT_DEPRECATED_WARNINGS

LIBS += \
        OpenGL32.lib
        Glu32.lib


SOURCES += \
    main.cpp \
    myglwidget.cpp

HEADERS += \
    Loader.h \
    camera.h \
    geometry.h \
    hit.h \
    hitlist.h \
    material.h \
    myglwidget.h \
    polygon.h \
    random.h \
    ray.h \
    sphere.h \
    stb_image.h \
    texture.h \
    utils.h \
    aabb.h \
    bvh.h
