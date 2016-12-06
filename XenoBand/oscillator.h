#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <QWidget>

#include "wavewidget.h"

class Synthesizer;

namespace Ui {
class Oscillator;
}

class Oscillator : public QWidget
{
    Q_OBJECT

public:
    explicit Oscillator(QWidget *parent = 0, Synthesizer *synthesizer = 0);
    ~Oscillator();

    double oscillatorFunction(double x);

    Synthesizer* synthesizer;

private slots:
    void on_amplitudeBox_valueChanged(double value);

    void on_ratioBox_valueChanged(double value);

private:
    Ui::Oscillator *ui;

    WaveWidget *waveWidget;

    void updateFunctionDisplay(double ampVal, double ratioVal);
};

#endif // OSCILLATOR_H
