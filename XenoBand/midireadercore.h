#ifndef MIDIREADERCORE_H
#define MIDIREADERCORE_H

#include "core.h"
#include "midinoteevent.h"

class MIDIReaderCore : public QObject
{
     Q_OBJECT

public:
    MIDIReaderCore();

    void readMIDI(ifstream&ifs);
    void copyBuffer(int n, char tar[]);

    int division;
    int timeSignatureN;
    int timeSignatureD;
    int tempo;

    QList<MIDINoteEvent *> noteEventList;

public slots:
signals:
    void sendRawMIDI(int n, string comment);

private:
    QList<MIDINoteEvent *> ons;

    MIDINoteEvent *getOnSibling(MIDINoteEvent *offSibling);

    void readnbytes(ifstream& ifs, int n, string comment);
    void readTrack(ifstream& ifs);
};

#endif // MIDIREADERCORE_H
