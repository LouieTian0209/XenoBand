#include <iostream>

#include "midiwritercore.h"

#define BUFFER_SIZE (1<<8)

static char buffer[BUFFER_SIZE];

static void clearBuffer()
{
    for (int i = 0; i<BUFFER_SIZE; i++)
        buffer[i] = 0;
}

MIDIWriterCore::MIDIWriterCore()
{
}

void MIDIWriterCore::setNoteEventList(QList<MIDINoteEvent *> list)
{
    noteEventList.clear();

    for (int i = 0; i < list.size(); i++)
    {
        noteEventList << list[i];
    }
}

void MIDIWriterCore::writeMIDI(ofstream &ofs)
{
    // MThd
    MIDIWriterCore::writeHeader(ofs, 1, 2, 24); // [YZ] hack

    // MTrk
    MIDIWriterCore::writeMTrk(ofs, 27); // 4 + 4 + 4 (name) + 7 + 8

    // Track Name
    MIDIWriterCore::writeMetaTrackName(ofs, "Hack");

    // Tempo
    MIDIWriterCore::writeMetaTempo(ofs, 120);

    // Time Signature
    MIDIWriterCore::writeMetaTimeSignature(ofs, 4, 2, 24, 8);

    // End
    MIDIWriterCore::writeMetaTrackEnd(ofs);

    int length = 1000; // XXX [YZ] need to calculate

    // MTrk
    MIDIWriterCore::writeMTrk(ofs, length);

    // Track Name
    MIDIWriterCore::writeMetaTrackName(ofs, "Piano");

    // Program Change
    MIDIWriterCore::writeProgramChange(ofs, 0);

    // MIDI Note Events
    for (int i = 0; i<noteEventList.size(); i++)
    {
        MIDINoteEvent *event = noteEventList.at(i);
        int delta = 0;
        if (i > 0)
            delta = event->position - noteEventList.at(i-1)->position;
        int onoff = event->onoff;
        int note = event->note;
        int velocity = event->velocity;

        if (onoff) // if note on
            MIDIWriterCore::writeNoteOn(ofs, delta, note, velocity);
        else
            MIDIWriterCore::writeNoteOff(ofs, delta, note);
    }

    // End
    MIDIWriterCore::writeMetaTrackEnd(ofs);
}

void MIDIWriterCore::writeHeader(ofstream &ofs, int format, int numTracks, int division)
{
    // MThd
    clearBuffer();
    sprintf(buffer, "MThd");
    ofs.write(buffer, 4);

    // Length = 6
    clearBuffer();
    buffer[3] = 6;
    ofs.write(buffer, 4);

    // Format
    clearBuffer();
    buffer[1] = format;
    ofs.write(buffer, 2);

    // # Tracks
    clearBuffer();
    buffer[1] = numTracks;
    ofs.write(buffer, 2);

    // Division
    clearBuffer();
    buffer[0] = (division & 0xff00) >> 8;
    buffer[1] = (division & 0x00ff);
    ofs.write(buffer, 2);
}

void MIDIWriterCore::writeDelta(ofstream &ofs, int delta)
{
    clearBuffer();

    if (delta > 1<<7)
        cerr << "// TODO [YZ] writeDelta" << endl;

    buffer[0] = delta & 0x7f;
    ofs.write(buffer, 1);
}

void MIDIWriterCore::writeMTrk(ofstream &ofs, int trackLength)
{
    // MTrk
    clearBuffer();
    sprintf(buffer, "MTrk");
    ofs.write(buffer, 4);

    // Length
    clearBuffer();
    buffer[0] = (trackLength & 0xff000000) >> 24;
    buffer[1] = (trackLength & 0x00ff0000) >> 16;
    buffer[2] = (trackLength & 0x0000ff00) >> 8;
    buffer[3] = (trackLength & 0x000000ff);
    ofs.write(buffer, 4);
}

void MIDIWriterCore::writeMetaTrackName(ofstream &ofs, const char *name)
{
    writeDelta(ofs, 0); // delta = 0

    clearBuffer();
    buffer[0] = 0xff;
    buffer[1] = 0x03; // meaning track name

    int strlength = strlen(name);
    buffer[2] = strlength;

    ofs.write(buffer, 3);

    clearBuffer();
    sprintf(buffer, "%s", name);
    ofs.write(buffer, strlength);
}

void MIDIWriterCore::writeMetaTempo(ofstream &ofs, int tempo)
{
    writeDelta(ofs, 0);

    clearBuffer();
    buffer[0] = 0xff;
    buffer[1] = 0x51;
    buffer[2] = 0x03;
    ofs.write(buffer, 3); // meta info

    int microseconds_per_quater = 60 * 1000 * 1000 / tempo;
    buffer[0] = (microseconds_per_quater & 0xff0000) >> 16;
    buffer[1] = (microseconds_per_quater & 0xff00) >> 8;
    buffer[2] = (microseconds_per_quater & 0xff);
    ofs.write(buffer, 3);
}

void MIDIWriterCore::writeMetaTimeSignature(ofstream &ofs, int nn, int dd, int cc, int bb)
{
    writeDelta(ofs, 0);

    clearBuffer();
    buffer[0] = 0xff;
    buffer[1] = 0x58;
    buffer[2] = 0x04;
    ofs.write(buffer, 3); // meta info

    buffer[0] = nn;
    buffer[1] = dd;
    buffer[2] = cc;
    buffer[3] = bb;
    ofs.write(buffer, 4);
}

void MIDIWriterCore::writeMetaTrackEnd(ofstream &ofs)
{
    writeDelta(ofs, 0);

    clearBuffer();
    buffer[0] = 0xff;
    buffer[1] = 0x2f;
    buffer[2] = 0x00;
    ofs.write(buffer, 3);
}


void MIDIWriterCore::writeNoteOn(ofstream &ofs, int delta, int note, int velocity)
{
    writeDelta(ofs, delta);

    clearBuffer();
    buffer[0] = 0x90; // note on channel 1
    buffer[1] = note;
    buffer[2] = velocity;
    ofs.write(buffer, 3);
}

void MIDIWriterCore::writeNoteOff(ofstream &ofs, int delta, int note)
{
    writeDelta(ofs, delta);

    clearBuffer();
    buffer[0] = 0x80; // note off channel 1
    buffer[1] = note;
    buffer[2] = 0;
    ofs.write(buffer, 3);
}


void MIDIWriterCore::writeProgramChange(ofstream &ofs, int program)
{
    writeDelta(ofs, 0);

    clearBuffer();
    buffer[0] = 0xc0; // program change on channel 1
    buffer[1] = program;
    ofs.write(buffer, 2);
}
