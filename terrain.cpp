#include "terrain.h"

#include <QGLWidget>
#include <Qt/qimage.h>

// A helper macro to get a position
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

static inline void qSetColor(float colorVec[], QColor c)
{
    colorVec[0] = c.redF();
    colorVec[1] = c.greenF();
    colorVec[2] = c.blueF();
    colorVec[3] = c.alphaF();
}

Terrain::Terrain(){
    width=0;
    height=0;
    hsc=0;
    wsc=0;
}

Terrain:: ~Terrain(){
    verts.clear();
    faces.clear();
    glDeleteBuffers(1, &vboIds[0]);
    glDeleteBuffers(1, &vboIds[1]);
}

Terrain::Terrain(QString fn){
    filename = fn;
    STHeightmap* heightmap = new STHeightmap (fn.toStdString());
    if (heightmap->GetPixels()==NULL)   return;

    QImage img(heightmap->GetWidth(),heightmap->GetHeight(),QImage::Format_ARGB32);


    STHeightmap* bigger = new STHeightmap(heightmap->GetWidth()+2,heightmap->GetHeight()+2,0);
    bigger->SetScale(heightmap->GetScale());
    for (int i=0; i<heightmap->GetWidth();i++)
        for (int j=0; j<heightmap->GetHeight();j++){
            float h = heightmap->GetPixel(i,j);
            bigger->SetPixel(i+1,j+1,h);
            img.setPixel(i,j,qRgb(h,h,h));
        }

    texture = QGLWidget::convertToGLFormat(img);
    loadTexture();
    delete heightmap;

    Create(bigger);
    delete bigger;
}

void Terrain::setColors(int start, float colorArray[4][4])
{
    /*int off = faces[start];
    for (int i = 0; i < 4; ++i)
        colors[i + off] = QVector4D(colorArray[i][0],
                                      colorArray[i][1],
                                      colorArray[i][2],
                                      colorArray[i][3]);*/
}

void Terrain::Create(STHeightmap *heightmap){
    width = heightmap->GetWidth();
    height = heightmap->GetHeight();
    float scale  = 13.8; //heightmap->GetScale();


    minh = 1000000.0f;
    maxh = 0.0f;

    wsc=width*scale;
    hsc=height*scale;

    for (int jj = 0; jj < height; ++jj) {
        for(int ii = 0; ii < width; ++ii) {

            float h = heightmap->GetPixel(ii,height-1-jj)*scale;

            Vertex v;
            v.location =  QVector3D((ii-width/2)*scale,(jj-height/2)*scale,h);
            v.tex = QVector2D(ii/float(width),jj/float(height));
            v.colour = DEF_COL; //QVector4D(0.6f, 0.5f, 0.3f, 1.0f);
            v.normal = QVector3D(0.0f, 0.0f, 0.0f);
            verts.push_back(v);

            if (h<minh)
                minh=h;
            if (h>maxh)
                maxh=h;
        }
    }


   for (int jj = 0; jj < height-1; jj++) {
        for(int ii = 0; ii < width-1; ii++) {
            unsigned int jjPlus1 = (jj + 1);
            unsigned int p1 = (ii + 0) + jj*width;
            unsigned int p2 = (ii + 1) + jj*width;
            unsigned int p3 = (ii + 1) + jjPlus1*width;
            unsigned int p4 = (ii + 0) + jjPlus1*width;

            // append first triangle
            faces.append(p3);
            faces.append(p4);
            faces.append(p1);
            // append second triangle
            faces.append(p2);
            faces.append(p3);
            faces.append(p1);

            //qWarning("%d %d %d %d",ii,ii+1,jj,jjPlus1);
        }
    }

   //qWarning("Vertices size: %d",verts.size());


   GenerateNormals();

   glGenBuffers(1, &vboIds[0]);
   glGenBuffers(1, &vboIds[1]);

   glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);


   glBufferData( GL_ARRAY_BUFFER,verts.size() * sizeof(Vertex), verts.constData(), GL_DYNAMIC_DRAW );
   glBufferData( GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(GLuint), faces.constData(), GL_STATIC_DRAW );

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

   //qWarning("Buffer loaded %d %d  %d %d", verts.size(), faces.size(),vboIds[0], vboIds[1]);

}

void Terrain::loadTexture(QString tex){
    texture =  QGLWidget::convertToGLFormat(QImage(tex));
    glGenTextures( 1, &texid );
    glBindTexture( GL_TEXTURE_2D, (texid) );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    //les deux commandes suivantes servent a elargir la texture pq. la surface soit remplie;
    //.. clamp_to_edge = repeter la derniere ligne de la texture (marche bien ici)
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texture.width(), texture.height(), 0,
                  GL_RGBA, GL_UNSIGNED_BYTE, texture.bits() );
    //qWarning("Texture loaded %d",texid);

    glBindTexture( GL_TEXTURE_2D, 0 );
}

void Terrain::loadTexture(){
    glGenTextures( 1, &texid );
    glBindTexture( GL_TEXTURE_2D, (texid) );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    //les deux commandes suivantes servent a elargir la texture pq. la surface soit remplie;
    //.. clamp_to_edge = repeter la derniere ligne de la texture (marche bien ici)
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texture.width(), texture.height(), 0,
                  GL_RGBA, GL_UNSIGNED_BYTE, texture.bits() );
    //qWarning("Texture loaded %d",texid);

    glBindTexture( GL_TEXTURE_2D, 0 );
}

void Terrain::GenerateNormals(){

    for (int ii = 0; ii < verts.size(); ++ii) {
        verts[ii].normal = QVector3D(0,0,0);
    }

    for (int ii = 0; ii < faces.size(); ii+=3) {
            GLuint f0 = faces[ii], f1 = faces[ii+1], f2 = faces[ii+2];

            QVector3D edge1 = verts[f1].location - verts[f0].location;
            QVector3D edge2 = verts[f2].location - verts[f0].location;
            QVector3D weightedNormal = QVector3D::crossProduct(edge1, edge2);

            verts[f0].normal += weightedNormal;
            verts[f1].normal += weightedNormal;
            verts[f2].normal += weightedNormal;
        }

        for (int ii = 0; ii < verts.size(); ++ii) {
            if (verts[ii].normal.lengthSquared() > 0.0f) {
                verts[ii].normal.normalize();
            }
        }
}

void Terrain::DrawContainer(){
    glColor4f(0.0,0.0,0.0,1.0);

     float xmin = -(wsc/2);
     float xmax = (wsc/2);

     float ymin = -(hsc/2);
     float ymax = (hsc/2);

     //glDisable(GL_CULL_FACE);

     glBegin( GL_QUADS );
     glVertex3f(xmin,ymin,0); glNormal3f(0,0,1);
     glVertex3f(xmin,ymax,0); glNormal3f(0,0,1);
     glVertex3f(xmax,ymax,0); glNormal3f(0,0,1);
     glVertex3f(xmax,ymin,0); glNormal3f(0,0,1);
     glEnd();

   }

void Terrain::Draw(){

    //glDisable(GL_LIGHTING);
    DrawContainer();
   // glEnable(GL_LIGHTING);
    
    //glDisable(GL_CULL_FACE);
    
    /*glVertexAttribPointer( program.attributes.position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, location) );
    glVertexAttribPointer( program.attributes.normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal) );
    glVertexAttribPointer( program.attributes.texcoord, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex));
    //glInterleavedArrays(GL_T2F_C4F_N3F_V3F, 64, verts.constData());*/
    GLfloat   scale[4];
    GLfloat   range[4];

  /*scale[0] = 0;
  scale[1] = maxh;///width;
  scale[2] = 0;
  scale[3] = 1.0;
  glProgramEnvParameter4fvARB(GL_VERTEX_PROGRAM_ARB, 1, scale);

  range[0] = 1.0 / maxh;
  range[1] = 0;
  range[2] = 0;
  range[3] = 1.0;
  glProgramEnvParameter4fvARB(GL_VERTEX_PROGRAM_ARB, 2, range);*/

    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0] );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vboIds[1] );

    int offset = 0;

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    //glVertexAttribPointer( program->GetUniformLocation("position"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, location) );
    //glVertexAttribPointer( program.attributes.normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal) );
    //glVertexAttribPointer( program.attributes.texcoord, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex));

    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(offset)); offset+=sizeof(QVector2D);
    glColorPointer(4, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(offset)); offset+=sizeof(QVector4D);
    glNormalPointer(GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(offset));    offset+=sizeof(QVector3D);
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(offset));    offset+=sizeof(QVector3D);
    
   glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    //glDrawArrays(GL_TRIANGLES, 0,faces.size());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

void Terrain::updateVerts(){
        glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
        glBufferData( GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), verts.constData(), GL_DYNAMIC_DRAW );
}
