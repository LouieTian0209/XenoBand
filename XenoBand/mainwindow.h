#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMessageBox>
#include <QTime>
#include <QTimer>
#include <QSlider>
#include <QTextCodec>
#include <QThread>
#include <QPixmap>
#include <QStringList>

#include "audiorecorder.h"
#include "core.h"
#include "instantchat.h"
#include "keyboard.h"
#include "librarywindow.h"
#include "midireader.h"
#include "midiwriter.h"
#include "keyboard.h"
#include "soundmapping.h"
#include "synthesizer.h"
#include "timewidget.h"
#include "trackpreview.h"
#include "transportwidget.h"
#include "transportcore.h"
#include "chatroom.h"

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void updateInfo(TrackPreview *preview);

private slots:

    void on_actionNew_triggered();
    void on_actionMIDI_Reader_triggered();
    void on_actionMIDI_Writer_triggered();
    void on_actionWave_Reader_triggered();
    void on_actionWave_Writer_triggered();
    void on_actionPitch_Shifter_triggered();
    void on_actionAudio_Recorder_triggered();
    void on_actionConnect_triggered();
    void on_actionSound_Synthesizer_triggered();
    void on_actionInstant_Chat_triggered();

    void on_actionWave_Editor_triggered();
    void on_actionMIDI_Editor_triggered();

    void on_actionKeyboard_triggered();

    void on_actionSound_Mapping_triggered();

    void on_actionLoop_triggered();

    void on_synthesizerButton_clicked();

    void on_waveCreatorButton_clicked();

    void on_recorderButton_clicked();

    void on_pitchShifterButton_clicked();

    void on_waveEditorButton_clicked();

    void on_mappingButton_clicked();

    void on_keyboardButton_clicked();

    void on_loopButton_clicked();

    void on_samplesButton_clicked();

    void on_editorButton_clicked();

    void on_localServerButton_clicked();

    void on_globalServerButton_clicked();

    void on_instantChatButton_clicked();

    void on_tutorialButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
