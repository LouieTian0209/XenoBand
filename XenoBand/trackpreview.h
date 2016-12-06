#ifndef TRACKWINDOW_H
#define TRACKWINDOW_H

#include <QWidget>

#include "midieditor.h"
#include "waveeditor.h"

namespace Ui {
class TrackPreview;
}

class TrackPreview : public QWidget
{
    Q_OBJECT

public:
    explicit TrackPreview(QWidget *parent = 0, bool isWaveTrack = false);
    ~TrackPreview();

    void setMIDITrack();
    void setWaveTrack();

    MIDIEditor *editorMIDI;
    WaveEditor *editorWave;
    bool isWaveTrack;

    static TrackPreview *activeTrackPreview;

signals:
    void activatePreview();

private slots:
    void on_openButton_clicked();

    void on_infoButton_clicked();

private:
    Ui::TrackPreview *ui;

};

#endif // TRACKWINDOW_H
