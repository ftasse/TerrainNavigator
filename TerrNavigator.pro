# -------------------------------------------------
# Project created by QtCreator 2010-10-02T15:42:41
# -------------------------------------------------
QT += opengl \
    xml
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
LIBS += -lqglviewer-qt4 \
    -lglut \
    -lGLEW
OTHER_FILES +=

RESOURCES += \
    application.qrc
