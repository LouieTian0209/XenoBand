#include "midireadercore.h"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1000
#endif

static char buffer[BUFFER_SIZE];

static int position = 0;

MIDIReaderCore::MIDIReaderCore()
{
}

void MIDIReaderCore::readMIDI(ifstream& ifs)
{
    readnbytes(ifs, 4, "MThd");
    readnbytes(ifs, 4, "Header Length = 6");

    readnbytes(ifs, 2, "Format");
    readnbytes(ifs, 2, "Number of Track Chunks");

    // read division
    ifs.read(buffer, 2);
    division = ((unsigned char) buffer[0] << 8) | (unsigned char) buffer[1];
    char info[50];
    sprintf(info, "Division = %d", division);
    emit sendRawMIDI(2, string(info));

    while (!ifs.eof())
        readTrack(ifs);
}

void MIDIReaderCore::copyBuffer(int n, char tar[])
{
    for (int i = 0; i<n; i++)
        tar[i] = buffer[i];
}

void MIDIReaderCore::readnbytes(ifstream& ifs, int n, string comment)
{
    if (ifs.eof())
        return;

    buffer[n] = '\0'; // to end the string
    ifs.read(buffer, n);

    emit sendRawMIDI(n, comment);
}

void MIDIReaderCore::readTrack(ifstream &ifs)
{
    char info[50];

    readnbytes(ifs, 4, "MTrk");

    // read track length
    ifs.read(buffer, 4);
    int trackLength = ((unsigned char) buffer[0] << 24)
            | ((unsigned char) buffer[1] << 16)
            | ((unsigned char) buffer[2] << 8)
            | ((unsigned char) buffer[3] << 0);
    sprintf(info, "Track Length = %d", trackLength);
    emit sendRawMIDI(4, string(info));

    /* Track Events */
    while (!ifs.eof())
    {
        int i = 0;
        unsigned char byte;

        // read and parse delta
        unsigned long delta = 0;
        for (i = 0;; i++)
        {
            ifs.read(buffer + i, 1);
            byte = buffer[i];
            delta = (delta << 7) | (byte & 0x7f);

            if (!(byte & 0x80))
            {
                break;
            }
        }

        int deltaLength = i + 1;
        sprintf(info, "Delta = %lu", delta);

        // [YZ] hack to ignore non-note delta
        if (noteEventList.size()==0)
            position = 12;

        position += delta * 24 / division; // [YZ] hack delta to reduce division to 24

        emit sendRawMIDI(deltaLength, string(info));

        // parse event type
        while (!ifs.eof())
        {
            ifs.read(buffer, 1);
            byte = buffer[0];

            if ((0x80 <= byte) && (byte <= 0xff))
                break;
            else
                emit sendRawMIDI(1, string("<undefined event type>"));
        }

        // note
        if ((0x80 <= byte) && (byte <= 0xaf))
        {
            bool noteOn = ((byte & 0xf0) == 0x90);

            ifs.read(buffer + 1, 2); // 2 more: note, velocity

            int note = (unsigned char) buffer[1];
            int velo = (unsigned char) buffer[2];

            if (noteOn)
            {
                MIDINoteEvent *onEvent = new MIDINoteEvent(position, 1, note, velo);

                noteEventList << onEvent;
                ons << onEvent;

                sprintf(info, "%03d %03d Note On ", note, velo);
            }
            else
            {
                MIDINoteEvent *offEvent = new MIDINoteEvent(position, 0, note, velo);

                noteEventList << offEvent;
                getOnSibling(offEvent); // [YZ] automatically linked

                sprintf(info, "%03d %03d Note Off", note, velo);
            }

            emit sendRawMIDI(3, string(info));
        }
        // control model change
        else if ((0xb0 <= byte) && (byte <= 0xbf))
        {
            ifs.read(buffer + 1, 2); // 2 more
            emit sendRawMIDI(3, string("Control Model Change"));
        }
        // program change
        else if ((0xc0 <= byte) && (byte <= 0xdf))
        {
            ifs.read(buffer + 1, 1); // 1 more: type
            emit sendRawMIDI(2, string("Program Change"));
        }
        // meta event
        else if (byte == 0xff)
        {
            ifs.read(buffer + 1, 2); // 2 more: meta type, length

            int eventLength = static_cast<int>(buffer[2]);

            char eventbuffer[eventLength + 1];
            eventbuffer[(int)eventLength] = '\0'; // to end the string
            ifs.read(eventbuffer, eventLength);

            // skip columns 1 and 2
            // ...

            unsigned char metatype = (unsigned char) buffer[1];

            if (metatype == 0x01)
            {
                emit sendRawMIDI(3, string("Meta: Text"));

                // comment
                emit sendRawMIDI(eventLength, string(eventbuffer));
            }
            else if (metatype == 0x03) // track name
            {
                emit sendRawMIDI(3, string("Meta: Track name"));

                // comment
                emit sendRawMIDI(eventLength, string(eventbuffer));
            }
            else if (metatype == 0x2f)
            {
                // [YZ] nothing should be added
                emit sendRawMIDI(3, string("Meta: Track End"));
                return;
            }
            else if (metatype == 0x51) // tempo
            {
                emit sendRawMIDI(3, string("Meta: Tempo"));

                // hex
                stringstream ss;
                char hexbuffer[10];
                for(int i = 0; i < eventLength; i++)
                {
                    sprintf(hexbuffer, "%02x", (unsigned char) eventbuffer[i]);
                    ss << hexbuffer;
                    if (i!=eventLength-1)
                        ss << " ";
                }

                int microseconds_per_quarter_note = 0;
                for (int i = 0; i < eventLength; i++)
                {
                    microseconds_per_quarter_note |= (((unsigned char)eventbuffer[i]) << (8 * (2 - i)));
                }

                tempo = 60000000 / microseconds_per_quarter_note;

                char info[50];
                sprintf(info, "%d", tempo);

                emit sendRawMIDI(eventLength, string(info));
            }
            else if (metatype == 0x58) // time signature
            {
                emit sendRawMIDI(3, string("Meta: Time Signature"));

                int nn, dd;
                nn = eventbuffer[0];
                dd = eventbuffer[1];
                dd = pow(2, dd);
                //cc = eventbuffer[2];
                //bb = eventbuffer[3];

                timeSignatureN = nn;
                timeSignatureD = dd;

                char info[50];
                sprintf(info, "%02d/%02d", nn, dd);

                emit sendRawMIDI(eventLength, string(info));
            }
            else
            {
                emit sendRawMIDI(3, string("<unknow>"));
            }
        } // end of meta
        else
        {
            emit sendRawMIDI(1, string("<unknown>"));
        }
    }
}

MIDINoteEvent *MIDIReaderCore::getOnSibling(MIDINoteEvent *offSibling)
{
    int note = offSibling->note;
    int pos = -1;

    for (int i = 0; i < ons.size(); i++)
    {
        if (ons.at(i)->note == note)
        {
            pos = i;
            break;
        }
    }
    if (pos == -1)
    {
        cerr << "MIDIReaderCore: sibling not found for " << *offSibling << endl;
        return NULL;
    }

    MIDINoteEvent *onSibling = ons.at(pos);

    // link siblings
    onSibling->sibling = offSibling;
    offSibling->sibling = onSibling;

    ons.removeAt(pos);
    return onSibling;
}
