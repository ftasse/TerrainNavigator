#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->wirefr_checkBox, SIGNAL(clicked(bool)), ui->viewer, SLOT(setWireframe(bool)) );
    connect( ui->skybox_checkBox, SIGNAL(clicked(bool)), ui->viewer, SLOT(setSkybox(bool)) );
    connect( ui->texture_checkBox, SIGNAL(clicked(bool)), ui->viewer, SLOT(setTextured(bool)) );

    connect( ui->enableLight_box, SIGNAL(clicked(bool)), ui->viewer, SLOT(enableLight(bool)) );
    connect( ui->enableGLSL_box, SIGNAL(clicked(bool)), ui->viewer, SLOT(enableGLSL(bool)) );

    ui->bgR_box->setValue( 255*ui->viewer->backgroundCol.x());
    connect(ui->bgR_box, SIGNAL(valueChanged(int)), ui->viewer, SLOT(setBackgroundR(int)));

    ui->bgG_box->setValue( 255.*ui->viewer->backgroundCol.y());
    connect(ui->bgG_box, SIGNAL( valueChanged(int)), ui->viewer, SLOT(setBackgroundG(int)));

    ui->bgB_box->setValue( 255.*ui->viewer->backgroundCol.z());
    connect(ui->bgB_box, SIGNAL(valueChanged(int)), ui->viewer, SLOT(setBackgroundB(int)));

    //Ambient
    ui->ambR_box->setValue( 255*ui->viewer->light.ambient[0] );
    connect(ui->ambR_box, SIGNAL(valueChanged(int)), ui->viewer, SLOT(setAmbientR(int)));

    ui->ambG_box->setValue( 255*ui->viewer->light.ambient[1] );
    connect(ui->ambG_box, SIGNAL(valueChanged(int)), ui->viewer, SLOT(setAmbientG(int)));

    ui->ambB_box->setValue( 255*ui->viewer->light.ambient[2] );
    connect(ui->ambB_box, SIGNAL(valueChanged(int)), ui->viewer, SLOT(setAmbientB(int)));

    //Diffuse
    ui->difR_box->setValue( 255*ui->viewer->light.diffuse[0] );
    connect(ui->difR_box, SIGNAL(valueChanged(int)), ui->viewer, SLOT(setDiffuseR(int)));

    ui->difG_box->setValue( 255*ui->viewer->light.diffuse[1] );
    connect(ui->difG_box, SIGNAL(valueChanged(int)), ui->viewer, SLOT(setDiffuseG(int)));

    ui->difB_box->setValue( 255*ui->viewer->light.diffuse[2] );
    connect(ui->difB_box, SIGNAL(valueChanged(int)), ui->viewer, SLOT(setDiffuseB(int)));

    //Specular
    ui->spcR_box->setValue( 255*ui->viewer->light.specular[0] );
    connect(ui->spcR_box, SIGNAL(valueChanged(int)), ui->viewer, SLOT(setSpecularR(int)));

    ui->spcG_box->setValue( 255*ui->viewer->light.specular[1] );
    connect(ui->spcG_box, SIGNAL(valueChanged(int)), ui->viewer, SLOT(setSpecularG(int)));

    ui->spcB_box->setValue( 255*ui->viewer->light.specular[2] );
    connect(ui->spcB_box, SIGNAL(valueChanged(int)), ui->viewer, SLOT(setSpecularB(int)));

    //LightPosition
    ui->lPosX_box->setValue( ui->viewer->light.position[0] );
    connect(ui->lPosX_box, SIGNAL(valueChanged(double)), ui->viewer, SLOT(setPositionX(double)));

    ui->lPosY_box->setValue( ui->viewer->light.position[1] );
    connect(ui->lPosY_box, SIGNAL(valueChanged(double)), ui->viewer, SLOT(setPositionY(double)));

    ui->lPosZ_box->setValue( ui->viewer->light.position[2] );
    connect(ui->lPosZ_box, SIGNAL(valueChanged(double)), ui->viewer, SLOT(setPositionZ(double)));

    ui->lPosW_box->setValue( ui->viewer->light.position[3] );
    connect(ui->lPosW_box, SIGNAL(valueChanged(double)), ui->viewer, SLOT(setPositionW(double)));

    //Shininess
    ui->mshininess->setValue( ui->viewer->mat.shininess);
    connect(ui->mshininess, SIGNAL(valueChanged(int)), ui->viewer, SLOT(setShininess(int)));

    ui->task1_widget->setHidden(true);

    //ui->Scene->setHidden(true);
    //ui->Lights->setHidden(true);
    ui->tabWidget->setTabEnabled(1,true);
    ui->tabWidget->setTabEnabled(2,true);

    //Subtask
    //ui->prevSubTask->setEnabled(true);
    connect(ui->nextSubTask, SIGNAL(pressed()), ui->viewer, SLOT(moveNext()) );
    connect(ui->prevSubTask, SIGNAL(pressed()), ui->viewer, SLOT(movePrevious()) );

    connect(ui->viewer, SIGNAL(updateTasklabel(QString)), ui->subtask_index_label, SLOT(setText(QString)) );
    connect(ui->viewer, SIGNAL(updateIDlabel(QString)), ui->task_label, SLOT(setText(QString)) );

    connect(ui->moveTerUp, SIGNAL(pressed()), ui->viewer, SLOT(moveUp()) );
    connect(ui->moveTerDown, SIGNAL(pressed()), ui->viewer, SLOT(moveDown()) );

    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), ui->viewer, SLOT(setCurrentTerrain(int)));

    connect(ui->viewer, SIGNAL(rankingChanged()), this, SLOT(changeItemsname()));
    connect(ui->viewer, SIGNAL(taskIsNot1(bool)), ui->task1_widget, SLOT(setHidden(bool)));
    connect(ui->viewer, SIGNAL(taskIsNot0(bool)), ui->prevSubTask, SLOT(setDisabled(bool)));

    ui->textEdit->setText(ui->viewer->task0_info);
    connect(ui->viewer, SIGNAL(infoChanged(QString)), ui->textEdit, SLOT(setText(QString)));

    connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(load()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exit()));
    connect(ui->actionDelete, SIGNAL(triggered()), this, SLOT(del()));
    connect(ui->actionReset, SIGNAL(triggered()), this, SLOT(reset()));
    connect(ui->actionDocumentation, SIGNAL(triggered()), this, SLOT(doc()));
    connect(ui->actionLoad_Experiment, SIGNAL(triggered()), ui->viewer, SLOT(LoadExperiment()));

    connect(ui->loadTexture_button, SIGNAL(clicked()), ui->viewer, SLOT(LoadTexture()));
}

void MainWindow::load(){
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
            ui->viewer->addTerrain(fileName);
            ui->viewer->updateGL();
        }

}

void MainWindow::save(){
    ui->viewer->saveSnapshot(false, false);
}

void MainWindow::doc(){
    ui->viewer->help();
}

void MainWindow::exit(){
    qApp->closeAllWindows();
}

void MainWindow::del(){
    ui->viewer->DeleteTerrain(ui->viewer->curTerr);
    ui->viewer->updateGL();
}

void MainWindow::reset(){
    ui->viewer->DeleteAll();
    ui->viewer->updateGL();
}

void MainWindow::changeItemsname(){
    int cur_row = ui->viewer->rankings.indexOf(ui->viewer->curTerr);
    for (int r=0; r< ui->listWidget->count(); r++){
        int owner = ui->viewer->rankings[r];
        QString  text;
        if (owner==0)   text = "LandscapeA";
        else if (owner==1)  text = "LandscapeB";
        else text = "LandscapeC";
        ui->listWidget->item(r)->setText(text);
    }
    ui->listWidget->setCurrentRow(cur_row);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

//hello

