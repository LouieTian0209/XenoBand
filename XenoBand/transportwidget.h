#ifndef TRANSPORTWIDGET_H
#define TRANSPORTWIDGET_H

#include <QDialog>

#include "transportcore.h"

namespace Ui {
class TransportWidget;
}

class TransportWidget : public QDialog
{
    Q_OBJECT

public:
    explicit TransportWidget(QWidget *parent = 0);
    ~TransportWidget();

    void setTransportCore(TransportCore *core);

signals:
    void notifyPlay();

    void notifyRewind();

    void notifyPause();

    void notifyRecord();

private slots:
    void on_playButton_clicked();

    void on_recordButton_clicked();

    void on_rewindButton_clicked();

    void on_pauseButton_clicked();

private:
    Ui::TransportWidget *ui;

    TransportCore *transportCore;
};

#endif // TRANSPORTWIDGET_H
