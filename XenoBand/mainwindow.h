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

    // test
    void on_actionNew_triggered();

    // tutorial link near XenoBand
    void on_tutorialButton_clicked();

    // MIDI menu
    void on_actionMIDI_Reader_triggered();
    void on_actionMIDI_Writer_triggered();
    void on_actionMIDI_Editor_triggered();

    // wave menu
    void on_actionWave_Reader_triggered();
    void on_actionWave_Writer_triggered();
    void on_actionWave_Editor_triggered();

    // help menu
    void on_actionTutorial_triggered();
    void on_actionContact_triggered();

    // first column
    void on_synthesizerButton_clicked();
    void on_waveCreatorButton_clicked();
    void on_recorderButton_clicked();
    void on_pitchShifterButton_clicked();
    void on_waveEditorButton_clicked();

    // second column
    void on_mappingButton_clicked();
    void on_keyboardButton_clicked();
    void on_loopButton_clicked();
    void on_samplesButton_clicked();
    void on_editorButton_clicked();

    // third column
    void on_localServerButton_clicked();
    void on_instantChatButton_clicked();

    void closeEvent(QCloseEvent *);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
