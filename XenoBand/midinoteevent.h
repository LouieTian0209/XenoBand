#ifndef MIDINOTEEVENT_H
#define MIDINOTEEVENT_H

#include "core.h"

class MIDINoteEvent
{
public:
    MIDINoteEvent(int position, int onoff, int note, int velocity);

    MIDINoteEvent *sibling;

    int position; // in ticks
    int onoff;
    int note;
    int velocity;
};

// [YZ] This works just like Java toString
ostream& operator<<(ostream &os, const MIDINoteEvent &event);

#endif // MIDINOTEEVENT_H
