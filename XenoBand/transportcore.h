#ifndef TRANSPORTCORE_H
#define TRANSPORTCORE_H

#include <QString>
#include <QStringList>
#include <QObject>
#include <QTimer>

class TransportCore : public QObject
{
    Q_OBJECT

public:
    TransportCore();

    int setTime(int min, int sec, int ms);
    int getTime();
    int setPosition(int position);
    int setPosition(int bar, int beat);
    int getPosition();

    int setTempo(int tempo);
    int getTempo();

    int play();
    int record();
    int stop();
    int rewind();

public slots:
    void onTimeout();
    int setTime(int ms);

signals:
    void notifyTimeout();
    void notifyRecord();
    void notifyRewind();

private:
    int time;
    int tempo;
    int division; // tick; per quarter note
    int period; // update period of timer
    int position; // total ticks

public:
    bool recording;
    QTimer *timer;
};

#endif // TRANSPORTCORE_H
