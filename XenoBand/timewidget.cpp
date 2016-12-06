#include "core.h"

#include "timewidget.h"
#include "ui_timewidget.h"

TimeWidget::TimeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimeWidget)
{
    ui->setupUi(this);
}

TimeWidget::~TimeWidget()
{
    delete ui;
}

void TimeWidget::onTimeout()
{
    displayTime(transportCore->getTime());
    displayPosition(transportCore->getPosition());
}

void TimeWidget::setTime(QString time)
{
    ui->time_lineEdit->setText(time);

}

void TimeWidget::setTempo(QString tempo)
{
    ui->tempo_lineEdit->setText(tempo);
}

void TimeWidget::setTransportCore(TransportCore *core)
{
    this->transportCore = core;

    connect(transportCore, SIGNAL(notifyTimeout()), this, SLOT(onTimeout()));
    connect(transportCore, SIGNAL(notifyRecord()), this, SLOT(onTimeout()));
}

void TimeWidget::displayTime(int time)
{
    int ms = time%1000;
    time/=1000;
    int ss = time%60;
    time/=60;
    int mm = time;
    char buffer[10];
    sprintf(buffer, "%02d:%02d.%03d", mm, ss, ms);
    ui->time_lineEdit->setText(buffer);
}

void TimeWidget::displayPosition(int position)
{
    int tick = position % 24; // XXX
    position/= 24; // XXX
    int beat = position % 4; // XXX
    position/= 4; // XXX
    int bar = position;
    char buffer[20];
    sprintf(buffer, "%d %d %d", bar+1, beat+1, tick);
    ui->pos_lineEdit->setText(buffer);
}

void TimeWidget::on_tempo_lineEdit_textChanged(const QString &text)
{
    transportCore->setTempo(text.toInt());
}
