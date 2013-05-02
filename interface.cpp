#include "mainwindow.h"
#include "interface.h"
#include <algorithm>
#include <math.h>

#include "ui_dialog.h"
#include <QFile>


#define SQR( number )		( number*number )
#define CUBE( number )		( number*number*number )
#define MIN( a, b )  ( ( ( a )<( b ) )?( a ):( b ) )
#define MAX( a, b )  ( ( ( a )<( b ) )?( b ):( a ) )

static GLuint	Shader[3], Shader_ter;

//char * MapShader =;

static char * ShaderSrc[3] = {
/* Vertex half of a Phong shader for ARB. Based on
   examples from nVidia and the Cg book         */
"!!ARBvp1.0					\n\
# Setup						\n\
ATTRIB	vPos	= vertex.position;		\n\
ATTRIB	vNorm	= vertex.normal;		\n\
ATTRIB  vCol	= vertex.color;			\n\
OUTPUT	oPos	= result.position;		\n\
OUTPUT	oCol	= result.color;			\n\
OUTPUT  oTex	= result.texcoord;		\n\
PARAM	CTM[4]	= { state.matrix.mvp };		\n\
PARAM	IMV[4]	= { state.matrix.modelview.invtrans };\n\
TEMP	eyeNorm;				\n\
## Standard transform				\n\
DP4  oPos.x, CTM[0], vPos;			\n\
DP4  oPos.y, CTM[1], vPos;			\n\
DP4  oPos.z, CTM[2], vPos;			\n\
DP4  oPos.w, CTM[3], vPos;			\n\
## Transform surface normal			\n\
DP3  eyeNorm.x, IMV[0], vNorm;			\n\
DP3  eyeNorm.y, IMV[1], vNorm; 			\n\
DP3  eyeNorm.z, IMV[2], vNorm;			\n\
# ...normalise					\n\
DP3  eyeNorm.w, eyeNorm, eyeNorm;		\n\
RSQ  eyeNorm.w, eyeNorm.w;			\n\
MUL  eyeNorm, eyeNorm, eyeNorm.w;		\n\
# ...pass to fragment as texture coords		\n\
MOV  oTex, eyeNorm;				\n\
## Lighting done in fragment shader		\n\
MOV  oCol, vCol;				\n\
END\n\
"
,
/* Phong shader for fragments */
"!!ARBfp1.0					\n\
# Setup						\n\
ATTRIB	fCol	= fragment.color;		\n\
ATTRIB	fTex	= fragment.texcoord;		\n\
TEMP	eyeNorm, coeff, shade;			\n\
PARAM	lVec	= program.env[1];		\n\
PARAM	lHalf	= state.light[0].half;		\n\
PARAM	red = { 1, 0, 0, 1 };			\n\
## Interpolated surface normal			\n\
MOV  eyeNorm, fTex;				\n\
# May not be unit length any more		\n\
DP3  eyeNorm.w, eyeNorm, eyeNorm;		\n\
RSQ  eyeNorm.w, eyeNorm.w;			\n\
MUL  eyeNorm, eyeNorm, eyeNorm.w;		\n\
## Lighting					\n\
## Ambient: no setup required			\n\
MUL  shade, state.lightmodel.ambient, fCol;	\n\
## Diffuse: light dot normal			\n\
DP3  coeff.x, lVec, eyeNorm;			\n\
## Specular					\n\
DP3  coeff.y, lHalf, eyeNorm;			\n\
MOV  coeff.w, state.material.shininess.x;	\n\
# Eval coefficients and sum			\n\
LIT  coeff, coeff;				\n\
MAD  shade, coeff.y, fCol, shade;		\n\
MAD  shade, coeff.z, fCol, shade;		\n\
MOV  result.color, shade;			\n\
END\n\
"
,
/* Heightfield shader that handles scaling, texture coord
   calculation from raw height data. */
"!!ARBvp1.0					\n\
# Setup						\n\
OUTPUT	oPos	= result.position;		\n\
OUTPUT  oCol	= result.color;			\n\
OUTPUT	oTex	= result.texcoord;		\n\
PARAM	CTM[4]	= { state.matrix.mvp };		\n\
PARAM   gScale  = program.env[1];		\n\
PARAM	hRange	= program.env[2];		\n\
TEMP    vPos, vTex;				\n\
PARAM	ZERO = { 0, 0, 0, 0 };			\n\
PARAM	ONE  = { 1, 1, 1, 1 };			\n\
## Texture coord from height			\n\
# Outgoing Q must be 1, others don't care	\n\
MOV  vTex, ONE;					\n\
MOV  vTex.x, vertex.position.y;			\n\
MUL  vTex, vTex, hRange;			\n\
MOV  oTex, vTex;				\n\
## Scale heightfield data			\n\
MOV  vPos, vertex.position;			\n\
MUL  vPos, vPos, gScale;			\n\
## Standard transform				\n\
DP4  oPos.x, CTM[0], vPos;			\n\
DP4  oPos.y, CTM[1], vPos;			\n\
DP4  oPos.z, CTM[2], vPos;			\n\
DP4  oPos.w, CTM[3], vPos;			\n\
## In case we forgot to change texture mode	\n\
MOV  oCol, ONE;					\n\
END\n\
"
};

// Constructor must call the base class constructor.

Viewer::Viewer(QWidget *parent) : QGLViewer(parent)
{
    //restoreStateFromFile();
    //help();
    qglviewer::Camera* c = camera();
    setCamera(new StandardCamera());
    delete c;
    shaderProg=0;
    curTerr = -1;
    task_id = TASK0;

    applyGLSL = false;
    applyTexture = false;
    backgroundCol = QVector3D(0.5,0.5,1.0);

    camera()->setUpVector(qglviewer::Vec(0.0f, 1.0f, 0.0f));
    camera()->setPosition(qglviewer::Vec(0.0f, -5000.0f, 0.0f));
    camera()->lookAt(qglviewer::Vec(0.0f, 0.0f, 0.0f));

    //emit taskIsNot1(true);
    task0_info = "Tutorial on how to use this interface.\n\n";
    task0_info += "Left click and hold to rotate the camera\n\nRight click and hold to drag the camera\n\n";
    task0_info += "Zoom in or out by scrolling the mouse wheel\n\n";
    task0_info += "Double click on a region to mark it as an artifact then select the severity level.\n\n ";
    task0_info += "Double click on a flag to delete it and unmark the region around it as an artifact.\n\n";
    task0_info += "When you are ready to start the experiment go to File->Load Experiment.";

    task1_info = "Classify Landscape A, B and C by decreasing order of realism.\n\n";
    task1_info = "Click on Landscape A, B or C to explore it.\n\n ";
    task1_info += "To change the position of the selected landscape, press Move Up or Move Down\n\n";
    task1_info += "Once you are satisfied with your classification, press Next.";


    task2_info = "Explore the terrain and find regions with artifacts\n\n";
    task2_info += "Double click on a region to mark it as an artifact then select the severity level.\n\n ";
    task2_info += "Double click on a flag to delete it and unmark the region around it as an artifact.\n\n";
    task2_info += "Once you have marked all artifacts, press Next.";
}

void Viewer::startTask2(QVector<QString> tlist){
    if (tlist.size()==0)    return;

    QMessageBox msgBox;
    msgBox.setText("Task 2 is about to begin\n You will find information about this task on the bottom left area of the screen");
    msgBox.exec();

    task_files = tlist;
    task_id = TASK2;
    task_status = 1;
    task_pos = 0;
    addTerrain(task_files.front());
    emit updateTasklabel(QString::number(task_pos+1)+"/"+QString::number(task_files.size()));
    emit updateIDlabel("TASK "+QString::number(task_id));
    emit taskIsNot1(true);
    emit taskIsNot0(true);
    emit infoChanged(task2_info);
}

void Viewer::startTask2(QString tlist_file){
    QVector<QString> tlist;

    QFile tfile( tlist_file );
    if (tfile.open(QFile::ReadOnly)) {
        QTextStream in(&tfile);
        QString line = in.readLine();
        while (!line.isNull()) {
           tlist.push_back(line);
            line = in.readLine();
        }
        tfile.close();
     }
    else{
        qWarning("Could not open list of files for TASK 2");
        return;
    }

    qWarning("Start TASK 2 with %d files.",tlist.size());
    startTask2(tlist);
}

void Viewer::startsubTask1(){
    //qWarning("Start subtask %d",(task_pos/3)+1);

    if (task_pos+3<=task_files.size()){
        rankings[0]=0;
        rankings[1]=1;
        rankings[2]=2;

        addTerrain(task_files[task_pos+0]);
        addTerrain(task_files[task_pos+1]);
        addTerrain(task_files[task_pos+2]);

        curTerr = 0;
        //updateGL();
        emit updateTasklabel(QString::number((task_pos/3)+1)+"/"+QString::number(task_files.size()/3));
        emit rankingChanged();
    }
    else{
        qWarning("Task 1  ended");
        task_status = 1;
        task_id = TASK0;
        emit updateIDlabel("TASK "+QString::number(task_id));
        emit updateTasklabel(QString::number(curTerr+1)+"/"+QString::number(terrains.size()));
        emit taskIsNot1(true);
        emit taskIsNot0(true);
        startTask2("task2_files.txt");
    }
}

void Viewer::endsubTask1(){
    //qWarning("Subtask over.");
    //qWarning("%s ranked %d",terrains[rankings[0]]->filename.toStdString().c_str(),0);
    //qWarning("%s ranked %d",terrains[rankings[1]]->filename.toStdString().c_str(),1);
    //qWarning("%s ranked %d",terrains[rankings[2]]->filename.toStdString().c_str(),2);

    QString pname = experimentID+".results";
    FILE * pFile = fopen( (char*) pname.toStdString().c_str(),"a");
    fprintf (pFile,"TASK1 SUBTASK%d \n", (task_pos/3)+1);
    for (int l=0; l < 3; l++){
        fprintf (pFile, "R%d I%d %s \n",l,rankings[l],terrains[rankings[l]]->filename.toStdString().c_str());
    }
    fclose(pFile);

    while(terrains.size()>0)
        DeleteTerrain(terrains.size()-1);
    //DeleteAll();
    curTerr = -1;
    task_pos+=3;
}

void Viewer::startTask1(QVector<QString> tlist){
    DeleteAll();
    if (tlist.size()==0)    return;
    task_files = tlist;
    task_id = TASK1;
    task_status = 1;
    task_pos = 0;

     QMessageBox msgBox;
     msgBox.setText("Task 1 is about to begin\n You will find information about this task on the bottom left area of the screen");
     msgBox.exec();

    rankings.push_back(0);  rankings.push_back(1);  rankings.push_back(2);
    emit updateIDlabel("TASK "+QString::number(task_id));
    startsubTask1();
    emit taskIsNot1(false);
    emit taskIsNot0(true);
    emit infoChanged(task1_info);
}

void Viewer::startTask1(QString tlist_file){
    QVector<QString> tlist;

    QFile tfile( tlist_file );
    if (tfile.open(QFile::ReadOnly)) {
        QTextStream in(&tfile);
        QString line = in.readLine();
        while (!line.isNull()) {
           tlist.push_back(line);
            line = in.readLine();
        }
        tfile.close();
     }
    else{
        qWarning("Could not open list of files for TASK 1");
        return;
    }

    qWarning("Start TASK 1 with %d files.",tlist.size());
    startTask1(tlist);
}

Viewer::~Viewer(void){

    //if (shaderProgram!=NULL)    delete shaderProgram;
    for (int k=0; k<terrains.size(); k++){
        delete terrains[k];
    }
}

void drawFlag(QVector3D vec){
    //glLineWidth(5);
    //glPolygonOffset(1.0, 2);
    glPushMatrix();
    glBegin(GL_LINES);
        glVertex3f(vec.x(),vec.y(),vec.z());
        glVertex3f(vec.x(),vec.y(),vec.z()+200);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex3f(vec.x(),vec.y(),vec.z()+200);
        glVertex3f(vec.x(),vec.y(),vec.z()+100);
        glVertex3f(vec.x()+50,vec.y(),vec.z()+150);
    glEnd();
    glPopMatrix();
    //glLineWidth(1);
}
static void getErrorLine (char * src, int err, char line[], int maxLine)
{
  int start, finish, n;

  start = err;
  while (start >= 0 && isprint(src[start]))
    start --;
  start ++;
  finish = err;
  while (isprint(src[finish]))
    finish ++;
  n = finish - start;
  if (n >= maxLine)
    n = maxLine;
  strncpy(line, src + start, n);
  line[n] = 0;
}

static void checkShader (int i)
{
  int  err;
  char msg[256];

  glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &err);
  if (err >= 0) {
    getErrorLine(ShaderSrc[i], err, msg, sizeof(msg));
    printf("Error in shader #%d: %s\n", i,
              glGetString(GL_PROGRAM_ERROR_STRING_ARB));
    printf("Source line:    %s\n", msg);
  }
}

static void initShader_phong ()
{
  /*if (! glutExtensionSupported("GL_ARB_vertex_program"))
    Fail("GL_ARB_vertex_program not available on this machine");
  if (! glutExtensionSupported("GL_ARB_fragment_program"))
    Fail("GL_ARB_fragment_program not available on this machine");*/

  glEnable(GL_VERTEX_PROGRAM_ARB);
  glEnable(GL_FRAGMENT_PROGRAM_ARB);

  glGenProgramsARB(3, Shader);

  glBindProgramARB(GL_VERTEX_PROGRAM_ARB, Shader[0]);
  glProgramStringARB(GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB,
                        strlen(ShaderSrc[0]), ShaderSrc[0]);
  checkShader(0);

  glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, Shader[1]);
  glProgramStringARB(GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB,
                        strlen(ShaderSrc[1]), ShaderSrc[1]);
  checkShader(1);

  /*glBindProgramARB(GL_VERTEX_PROGRAM_ARB, Shader[2]);
  glProgramStringARB(GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB,
                        strlen(ShaderSrc[2]), ShaderSrc[2]);
  checkShader(2);*/

  glDisable(GL_VERTEX_PROGRAM_ARB);
  glDisable(GL_FRAGMENT_PROGRAM_ARB);
}

void Viewer::init()
{
    glewInit();
    //shader_vert="toon.vert";    shader_frag = "toon.frag";
    //shader_vert="vertexshader.txt";    shader_frag = "fragmentshader.txt";
    //InitializeProgram();
    //initShader_ter();
    restoreStateFromFile();

    glEnable (GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glEnable(GL_TEXTURE_2D);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_COLOR_MATERIAL);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment

    // pretty rendering
    // fast rendering
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    light.setLight();
    mat.setMaterial();

    mySky.Set( 0,0,0, 300000.0f );
    mySky.LoadTextures(":sky");

    emit updateTasklabel(QString::number(curTerr+1)+"/"+QString::number(terrains.size()));

    initShader_phong();
    QVector4D ltmp(light.position[0],light.position[1],light.position[2],light.position[3]);
    ltmp.normalize();
    GLfloat lpos[4] = {ltmp.x(),ltmp.y(),ltmp.z(),ltmp.w()};
    glProgramEnvParameter4fvARB(GL_FRAGMENT_PROGRAM_ARB, 1, lpos);

    //QVector<QString> tlist;
    //tlist.push_back("CD.ter");
    //tlist.push_back("terr_gcut.ter");
//    /addTerrain("CD.ter");
    //addTerrain("terr_gcut.ter");
    help();
    //DrawInit();
    //qWarning("start drawing");

}

void Viewer::LoadTexture()
{
    QString fileName = QFileDialog::getOpenFileName(this, QObject::tr("Open Texture"),
                                                    QObject::tr(""),
                                                    QObject::tr("Images (*.png *.xpm *.jpg)"));
    if (fileName.size() > 0)
    {
        for (int i=0; i<terrains.size(); i++)
            terrains[i]->loadTexture(fileName);
        updateGL();
    }
}

void Viewer::LoadExperiment(){
    for (int i=0; i<terrains.size(); i++)
        DeleteTerrain(i);
    curTerr=-1;
    experimentID = QDate::currentDate().toString() +" "+ QTime::currentTime().toString();

    QString faculty, department, studyyear, age, sex, gameexperience;

    Ui::Dialog dia;
    QDialog *dialog = new QDialog;
    dia.setupUi(dialog);
    dialog->exec();

    connect(dia.buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(dia.buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));

    //if (dialog->Rejected)   return;

    faculty = dia.FacultyEdit->displayText();
    department = dia.DepartmentEdit->displayText();
    studyyear = dia.YearStudyEdit->displayText();
    age = dia.AgeEdit->displayText();
    sex = dia.SexBox->currentText();
    gameexperience = dia.gamingBox->currentText();

    QString pname = experimentID+".results";
    FILE * pFile = fopen( (char*) pname.toStdString().c_str(),"a");
    fprintf (pFile,"Experiment conducted at %s\n", experimentID.toStdString().c_str());
    fprintf (pFile,"Demographics\n");
    fprintf (pFile,"faculty: %s\n",faculty.toStdString().c_str());
    fprintf (pFile,"department: %s\n",department.toStdString().c_str());
    fprintf (pFile,"year_of_study: %s\n", studyyear.toStdString().c_str());
    fprintf (pFile,"age: %s\n",age.toStdString().c_str());
    fprintf (pFile,"sex: %s\n",sex.toStdString().c_str());
    fprintf (pFile,"game_experience: %s\n",gameexperience.toStdString().c_str());
    fclose(pFile);

    startTask1("task1_files.txt");
    updateGL();
}

void Viewer::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClearColor(backgroundCol.x(), backgroundCol.y(), backgroundCol.z(), 1.0f);

    if (mySky.wantSky())
         mySky.Render( );

    glEnable(GL_LIGHTING);

    if (applyGLSL)
        {
            glEnable(GL_VERTEX_PROGRAM_ARB);
            glEnable(GL_FRAGMENT_PROGRAM_ARB);
            light.position[3] = 1;
            light.setLight();
        }
    else{
        light.position[3] = 0;
        light.setLight();
    }

    QVector4D ltmp(light.position[0],light.position[1],light.position[2],light.position[3]);
    ltmp.normalize();
    GLfloat lpos[4] = {ltmp.x(),ltmp.y(),ltmp.z(),ltmp.w()};
    glProgramEnvParameter4fvARB(GL_FRAGMENT_PROGRAM_ARB, 1, lpos);

    // Draws a terrain
    if (curTerr>=0 && curTerr<terrains.size()){

        Terrain * terrain =  terrains[curTerr];
        if (applyTexture)   glBindTexture( GL_TEXTURE_2D, terrain->texid );
        else glBindTexture( GL_TEXTURE_2D, 0);
        terrain->Draw();

        //glLineWidth(10.0);
        //glPointSize(10.0);

        //glColor3f(1.0f,0.0f,0.0f);
        for (int i=0; i<terrain->artifacts.size(); i++){
            glColor3f(terrain->severity[i]/2.2f, 0.0f, 0.0f);
                drawFlag(terrain->artifacts[i]);
        }
    }

    glDisable(GL_VERTEX_PROGRAM_ARB);
    glDisable(GL_FRAGMENT_PROGRAM_ARB);

    glDisable(GL_LIGHTING);

    //glLineWidth(1.0);
    //glPointSize(1.0);



}

void Viewer::DeleteTerrain ( unsigned int k ){

    if (k< (unsigned int)terrains.size()){

        if (task_id == TASK2){

            QString pname = experimentID+".results";
            FILE * pFile = fopen( (char*) pname.toStdString().c_str(),"a");
            fprintf (pFile,"TASK2 SUBTASK%d %d \n", task_pos+1,terrains[k]->artifacts.size());
            for (int l=0; l < terrains[k]->artifacts.size(); l++){
                QVector3D npoint = terrains[k]->artifacts[l];
                float sc  = terrains[k]->wsc/terrains[k]->width;
                int xs =  (int) ( (npoint.x()/sc) + terrains[k]->width/2);
                int ys =  (int) ( (npoint.y()/sc) + terrains[k]->height/2);
                fprintf (pFile, "X%d Y%d L%d\n",xs-1,ys-1,(int) (terrains[k]->severity[l]+0.7));
            }
            fclose(pFile);

        }

        delete terrains[k];
        terrains.erase(terrains.begin()+k);
    }
    if (terrains.size()>0)
        curTerr-=1;
    else curTerr=-1;

    if (task_id == TASK0)   emit updateTasklabel(QString::number(curTerr+1)+"/"+QString::number(terrains.size()));
}

void Viewer::DeleteAll (){
    while(terrains.size()>0)
        DeleteTerrain(terrains.size()-1);
    curTerr=-1;
    task_id = TASK0;
    emit updateTasklabel(QString::number(curTerr+1)+"/"+QString::number(terrains.size()));
}


void Viewer::addTerrain(QString tername){
    terrains.push_back(new Terrain(tername));
    curTerr=terrains.size()-1;
    camera()->setPosition(qglviewer::Vec(0.0f,-terrains[curTerr]->hsc,terrains[curTerr]->hsc));
    camera()->lookAt(qglviewer::Vec(0.0f, 0.0f, 0.0f));
    if (task_id == TASK0) emit updateTasklabel(QString::number(curTerr+1)+"/"+QString::number(terrains.size()));
}

void Viewer::mouseDoubleClickEvent(QMouseEvent * e){
    if (e->button() == Qt::LeftButton && (e->modifiers() == Qt::NoButton)){
        QPointF point = e->posF();
        bool found;

        int tid = curTerr;
        if (tid <0 || tid > terrains.size())
            return;

        Terrain* terrain = terrains[tid];

        qglviewer::Vec selectedPoint = camera()->pointUnderPixel(point.toPoint(), found); //QVector3D(orig.x,orig.y,orig.z)+10000*QVector3D(dir.x,dir.y,dir.z);//
        if (!found) return;

        QVector3D I2 = QVector3D(selectedPoint.x,selectedPoint.y,selectedPoint.z);
        float sc = terrain->wsc/terrain->width;
        I2/=sc;
        float xs = I2.x()+terrain->width/2;
        float ys = I2.y()+terrain->height/2;
        xs = (int) xs;
        ys = (int) ys;

        //qWarning("Intersection is %d %d", (int)xs, (int)ys);
        //qWarning("Selected point is %f %f %f\n",selectedPoint.x,selectedPoint.y,selectedPoint.z);

        int id = (xs)+(ys)*terrain->width;
        if (!(xs>0 && ys>0 && xs<terrain->width-1 && ys< terrain->height-1)){
            return;
        }
        QVector3D npoint = terrain->verts[id].location;

        int index = terrain->artifacts.indexOf(npoint);

        if (index==-1 || terrain->verts[id].colour.y()!=DEF_COL.y()){
            for (int j=-5; j<5; j++){
                for (int i=-5; i<5; i++){
                    id = (xs+i)+(ys+j)*terrains[tid]->width;
                    if (xs+i>0 && ys+j>0 && xs+i<terrain->width-1 && ys+j< terrain->height-1){
                        index = terrain->artifacts.indexOf(terrain->verts[id].location);
                        if (index!=-1){
                            xs += i; ys += j;
                            npoint = terrain->verts[id].location;
                            break;
                        }
                    }
                }
            if (index!=-1)
                break;
        }
        }
        if (index==-1){
            QMenu menu("Severity", this );
            QMap<QAction*, float> menuMap;
            menuMap[menu.addAction("Slightly severe ")] = 1.1;
            menuMap[menu.addAction("Moderately severe")] = 1.35;
            menuMap[menu.addAction("Very severe")] = 3.0;
            QAction* action = menu.exec(e->globalPos());
            if (!action)
                return;

            terrain->artifacts.push_back(QVector3D( npoint.x(), npoint.y(), npoint.z()));
            terrain->severity.push_back(menuMap[action]);
            for (int j=-10; j<10; j++)
                for (int i=-10; i<10; i++){
                    id = (xs+i)+(ys+j)*terrain->width;
                    if (xs+i>0 && ys+j>0 && xs+i<terrain->width-1 && ys+j< terrain->height-1){
                        QVector4D& col = terrain->verts[id].colour;
                        col.setY(col.y()*terrain->severity.back());
                        //terrains[tid]->verts[id].colour* //= QVector4D(1.,0,0,0.1);
                    }
            }
        }
        else {
            for (int j=-10; j<10; j++)
                for (int i=-10; i<10; i++){
                    id = (xs+i)+(ys+j)*terrain->width;
                    if (xs+i>0 && ys+j>0 && xs+i<terrain->width-1 && ys+j< terrain->height-1){
                        QVector4D& col = terrain->verts[id].colour;
                        col.setY(col.y()/terrain->severity[index]);
                        //terrains[tid]->verts[id].colour* //= QVector4D(1.,0,0,0.1);
                    }
                }
            terrain->artifacts.erase(terrain->artifacts.begin()+index);
            terrain->severity.erase(terrain->severity.begin()+index);
            terrain->updateVerts();
        }


        QString str = QString::number(xs)+"  " +QString::number(ys);
        //displayMessage(str,5000);
        terrain->updateVerts();

        updateGL();
        return;
    }
    else
        QGLViewer::mouseDoubleClickEvent(e);
}

QString Viewer::helpString() const
{
    QString text("<h2>Experiment</h2>");

    /*text+="The duration of the experiment for each participant is about 50 mins. You have 5-10 mins to familiarize yourself with this interface.<br>";
    text+="You will then perform the following tasks:<br><br>";

    text+="<b>Task 1</b><br> This task consists of three subtasks. In every subtask, you are presented with three terrains in random order. You then have to classify them in a decreasing order of realism<br>";
    text+="A terrain is said to be <i><b>realistic</b></i> if it seems to accurately represent real natural terrains.<br>";
    text+="Estimate duration: 15 mins <br><br>";

    text+="<b>Task 2</b><br> This task consists of 18 subtasks. In every subtask, you are presented with a single terrain. You then have to select regions with artifacts by double clicking on them<br>";
    text+="An <i><b>artifact</i></b> is an anomaly on the surface of the terrain. An example is a visible edge or areas which are not smooth. <br>";
    text+="Estimate duration: 30 mins <br><br>";

    text+= "At all times, information on what you need to do will be provided in the bottom left area of your screen.<br>";
    */

    text+="<h3>Quick tutorial on exploring a terrain</h3>";

    text+= "<b>Left click</b> hold to rotate the camera<br>";
    text+= "<b>Right click</b> hold to drag the camera<br>";
    text+= "<b>Wheel</b> scroll to zoom in or out<br>";
    text+= "<b>Left double click</b> on terrain to mark a region<br>";
    text+= "<b>Left double click</b> on a flag to mark a region<br>";

    return text;
}

void Viewer::setSkybox(bool value){
    mySky.switchSky();
    updateGL();
}
void Viewer::setWireframe(bool value){
    if (value)  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    updateGL();
}

void Viewer::setTextured(bool value){
    applyTexture = !applyTexture;
    updateGL();
    //qWarning("apply texture %d", (int) applyTexture);
}

//Lightning
void Viewer::enableLight(bool value){
    if (value) light.enable();
    else light.disable();
    updateGL();
}

void Viewer::enableGLSL(bool value){
    applyGLSL = !applyGLSL;
    updateGL();
}

void Viewer::setBackgroundR(int v){
    float val = v/255.;
    if (backgroundCol.x() != val) {
                backgroundCol.setX(val);
                //emit BackgroundRChanged(val);
                updateGL();
            }
}

void Viewer::setBackgroundG(int v){
    float val = v/255.;
    if (backgroundCol.y() != val) {
                backgroundCol.setY(val);
                //emit BackgroundRChanged(val);
                updateGL();
            }
}

void Viewer::setBackgroundB(int v){
    float val = v/255.;
    if (backgroundCol.z() != val) {
                backgroundCol.setZ(val);
                //emit BackgroundRChanged(val);
                updateGL();
            }
}

//Ambient
void Viewer::setAmbientR(int v){
    float val = v/255.;
    light.ambient[0] = val;
    light.setLight();
    updateGL();
}

void Viewer::setAmbientG(int v){
    float val = v/255.;
    light.ambient[1] = val;
    light.setLight();
    updateGL();
}

void Viewer::setAmbientB(int v){
    float val = v/255.;
    light.ambient[2] = val;
    light.setLight();
    updateGL();
}

//Diffuse
void Viewer::setDiffuseR(int v){
    float val = v/255.;
    light.diffuse[0] = val;
    light.setLight();
    updateGL();
}

void Viewer::setDiffuseG(int v){
    float val = v/255.;
    light.diffuse[1] = val;
    light.setLight();
    updateGL();
}

void Viewer::setDiffuseB(int v){
    float val = v/255.;
    light.diffuse[2] = val;
    light.setLight();
    updateGL();
}

//Specular
void Viewer::setSpecularR(int v){
    float val = v/255.;
    light.specular[0] = val;
    light.setLight();
    updateGL();
}

void Viewer::setSpecularG(int v){
    float val = v/255.;
    light.specular[1] = val;
    light.setLight();
    updateGL();
}

void Viewer::setSpecularB(int v){
    float val = v/255.;
    light.specular[2] = val;
    light.setLight();
    updateGL();
}

//lightPosition
void Viewer::setPositionX(double val){
    light.position[0] = val;
    light.setLight();
    updateGL();
}

void Viewer::setPositionY(double val){
    light.position[1] = val;
    light.setLight();
    updateGL();
}

void Viewer::setPositionZ(double val){
    light.position[2] = val;
    light.setLight();
    updateGL();
}

void Viewer::setPositionW(double val){
    light.position[3] = val;
    light.setLight();
    updateGL();
}

void Viewer::setShininess(int val){
    mat.shininess = val;
    mat.setMaterial();
    updateGL();
}

/*
QString Viewer::updateTlabel(){
    //if (task_id == TASK0)
     return QString::number(curTerr)+"/"+QString::number(terrains.size());
}
*/
bool Viewer::moveNext(){
    //qWarning("next %d!n",task_id);
    switch(task_id){

    case TASK0:
        {
            if (curTerr<terrains.size()-1){
                curTerr+=1;
                updateGL();
                emit updateTasklabel(QString::number(curTerr+1)+"/"+QString::number(terrains.size()));
                return true;
            }
            return false;
        }

    case TASK1:
        {
            endsubTask1();
            //qWarning("task ended!n");
            startsubTask1();
            updateGL();
            return true;
        }

    case TASK2:
        {
            if (terrains.size()>0){
                DeleteTerrain(curTerr);
                curTerr -= 1;

            }
                if (task_pos+1<task_files.size()){
                    task_pos+=1;
                    addTerrain(task_files[task_pos]);
                    emit updateTasklabel(QString::number(task_pos+1)+"/"+QString::number(task_files.size()));
                }
                else{

                    qWarning("Task 2  ended");
                    DeleteAll();
                    task_status = 1;
                    task_id = TASK0;
                    emit updateIDlabel("TASK "+QString::number(task_id));
                    emit updateTasklabel(QString::number(curTerr+1)+"/"+QString::number(terrains.size()));
                    emit taskIsNot0(false);
                    emit infoChanged(task0_info);

                    QMessageBox msgBox;
                    msgBox.setText("Thank you for participating to this experiment!\n  Let me know that you are done.");
                    msgBox.exec();

                }

                updateGL();


                return false;
        }

    }
    return false;
}

bool Viewer::movePrevious(){
    switch(task_id){

    case TASK0:
        {
            if (curTerr>0){
                curTerr-=1;
                updateGL();
                emit updateTasklabel(QString::number(curTerr+1)+"/"+QString::number(terrains.size()));
                return true;
            }
            return false;
        }

    case TASK1:
        {
            return false;
        }

    case TASK2:
        {
            return false;
        }

    }
    return true;
}

bool Viewer::moveUp(){
    int rank = rankings.indexOf(curTerr);
    if (rank!=0){
        int cur_owner = rankings[rank-1];
        rankings[rank-1] = curTerr;
        rankings[rank] = cur_owner;
    }
    //qWarning("%s ranked %d",terrains[rankings[0]]->filename.toStdString().c_str(),0);
    //qWarning("%s ranked %d",terrains[rankings[1]]->filename.toStdString().c_str(),1);
    //qWarning("%s ranked %d",terrains[rankings[2]]->filename.toStdString().c_str(),2);

    rankingChanged();

    return true;
}

bool Viewer::moveDown(){
    int rank = rankings.indexOf(curTerr);
    if (rank!=2){
        int cur_owner = rankings[rank+1];
        rankings[rank+1] = curTerr;
        rankings[rank] = cur_owner;
    }
    //qWarning("%s ranked %d",terrains[rankings[0]]->filename.toStdString().c_str(),0);
    //qWarning("%s ranked %d",terrains[rankings[1]]->filename.toStdString().c_str(),1);
    //qWarning("%s ranked %d",terrains[rankings[2]]->filename.toStdString().c_str(),2);

    emit rankingChanged();

    return true;
}

void Viewer::setCurrentTerrain(int row){
    curTerr = rankings[row];
    updateGL();
}

