# -------------------------------------------------
# Project created by QtCreator 2010-10-02T15:42:41
# -------------------------------------------------
QT += opengl xml

QMAKE_CXXFLAGS +=
QMAKE_LFLAGS +=

TARGET = TerrNavigator
TEMPLATE = app

SOURCES += main.cpp \
    mainwindow.cpp \
    interface.cpp \
    STHeightmap_ter.cpp \
    STHeightmap_pgm.cpp \
    STHeightmap.cpp \
    sky.cpp \
    terrain.cpp \
    standardCamera.cpp \
    light.cpp
HEADERS += mainwindow.h \
    interface.h \
    STHeightmap.h \
    sky.h \
    terrain.h \
    standardCamera.h \
    light.h
FORMS += mainwindow.ui \
    dialog.ui

win32::INCLUDEPATH += external/libQGLViewer/

linux::LIBS += -lqglviewer-qt4  -lglut  -lGLEW
macos::LIBS += -lqglviewer-qt4 -lglut -lGLEW
#win32::LIBS += -L"$$PWD/external/libQGLViewer/QGLViewer/debug" -lQGLViewer2d
win32::LIBS += -L"$$PWD/external/libQGLViewer/QGLViewer/release" -lQGLViewer2
win32::LIBS += -lglut32 -lglew32

OTHER_FILES +=

RESOURCES += \
    application.qrc
