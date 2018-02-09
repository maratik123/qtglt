#-------------------------------------------------
#
# Project created by QtCreator 2018-02-08T21:32:42
#
#-------------------------------------------------

TARGET = qtglt
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += Q_ENABLE_OPENGL_FUNCTIONS_DEBUG


SOURCES += \
        main.cpp \
        openglwindow.cpp \
    trianglewindow.cpp

HEADERS += \
        openglwindow.h \
    trianglewindow.h
