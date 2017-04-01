#ifndef MIDIEDITOR_H
#define MIDIEDITOR_H

#include <QApplication>
#include <QBrush>
#include <QCloseEvent>
#include <QColor>
#include <QColorDialog>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>
#include <QMouseEvent>
#include <QScrollBar>
#include <QString>
#include <QTransform>
#include <QWidget>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <stdlib.h>
#include <string>

#include "midieditorcore.h"
#include "midireadercore.h"
#include "keyboard.h"
#include "soundmappingcore.h"
#include "transportwidget.h"
#include "transportcore.h"
#include "timewidget.h"

namespace Ui {
class MIDIEditor;
}

class MIDIEditor : public QWidget
{
    Q_OBJECT

public:
    explicit MIDIEditor(QWidget *parent = 0);
    ~MIDIEditor();
    QGraphicsLineItem *cursor;
    MIDIEditorCore *midiTrackCore;
    int playIndex;
    void setColor(QColor color);
    QColor getColor();
    inline TransportCore *getTransportCore() { return transportCore; }

    QString instname;

    inline MIDIReaderCore *getMIDIReaderCore()
    {
        if (readerCore == NULL) qDebug() << "MIDIEditor: reader core is null" << endl;
        return readerCore;
    }

    QGraphicsScene *getQGraphicsScene();

    void setInstrument(QString instname);

private slots:
    void on_timeout();

    void on_record();

    void on_MIDI_received(int note);

    void on_lengthBox_valueChanged(int arg1);

    void on_lengthSlider_valueChanged(int value);

    void on_pitchBox_valueChanged(int arg1);

    void on_pitchSlider_valueChanged(int value);

    void on_addNoteButton_clicked();

    void on_removeNoteButton_clicked();

    //void dragMoveEvent(QDragMoveEvent *event);

    void on_velocitySlider_valueChanged(int value);

    void on_velocityBox_valueChanged(int value);

    void on_loadButton_clicked();

    /* draw note (rectangle) onto graphics scene */
    void drawRect(int pos, int pitch, int length, int velocity);

    /* draw cursor onto graphics scene */
    void drawCursor();

    /* clear the scene */
    void clear();

    // void setCursorWithMouse(QMouseEvent *mouseEvent);

    void mousePressEvent(QMouseEvent *e);

    /* save file onto local file system */
    void on_saveButton_clicked();

    void closeEvent(QCloseEvent *);

    void on_colorButton_clicked();

    void on_zoomXBox_valueChanged(int value);

    void on_zoomYBox_valueChanged(int value);

    void on_keyboardButton_clicked();

    // [YZ]
    void onInstChanged(QString instname);

private:
    Ui::MIDIEditor *ui;
    QGraphicsScene *scene;
    TransportWidget *transportWidget;
    TimeWidget *timeWidget;
    TransportCore *transportCore;
    QMouseEvent *mEvent;
    QColor noteColor;
    Keyboard *keyboard;
    MIDIReaderCore *readerCore;

    SoundMappingCore *smCore;

    int xPerLength;
    int yPerPitch;
    int scaleX;
    int scaleY;
};

#endif // MIDITRACKEDITOR_H
