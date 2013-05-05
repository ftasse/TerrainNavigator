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

win32::INCLUDEPATH += $$quote(C:/Program Files (x86)/libQGLViewer/)

linux::LIBS += -lqglviewer-qt4  -lglut  -lGLEW
macos::LIBS += -lqglviewer-qt4 -lglut -lGLEW
win32::LIBS += -L$$quote(C:/Windows/System32) -lQGLViewer2 -lglut32 -lglew32 -lkernel32 -lgdi32

OTHER_FILES +=

RESOURCES += \
    application.qrc
