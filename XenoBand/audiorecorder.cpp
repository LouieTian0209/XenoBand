#include "audiorecorder.h"
#include "ui_audiorecorder.h"

AudioRecorder::AudioRecorder(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudioRecorder)
{
    ui->setupUi(this);

    recorder = new QAudioRecorder;
    timer = new QTimer;

    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));

    recording = false;
}

AudioRecorder::~AudioRecorder()
{
    delete ui;

    delete recorder;
    delete timer;
}

void AudioRecorder::timeout() {
    this->time += 10;

    // [YZ] use local variable
    int time = this->time;

    int ms = time % 1000;
    time /= 1000;
    int ss = time % 60;
    time /= 60;
    int mm = time;
    char buffer[50];
    sprintf(buffer, "Recording - %02d:%02d.%03d", mm, ss, ms);

    ui->statusLabel->setText(buffer);
}

void AudioRecorder::on_recordButton_clicked()
{
    // if currently recording, then stop recording
    if (recording)
    {
        recorder->stop();

        timer->stop();

        ui->statusLabel->setText("Recorded");

        ui->recordButton->setText("Record");
    }
    // otherwise start recording
    else
    {
        time = 0;

        QAudioEncoderSettings settings;
        settings.setCodec("audio/amr");
        settings.setSampleRate(22050);
        settings.setChannelCount(2);
        settings.setQuality(QMultimedia::NormalQuality);
        settings.setEncodingMode(QMultimedia::ConstantQualityEncoding);

        recorder->setEncodingSettings(settings);
        recorder->setOutputLocation(QUrl::fromLocalFile(QString::fromStdString(Core::defaultWavePath())));
        recorder->record();

        timer->start();

        ui->recordButton->setText("Stop");
    }

    // status change
    recording = !recording;
}
