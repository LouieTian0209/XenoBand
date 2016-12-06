#include "midieditorcore.h"

using namespace std;

MIDIEditorCore::MIDIEditorCore()
{
    yPerPitch = 10;
}

/*
 * Inserts given note event into appropriate position in note event list.
 */
void MIDIEditorCore::insertNoteEvent(MIDINoteEvent* event)
{
    cerr << "insert " << *event << endl;
    int index = getIndex(event->position);
    cerr << " - at index " << index << endl;
    noteEventList.insert(index, event);
}

/*
 * Removes given note event from note event list, if there are multiple qualified
 * events, remove the one that appears first.
 */
void MIDIEditorCore::removeNoteEvent(MIDINoteEvent* event)
{
    QList<MIDINoteEvent*> onCheck; // store pre-qualified events
    QList<int> onIndex; // store index for pre-qualified events
    for(int i = 0; i < noteEventList.size(); i++)
    {
        if(noteEventList.at(i)->position == event->position)
        {
            onCheck.append(noteEventList.at(i));
            onIndex.append(i);
        }
    }
    if(onCheck.size() == 0 || onIndex.size() == 0)
    {
        cerr << "Target event not found." << endl;
        return;
    }
    for(int j = 0; j < onCheck.size(); j++)
    {
        if(onCheck.at(j)->note != event->note || onCheck.at(j)->onoff != event->onoff || onCheck.at(j)->velocity != event->velocity)
        {
            onIndex.replace(j, -1); // remove all indexes for non-identical events
        }
    }
    int len = onIndex.size();
    int pt = 0;
    while(pt < len)
    {
        if(onIndex.at(pt) != -1)
        {
            removeNoteEvent(onIndex.at(pt)); // remove the first identical event
            break;
        }
        pt += 1;
    }
}

/*
 * Removes the note event on given index in note event list.
 */
void MIDIEditorCore::removeNoteEvent(int index)
{
    if(index < 0 || index >= noteEventList.size())
    {
        cerr << "Invalid index." << endl;
        return;
    }
    else
        noteEventList.removeAt(index);
}

/*
 * Returns the appropriate index to insert into note event list for given start position.
 */
int MIDIEditorCore::getIndex(int position)
{
    // This is done using binary search.
    int l = 0, h = noteEventList.size(), m = (l + h) / 2;
    while (m < h)
    {
        m = (l + h) / 2;

        int mpos = noteEventList.at(m)->position;

        if (mpos >= position && (m == 0 || noteEventList.at(m - 1)->position < position))
            return m;
        else if (position > mpos)
        {
            if (l == m)
                return m + 1;
            l = m;
        }
        else
            h = m;
    }
    return m;
}

/*
 * Clear note event list.
 */
void MIDIEditorCore::clear()
{
    noteEventList.clear();
}

/*
 * Returns an on event corresponding to given MIDI note.
 */
MIDINoteEvent* MIDIEditorCore::getOnEvent(QGraphicsRectItem* item)
{
    int position = item->boundingRect().x();
    int onoff = 1;
    int note = item->boundingRect().y() / yPerPitch;
    int velocity =255 - item->brush().color().red() * 2;
    MIDINoteEvent *event = new MIDINoteEvent(position, onoff, note, velocity);
    return event;
}

/*
 * Returns an off event corresponding to given MIDI note.
 */
MIDINoteEvent* MIDIEditorCore::getOffEvent(QGraphicsRectItem* item)
{
    int position = item->boundingRect().x() + item->boundingRect().width();
    int onoff = 0;
    int note = item->boundingRect().y() / yPerPitch;
    int velocity = 0;
    MIDINoteEvent *event = new MIDINoteEvent(position, onoff, note, velocity);
    return event;
}

