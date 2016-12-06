#include "timeinfo.h"
#include "ui_timeinfo.h"

TimeInfo::TimeInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TimeInfo)
{
    ui->setupUi(this);
    ui->spinBox_metronome->setRange(40,208);
    ui->dial_metronome->setRange(40,208);
    /* can use picture in the future */
    ui->label_b1->setText("O");
    ui->label_b2->setText("X");
    ui->label_empty->setText("<<<<<");
    ui->label_b1->setVisible(true);
    ui->label_b2->setVisible(false);

    ui->min_lineEdit->setAlignment(Qt::AlignHCenter);
    ui->sec_lineEdit->setAlignment(Qt::AlignHCenter);
    ui->ms_lineEdit->setAlignment(Qt::AlignHCenter);
}

TimeInfo::~TimeInfo()
{
    delete ui;
}

void TimeInfo::beat(int beat)
{
    if(beat%2==0){
        cerr << 1 << endl;
        ui->label_b1->setVisible(true);
        ui->label_b2->setVisible(false);
        ui->label_empty->setText("<<<<<");
    }
    else{
        cerr << 0 << endl;
        ui->label_b1->setVisible(false);
        ui->label_b2->setVisible(true);
        ui->label_empty->setText(">>>>>");
    }
}

void TimeInfo::setTime(int pos)
{
    QString time = timeFormat(pos);
    QStringList myStringList = time.split(" :");

    ui->min_lineEdit->setText(myStringList.at(0));
    ui->sec_lineEdit->setText(myStringList.at(1));
    ui->ms_lineEdit->setText(myStringList.at(2));
}

void TimeInfo::setTempo(int nominator, int denominator)
{
    char buffer[1],sigbuffer[4];
    sigbuffer[3]='\0';
    sprintf(buffer, "%d", nominator);
    strcat(sigbuffer, buffer);
    strcat(sigbuffer, "/");
    sprintf(buffer, "%d", denominator);
    strcat(sigbuffer, buffer);
    ui->label_signature->setText(QString(sigbuffer));
    strcpy(sigbuffer,"");
    strcat(sigbuffer, "0/");
    strcat(sigbuffer, buffer);
    ui->label_signature_now->setText(QString(sigbuffer));
}

/* duration of  */
void TimeInfo::setDuration(int dur)
{
    ui->timeSlider->setRange(0, dur);
    ui->label_duration->setText("Duration: "+timeFormat(dur));
}
