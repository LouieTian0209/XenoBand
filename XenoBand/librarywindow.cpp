#include <iostream>

#include "core.h"

#include "librarywindow.h"
#include "ui_librarywindow.h"

#include "synthesizer.h"
#include "ui_synthesizer.h"

using namespace std;

LibraryWindow::LibraryWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LibraryWindow)
{
    ui->setupUi(this);

    QSettings setting("XenoBand","sound mapping");

    setting.beginGroup("mappingName");
    foreach(const QString &name,setting.allKeys()){
        ui->listWidget->addItem(name);
    }
    setting.endGroup();
}

LibraryWindow::~LibraryWindow()
{
    delete ui;
}

void LibraryWindow::on_createButton_clicked()
{
    Synthesizer *sw = new Synthesizer;
    sw->show();
}
