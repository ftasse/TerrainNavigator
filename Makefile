#############################################################################
# Makefile for building: TerrNavigator
# Generated by qmake (2.01a) (Qt 4.6.2) on: Fri Oct 8 01:20:12 2010
# Project:  TerrNavigator.pro
# Template: app
# Command: /usr/bin/qmake-qt4 -spec /usr/share/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o Makefile TerrNavigator.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_XML_LIB -DQT_OPENGL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtOpenGL -I/usr/include/qt4/QtXml -I/usr/include/qt4 -I/usr/X11R6/include -I. -I.
LINK          = g++
LFLAGS        = 
LIBS          = $(SUBLIBS)  -L/usr/lib -L/usr/X11R6/lib /usr/lib/libqglviewer-qt4.so -lglut -lGLEW -lQtXml -lQtOpenGL -lQtGui -lQtCore -lGLU -lGL -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake-qt4
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp \
		mainwindow.cpp \
		interface.cpp \
		STHeightmap_ter.cpp \
		STHeightmap_pgm.cpp \
		STHeightmap.cpp \
		sky.cpp \
		terrain.cpp \
		standardCamera.cpp \
		STShaderProgram.cpp moc_mainwindow.cpp
OBJECTS       = main.o \
		mainwindow.o \
		interface.o \
		STHeightmap_ter.o \
		STHeightmap_pgm.o \
		STHeightmap.o \
		sky.o \
		terrain.o \
		standardCamera.o \
		STShaderProgram.o \
		moc_mainwindow.o
DIST          = /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/opengl.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		TerrNavigator.pro
QMAKE_TARGET  = TerrNavigator
DESTDIR       = 
TARGET        = TerrNavigator

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET): ui_mainwindow.h $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: TerrNavigator.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/opengl.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/libqglviewer-qt4.prl \
		/usr/lib/libQtXml.prl \
		/usr/lib/libQtOpenGL.prl \
		/usr/lib/libQtGui.prl \
		/usr/lib/libQtCore.prl
	$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o Makefile TerrNavigator.pro
/usr/share/qt4/mkspecs/common/g++.conf:
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/debug.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/opengl.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/libqglviewer-qt4.prl:
/usr/lib/libQtXml.prl:
/usr/lib/libQtOpenGL.prl:
/usr/lib/libQtGui.prl:
/usr/lib/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o Makefile TerrNavigator.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/TerrNavigator1.0.0 || $(MKDIR) .tmp/TerrNavigator1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/TerrNavigator1.0.0/ && $(COPY_FILE) --parents mainwindow.h interface.h STHeightmap.h sky.h terrain.h standardCamera.h STShaderProgram.h .tmp/TerrNavigator1.0.0/ && $(COPY_FILE) --parents main.cpp mainwindow.cpp interface.cpp STHeightmap_ter.cpp STHeightmap_pgm.cpp STHeightmap.cpp sky.cpp terrain.cpp standardCamera.cpp STShaderProgram.cpp .tmp/TerrNavigator1.0.0/ && $(COPY_FILE) --parents mainwindow.ui .tmp/TerrNavigator1.0.0/ && (cd `dirname .tmp/TerrNavigator1.0.0` && $(TAR) TerrNavigator1.0.0.tar TerrNavigator1.0.0 && $(COMPRESS) TerrNavigator1.0.0.tar) && $(MOVE) `dirname .tmp/TerrNavigator1.0.0`/TerrNavigator1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/TerrNavigator1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_mainwindow.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_mainwindow.cpp
moc_mainwindow.cpp: interface.h \
		STShaderProgram.h \
		sky.h \
		terrain.h \
		STHeightmap.h \
		standardCamera.h \
		mainwindow.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) mainwindow.h -o moc_mainwindow.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_mainwindow.h
compiler_uic_clean:
	-$(DEL_FILE) ui_mainwindow.h
ui_mainwindow.h: mainwindow.ui \
		interface.h \
		STShaderProgram.h \
		sky.h \
		terrain.h \
		STHeightmap.h \
		standardCamera.h
	/usr/bin/uic-qt4 mainwindow.ui -o ui_mainwindow.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_uic_clean 

####### Compile

main.o: main.cpp mainwindow.h \
		interface.h \
		STShaderProgram.h \
		sky.h \
		terrain.h \
		STHeightmap.h \
		standardCamera.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

mainwindow.o: mainwindow.cpp mainwindow.h \
		interface.h \
		STShaderProgram.h \
		sky.h \
		terrain.h \
		STHeightmap.h \
		standardCamera.h \
		ui_mainwindow.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mainwindow.o mainwindow.cpp

interface.o: interface.cpp mainwindow.h \
		interface.h \
		STShaderProgram.h \
		sky.h \
		terrain.h \
		STHeightmap.h \
		standardCamera.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o interface.o interface.cpp

STHeightmap_ter.o: STHeightmap_ter.cpp STHeightmap.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o STHeightmap_ter.o STHeightmap_ter.cpp

STHeightmap_pgm.o: STHeightmap_pgm.cpp STHeightmap.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o STHeightmap_pgm.o STHeightmap_pgm.cpp

STHeightmap.o: STHeightmap.cpp STHeightmap.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o STHeightmap.o STHeightmap.cpp

sky.o: sky.cpp sky.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o sky.o sky.cpp

terrain.o: terrain.cpp terrain.h \
		STHeightmap.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o terrain.o terrain.cpp

standardCamera.o: standardCamera.cpp standardCamera.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o standardCamera.o standardCamera.cpp

STShaderProgram.o: STShaderProgram.cpp STShaderProgram.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o STShaderProgram.o STShaderProgram.cpp

moc_mainwindow.o: moc_mainwindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_mainwindow.o moc_mainwindow.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:
