#ifndef TERRAIN_H
#define TERRAIN_H

#include <GL/glew.h>

#include "STHeightmap.h"

#include <QString>
#include <QImage>
#include <QColor>
#include <QVector3D>
#include <QVector4D>
#include <QVector2D>

#define DEF_COL QVector4D(0.6f, 0.5f, 0.4f, 1.0f)
//Define this somewhere in your header file
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

typedef struct
{
        QVector2D tex;
        QVector4D colour;
        QVector3D normal;
        QVector3D location;
        unsigned char padding[16]; // Pads the struct out to 64 bytes for performance increase
} Vertex;


class Terrain{
private:

    QImage texture;

    void Create(STHeightmap* heightmap);
public:
    Terrain();
    ~Terrain();
    Terrain(QString fname);
    void loadTexture(QString tex);
    void loadTexture();
    void GenerateNormals();
    void Draw();

    void updateVerts();

    void setColors(int start, float colors[4][4]);
    const unsigned int *indices() const { return faces.constData(); }
    int count() const { return faces.count(); }

    void DrawContainer();

    int width;
    int height;
    float wsc, hsc;
    float maxh;
    float minh;
    GLuint texid;

    GLuint vboIds[2];

    QString filename;
    QVector<QVector3D> artifacts;
    QVector<float> severity;

    QVector<Vertex> verts;
    QVector<unsigned int> faces;

};

/*
void JamesLight(){
    GLfloat lmodel_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

    // key light
    float lpos[] = {0.0f, 3.0f, 10.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lpos);

    float lamb1[] = {0.1f, 0.1f, 0.1f, 1.0f};
    float ldif1[] = {0.87f, 0.7f, 0.7f, 1.0f};
    float lspc1[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT1, GL_AMBIENT, lamb1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, ldif1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lspc1);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, ldif1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, ldif1);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, ldif1);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, ldif1);

}

void DemoLight2(){
    GLfloat redDiffuseMaterial[] = {1, 0.0, 0.0}; //set the material to red
    GLfloat whiteSpecularMaterial[] = {1, 1, 1}; //set  the material to white
    GLfloat greenEmissiveMaterial[] = {0.0, 1, 0.0}; //set the material to green

    GLfloat whiteSpecularLight[] = {1, 1, 1}; //set the light specular to white
    GLfloat blackAmbientLight[] = {0, 0, 0}; //set the light ambient to black
    GLfloat whiteDiffuseLight[] = {1, 1, 1}; //set the diffuse light to white

    GLfloat blankMaterial[] = {0.0, 0.0, 0.0}; //set the diffuse light to white
    GLfloat mShininess[] = {128}; //set the shininess of the material

    glLightfv(GL_LIGHT0, GL_SPECULAR, whiteSpecularLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, blackAmbientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDiffuseLight);

    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,greenEmissiveMaterial);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redDiffuseMaterial);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecularMaterial);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);
}

void DemoLight3(void)
 {
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_LIGHT1);
   glShadeModel(GL_SMOOTH);
   //glEnable(GL_NORMALIZE);
#include <GL/glew.h>

   // Light model parameters:
   // -------------------------------------------

   GLfloat lmKa[] = {0.0, 0.0, 0.0, 0.0 };
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);

   glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
   glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

   // -------------------------------------------
   // Spotlight Attenuation

   GLfloat spot_direction[] = {0, 0, -1.0 };//{1.0, -1.0, -1.0 };
   GLint spot_exponent = 30;
   GLint spot_cutoff = 180;

   glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
   glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
   glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);

   glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
   glLighti(GL_LIGHT1, GL_SPOT_EXPONENT, spot_exponent);
   glLighti(GL_LIGHT1, GL_SPOT_CUTOFF, spot_cutoff);

   GLfloat Kc = 1.0;
   GLfloat Kl = 0.0;
   GLfloat Kq = 0.0;

   glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,Kc);
   glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
   glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);

   glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION,Kc);
   glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, Kl);
   glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, Kq);


   // -------------------------------------------
   // Lighting parameters:

   GLfloat light_pos[] = {0.0f, 5.0f, 5.0f, 1.0f};
   GLfloat light_Ka[]  = {1.0f, 0.5f, 0.5f, 1.0f};
   GLfloat light_Kd[]  = {1.0f, 0.1f, 0.1f, 1.0f};
   GLfloat light_Ks[]  = {1.0f, 1.0f, 1.0f, 1.0f};

   glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
   glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);

   GLfloat light_pos1[] = {0.0f, 0.0f, 5.0f, 1.0f};
   glLightfv(GL_LIGHT1, GL_POSITION, light_pos1);
   glLightfv(GL_LIGHT1, GL_AMBIENT, light_Ka);
   glLightfv(GL_LIGHT1, GL_DIFFUSE, light_Kd);
   glLightfv(GL_LIGHT1, GL_SPECULAR, light_Ks);

   // -------------------------------------------
   // Material parameters:

   GLfloat material_Ka[] = {0.6f, 0.6f, 0.2f, 1.0f};
   GLfloat material_Kd[] = {0.6f, 0.6f, 0.2f, 1.0f};
   GLfloat material_Ks[] = {0.8f, 0.8f, 0.0f, 1.0f};
   GLfloat material_Ke[] = {0.1f, 0.0f, 0.0f, 1.0f};
   GLfloat material_Se = 1.0f;

   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
   glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);
 }
*/
#endif // TERRAIN_H
