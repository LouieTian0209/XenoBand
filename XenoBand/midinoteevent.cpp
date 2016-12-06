#include "midinoteevent.h"

MIDINoteEvent::MIDINoteEvent(int position, int onoff, int note, int velocity)
{
    this->position = position;
    this->onoff = onoff;
    this->note = note;
    this->velocity = velocity;

    sibling = NULL;
}

ostream& operator<<(ostream &os, const MIDINoteEvent &event)
{
    return os << "{" << event.position << ", " << event.onoff << ", " << event.note << ", " << event.velocity << "}";
}
