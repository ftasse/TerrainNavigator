#ifndef LIGHT_H
#define LIGHT_H

#include <QtOpenGL>
#include <QVector4D>



class Material{
public:
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat emissive[4];
    GLfloat shininess;

    Material();
    ~Material();

    void setMaterial();
};
class Light{
public:
    GLenum lightmode;
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat position[4];

    Light();

    Light(GLenum mode);

    ~Light();

    void enable();

    void disable();
    void setLight();
};

#endif // LIGHT_H
