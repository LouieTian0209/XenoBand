#include "pathimport.h"
#include "ui_pathimport.h"
#include "soundmapping.h"
PathImport::PathImport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PathImport)
{
    ui->setupUi(this);
    ui->InstrumetList->addItem("Piano");
    piano=false;
}

PathImport::~PathImport()
{
    delete ui;
}

void PathImport::on_InstrumetList_itemDoubleClicked(QListWidgetItem *item)
{
    if(item->text()=="Piano"){
        piano=true;
        showFilePath(item->text());
    }
}
void PathImport::showFilePath(QString instr){
    ui->FilePathList->addItem("piano files");
}

void PathImport::on_pushButton_clicked()
{
    SoundMapping *sm=new SoundMapping;
    sm->LoadFilePath();
}
