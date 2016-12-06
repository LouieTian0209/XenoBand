#ifndef PITCHSHIFTER_H
#define PITCHSHIFTER_H

#include <QWidget>

#include "core.h"
#include "wavereadercore.h"
#include "wavewidget.h"
#include "wavewritercore.h"

namespace Ui {
class PitchShifter;
}

class PitchShifter : public QWidget
{
    Q_OBJECT

public:
    explicit PitchShifter(QWidget *parent = 0);
    ~PitchShifter();

     // [YZ] This should be a core part
    static vector<int> pitchShift(vector<int> &, double ratio);

private slots:
    void on_importButton_clicked();

    void on_shiftButton_clicked();

    void on_playButton_clicked();

    void on_scaleButton_clicked();

    void on_zoomInButton_clicked();

    void on_zoomOutButton_clicked();

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::PitchShifter *ui;

    WaveReaderCore *waveReaderCore;

    WaveWidget *waveBeforeWidget;
    WaveWidget *waveAfterWidget;

    vector<int> buffer;
    vector<int> bufferShifted;
};

#endif // PITCHSHIFTER_H
