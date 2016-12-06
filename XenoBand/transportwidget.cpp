#include "transportwidget.h"
#include "ui_transportwidget.h"
#include "core.h"

TransportWidget::TransportWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransportWidget)
{
    ui->setupUi(this);
}

TransportWidget::~TransportWidget()
{
    delete ui;
}

void TransportWidget::on_playButton_clicked()
{
    transportCore->play();

    emit notifyPlay();
}

void TransportWidget::on_recordButton_clicked()
{
    transportCore->record();

    emit notifyRecord();
}

void TransportWidget::setTransportCore(TransportCore *core)
{
    this->transportCore = core;
}

void TransportWidget::on_rewindButton_clicked()
{
    transportCore->stop();
    transportCore->setTime(0);
    transportCore->setPosition(0);
    ui->recordButton->setEnabled(true);
    emit notifyRewind();
}

void TransportWidget::on_pauseButton_clicked()
{
    transportCore->stop();
    emit notifyPause();
}
