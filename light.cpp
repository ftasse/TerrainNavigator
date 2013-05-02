#include "light.h"

void fillvec4(float vec [], float a, float b, float c, float d ){
    vec[0]=a;
    vec[1]=b;
    vec[2]=c;
    vec[3]=d;
}


Material::Material(){
    fillvec4(ambient,1.0f, 1.0f, 1.0f, 1.0f);
    fillvec4(diffuse,1.0f, 1.0f, 1.0f, 1.0f);
    fillvec4(specular,1.0f, 1.0f, 1.0f, 1.0f);
    fillvec4(emissive,0.1f, 0.1f, 0.1f, 1.0f);
    shininess = 128.0f;
}

Material::~Material(){
}

void Material::setMaterial(){
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissive);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

Light::Light(){
    lightmode = GL_LIGHT0;
    fillvec4(ambient,0.3f, 0.3f, 0.3f, 1.0f);
    fillvec4(diffuse,1.0f, 1.0f, 1.0f, 1.0f);
    fillvec4(specular,0.1f,0.1f,0.1f, 1.0f);

    fillvec4(position,1.0f, 1.0f,1.0f, 1.0f);
}

Light::Light(GLenum mode){
     lightmode = mode;


     fillvec4(ambient,0.2f, 0.2f, 0.2f, 1.0f);
     fillvec4(diffuse,0.5f, 0.3f, 0.5f, 1.0f);
     fillvec4(specular,0.1f,0.1f,0.1f, 1.0f);

     fillvec4(position,10.0f, 10.0f,10.0f, 10.0f);
}

Light::~Light(){
    if (lightmode!=1) disable();
}

void Light::enable(){
    glEnable(lightmode);
}

void Light::disable(){
    glDisable(lightmode);
}

void Light::setLight(){
    glLightfv(lightmode, GL_POSITION, position);
    glLightfv(lightmode, GL_AMBIENT, ambient);
    glLightfv(lightmode, GL_DIFFUSE, diffuse);
    glLightfv(lightmode, GL_SPECULAR, specular);
}
