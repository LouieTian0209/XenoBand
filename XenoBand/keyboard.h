#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QWidget>
#include <QKeyEvent>
#include <QDebug>
#include <QtCore>
#include <QSettings>
#include <QLineEdit>
#include <QTimer>
#include "core.h"
#include "soundmappingcore.h"

namespace Ui {
class Keyboard;
}

class Keyboard : public QWidget
{
    Q_OBJECT

public:
    explicit Keyboard(QWidget *parent = 0);
    ~Keyboard();

    void setInstrument(QString instname);

signals:
    void sendMIDI(int note);
   // void on_Octave_valueChanged(int arg1);

    void instChanged(QString instname);

private slots:

    void keyPressEvent(QKeyEvent *keyevent);
    void keyReleaseEvent(QKeyEvent *keyevent);

    void playSound(int note);

    void on_pushButton_C_pressed();

    void on_pushButton_D_pressed();

    void on_pushButton_E_pressed();

    void on_pushButton_F_released();

    void on_pushButton_F_pressed();

    void on_pushButton_G_pressed();

    void on_pushButton_A_pressed();

    void on_pushButton_B_pressed();

    void on_pushButton_C2_pressed();

    void on_pushButton_D2_pressed();

    void on_pushButton_E2_pressed();

    void on_pushButton_F2_pressed();

    void on_pushButton_C_released();

    void on_pushButton_D_released();

    void on_pushButton_E_released();

    void on_pushButton_G_released();

    void on_pushButton_A_released();

    void on_pushButton_B_released();

    void on_pushButton_C2_released();

    void on_pushButton_D2_released();

    void on_pushButton_E2_released();

    void on_pushButton_F2_released();

    void on_octave_valueChanged(int value);

    void on_comboBox_activated(const QString &name);

private:
    Ui::Keyboard *ui;

    SoundMappingCore *smCore;

    QKeyEvent *keyevent;
    int octave;

    QTimer *timer;

    int soundlength;
};

#endif // KEYBOARD_H
