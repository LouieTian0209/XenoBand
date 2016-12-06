#ifndef WAVEWRITER_H
#define WAVEWRITER_H

#include "core.h"
#include "wavewidget.h"

namespace Ui {
class WaveWriter;
}

class WaveWriter : public QWidget
{
    Q_OBJECT

public:
    explicit WaveWriter(QWidget *parent = 0);
    ~WaveWriter();

    double (*fp) (double);

    double (*getFunctionByIndex(int index)) (double);

    void setFunctionSynthesis(double (*fp) (double));

private slots:
    void on_playButton_clicked();

    void on_ratioAMDial_sliderMoved(int position);

    void on_ratioFMDial_sliderMoved(int position);

    void on_deltaAMDial_sliderMoved(int position);

    void on_deltaFMDial_sliderMoved(int position);

    void on_ratioAM_valueChanged(double arg1);

    void on_ratioFM_valueChanged(double arg1);

    void on_deltaAM_valueChanged(double arg1);

    void on_deltaFM_valueChanged(double arg1);

    void onWaveChanged(int);

    void on_scaleButton_clicked();

    void on_attack_valueChanged(int arg1);

    void on_exportButton_clicked();

private:
    Ui::WaveWriter *ui;

    WaveWidget *waveWidget;

    void saveInstrument(QString &);
};

#endif // WAVEWRITER_H
