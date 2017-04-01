/*

void setTime(int minutes, int seconds, int milliseconds);

int getTime() / return time in milliseconds /

void setTempo(int tempo)

int getTempo()

void setPosition(int bar, int beat);

void getPosition();

*/
#include "transportcore.h"
#include <iostream>

using namespace std;

TransportCore::TransportCore()
{
    /* something */
    division = 24;
    position = 0;
    recording = false;
    tempo = 120;
    time = 0;
    period = (60000 / tempo) / division;

    timer = new QTimer();

    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

/* public method */
int TransportCore::setTime(int ms)
{
    this->time = ms;
    emit notifyTimeout();
    return 0;
}

int TransportCore::getTime()
{
    return time;
}

int TransportCore::setTempo(int tempo)
{
    this->tempo = tempo;
    this->period = (60000 / tempo) / division;

    if (timer->isActive())
        timer->start(period);

    return 0;
}

int TransportCore::getTempo()
{
    return tempo;
}

int TransportCore::setPosition(int position)
{
    this->position = position;
    emit notifyTimeout();
    return 0;
}

int TransportCore::setPosition(int bar, int beat)
{
    setPosition( (bar-1)*4*24 + (beat-1)*24); // XXX nominator is 4, division is 24
    return 0;
}

int TransportCore::getPosition()
{
    return position;
}

int TransportCore::play()
{
    recording = false;
    timer->start(period);
    return 0;
}

int TransportCore::record()
{
    recording = true;
    timer->start(period);
    return 0;
}

int TransportCore::stop()
{
    recording = 0;
    timer->stop();

    // emit notifyTimeout();
    return 0;
}

int TransportCore::rewind()
{
    recording = 0;
    timer->stop();

    emit notifyRewind();
    setPosition(0);
    return 0;
}

void TransportCore::onTimeout()
{
    time+=period;
    position++;

    emit (recording ? notifyRecord() : notifyTimeout());
}
