#ifndef INTERFACE_H
#define INTERFACE_H

#define NLIGHTS 3

#define TASK0 0
#define TASK1 1
#define TASK2 2

#include <GL/glew.h>

#include <qgl.h>
#include <qevent.h>
#include <qcursor.h>
#include <qmenu.h>
#include <qlineedit.h>
#include <QGLViewer/qglviewer.h>
#include "sky.h"
#include "light.h"
#include "terrain.h"
#include "standardCamera.h"


class MainWindow;

class Viewer : public QGLViewer
{
    Q_OBJECT  // Necessary when signals and slots are present
private:
  bool drawMesh;
  SKY mySky;
  bool wireframe;
  bool addTexture;

  bool addLight[NLIGHTS];
  QVector<Terrain *> terrains;

  GLuint shaderProg;
  QString shader_vert, shader_frag;

  void InitializeProgram();


public :
    Viewer(QWidget *parent);
    ~Viewer(void);
    bool DrawInit( void );
    bool DrawShutdown( void );
    bool CheckExtension( const QString& szExtensionName );

    bool LoadHeightMap(QString fname);
    bool LoadHeightmaps(QVector<QString> files);
    void clearAll();

    //MainWindow *GlWindowClass;

    void SetCamera();
    void ComputeView( void );
    void getOpenGLmouseCoords();
    GLuint GetObjectHitByMouse();
    void DeleteTerrain ( unsigned int ChoosedTerrain );
    void addTerrain(QString tername);

    void processDoubleClick();

    bool del_flag, artifact_flag;
    //matrice decrivant la region vue par la camera
    float viewMatrix[6][4];

    QString task0_info, task1_info , task2_info;

    int curTerr;

    Light light;
    Material mat;
    QVector3D backgroundCol;

    QVector<int> rankings;


 void startTask2(QVector<QString> list);
 void startTask2(QString tlist_file);

 void startTask1(QVector<QString> list);
 void startTask1(QString tlist_file);

 void startsubTask1();
 void endsubTask1();

 QString experimentID;

 void DeleteAll();

signals:
    //Lighting

    //Background
    void updateTasklabel(QString str);
    void updateIDlabel(QString str);

    void rankingChanged();

    void taskIsNot0(bool);
    void taskIsNot1(bool);

    void infoChanged(QString str);

public slots:
    //Lightning
    void enableLight(bool value);
    void enableGLSL(bool value);

    //Background
    void setBackgroundR(int val);
    void setBackgroundG(int val);
    void setBackgroundB(int val);

    //Ambient
    void setAmbientR(int val);
    void setAmbientG(int val);
    void setAmbientB(int val);

    //Diffuse
    void setDiffuseR(int val);
    void setDiffuseG(int val);
    void setDiffuseB(int val);

    //Specular
    void setSpecularR(int val);
    void setSpecularG(int val);
    void setSpecularB(int val);

    //lightPosition
    void setPositionX(double val);
    void setPositionY(double val);
    void setPositionZ(double val);
    void setPositionW(double val);

    void setShininess(int val);

     void setSkybox(bool value);
     void setWireframe(bool value);
     void setTextured(bool value);

     bool moveNext();
     bool movePrevious();

     bool moveUp();
     bool moveDown();

     void setCurrentTerrain(int row);

     void LoadExperiment();
     void LoadTexture();

protected :
        void	      init();
        void          draw();
        void mouseDoubleClickEvent(QMouseEvent * e);
        virtual QString helpString() const;

        bool applyTexture;
        bool applyWire;
        bool applyGLSL;

        int task_id;
        int task_pos;
        QVector<QString> task_files;
        int task_status; //0 for not started 1 for started 2 for finished


};


#endif // INTERFACE_H
