#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include <QWidget>

#include "wavewidget.h"

namespace Ui {
class Synthesizer;
}

class Synthesizer : public QWidget
{
    Q_OBJECT

public:
    explicit Synthesizer(QWidget *parent = 0);
    ~Synthesizer();

    void updateResultFunctionDisplay();

private slots:
    void on_addOscillatorButton_clicked();

    void on_playButton_clicked();

    // void on_exportScaleButton_clicked();

    void on_waveWriterButton_clicked();

    void on_clearButton_clicked();

private:
    Ui::Synthesizer *ui;

    WaveWidget *resultWaveWidget;

    void closeEvent(QCloseEvent *);
};

#endif
