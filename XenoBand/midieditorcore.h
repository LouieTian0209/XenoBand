#ifndef MIDIEDITORCORE_H
#define MIDIEDITORCORE_H

#include "core.h"
#include "midinoteevent.h"

#include <QGraphicsRectItem>

class MIDIEditorCore
{
public:
    MIDIEditorCore();

    QList<MIDINoteEvent *> noteEventList;

    /* binary search for position and then insert */
    void insertNoteEvent(MIDINoteEvent *event);

    /* binary search for position and then remove */
    void removeNoteEvent(MIDINoteEvent *event);

    /* Removes the note event on given index in note event list */
    void removeNoteEvent(int index);

    /* remove all events */
    void clear();

    /* Returns the appropriate index to insert into note event list for given start position */
    int getIndex(int position);

    /* Returns an on event corresponding to given MIDI note */
    MIDINoteEvent* getOnEvent(QGraphicsRectItem *item);

    /* Returns an off event corresponding to given MIDI note */
    MIDINoteEvent* getOffEvent(QGraphicsRectItem *item);

private:
    /* Fixed size of the height of each note representation */
    int yPerPitch;
};

#endif // MIDITRACKCORE_H
