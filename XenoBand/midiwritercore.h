#ifndef MIDIWRITERCORE_H
#define MIDIWRITERCORE_H

#include "core.h"
#include "midinoteevent.h"

class MIDIWriterCore
{
public:
    MIDIWriterCore();

    QList<MIDINoteEvent *> noteEventList;

    void writeMIDI(ofstream&);
    void setNoteEventList(QList<MIDINoteEvent *> list);

    static void writeHeader(ofstream &ofs, int format, int numTracks, int division);
    static void writeMTrk(ofstream &ofs, int trackLength);
    static void writeDelta(ofstream &ofs, int delta);

    static void writeMetaTrackName(ofstream &ofs, const char *name);
    static void writeMetaTempo(ofstream &ofs, int tempo);
    static void writeMetaTimeSignature(ofstream &ofs, int nn, int dd, int cc, int bb); /* dd =: log 2 */
    static void writeMetaTrackEnd(ofstream &ofs);

    static void writeProgramChange(ofstream &ofs, int program);

    static void writeNoteOn(ofstream &ofs, int delta, int note, int velocity);
    static void writeNoteOff(ofstream &ofs, int delta, int note);
};

#endif // MIDIWRITERCORE_H
