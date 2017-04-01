#ifndef WAVEEDITOR_H
#define WAVEEDITOR_H

#include <math.h>

#include <QColor>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QWidget>
#include <QScrollBar>

#include "core.h"
#include "timewidget.h"
#include "transportwidget.h"
#include "transportcore.h"
#include "wavereadercore.h"
#include "wavewidget.h"

namespace Ui {
class WaveEditor;
}

class WaveEditor : public QWidget
{
    Q_OBJECT

public:
    explicit WaveEditor(QWidget *parent = 0);
    ~WaveEditor();

    int playIndex;

    WaveReaderCore *waveReaderCore;

    QGraphicsScene *getQGraphicsScene();

private slots:
    /* handle load file event when load button is clicked */
    void on_loadButton_clicked();

    /* handle delete wave event when delete button is clicked */
    void on_deleteButton_clicked();

    /* handle copy wave event when copy button is clicked */
    void on_copyButton_clicked();

    /* handle cut wave event when cut button is clicked */
    void on_cutButton_clicked();

    /* handle paste wave event when paste button is clicked */
    void on_pasteButton_clicked();

    /* zoom wave on x direction for given multiplier */
    void on_xZoomBox_valueChanged(int);

    /* zoom wave on y direction for given multiplier */
    void on_yZoomBox_valueChanged(int);

    void onPlay();

    /* handle select wave event when select button is clicked */
    void on_selectButton_clicked();

    /* handle save file event when save button is clicked */
    void on_saveButton_clicked();

    /* handle shift cut (cut selected area and shift the rest accordingly) event when shift cut button is clicked */
    void on_shiftCutButton_clicked();

    /* handle shift paste (paste at selected start time and shift the rest accordingly) event when shift paste button is clicked */
    void on_shiftPasteButton_clicked();

    /* handle color picking event using system color picker */
    void on_colorButton_clicked();

    void on_fadeButton_clicked();

private:
    Ui::WaveEditor *ui;

    WaveWidget *waveWidget;

    vector<int> buffer;

    TransportCore *transportCore;

    TransportWidget *transportWidget;

    TimeWidget *timeWidget;

    vector<int> cbuffer;

    double baseRatioX;

    double baseRatioY;

    double base;

    QGraphicsLineItem *cursor;

    /* general function to handle display update */
    void updateDisplay();

    /* draw cursor onto graphics scene */
    void drawCursor();

    /* clear the graphics scene */
    void clear();

    int height;
};

#endif // WAVEEDITOR_H
