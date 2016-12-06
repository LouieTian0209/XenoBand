#ifndef TIMEWIDGET_H
#define TIMEWIDGET_H

#include <QWidget>
#include <QDialog>

#include "transportcore.h"

namespace Ui {
class TimeWidget;
}

class TimeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TimeWidget(QWidget *parent = 0);
    ~TimeWidget();

    void setTransportCore(TransportCore *core);
    void displayTime(int time);
    void displayPosition(int position);
    void setTime(QString time);

    void setTempo(QString tempo);

private:
    Ui::TimeWidget *ui;

    TransportCore *transportCore;

public slots:
    void onTimeout();

private slots:
    void on_tempo_lineEdit_textChanged(const QString &arg1);
};

#endif // TIMEWIDGET_H
