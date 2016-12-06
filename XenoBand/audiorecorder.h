#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QAudioRecorder>
#include <QTimer>
#include <QWidget>

#include "core.h"

namespace Ui {
class AudioRecorder;
}

class AudioRecorder : public QWidget
{
    Q_OBJECT

public:
    explicit AudioRecorder(QWidget *parent = 0);
    ~AudioRecorder();

private slots:
    void on_recordButton_clicked();

    void timeout();

private:
    Ui::AudioRecorder *ui;

    QAudioRecorder *recorder;

    QTimer *timer;

    int time; // elapsed time since recording started

    bool recording;
};

#endif // AUDIORECORDER_H
