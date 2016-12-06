#ifndef LOOP_H
#define LOOP_H

#include <QWidget>

#include <soundmappingcore.h>
#include <transportwidget.h>
#include <transportcore.h>

namespace Ui {
class Loop;
}

class Loop : public QWidget
{
    Q_OBJECT

public:
    explicit Loop(QWidget *parent = 0);
    ~Loop();

    TransportCore *transportCore;
    TransportWidget *transportWidget;

    int playIndex;
    int rootIndex;

    int root;

    void closeEvent(QCloseEvent *);

private:
    Ui::Loop *ui;

    SoundMappingCore *smCore;

    bool initDone;

    void playPattern(QString pattern);

private slots:
    void onPlay();

    void onStop();

    void onTimeout();
    void on_instCombo_currentIndexChanged(const QString &instname);
    void on_tempBox_valueChanged(int arg1);
};

#endif // LOOP_H
