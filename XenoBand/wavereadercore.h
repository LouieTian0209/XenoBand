#ifndef WAVEREADERCORE_H
#define WAVEREADERCORE_H

#include "core.h"

class WaveReaderCore : public QObject
{
    Q_OBJECT
public:
    WaveReaderCore();

    void readWave(ifstream &);

    vector<int> samples;

    int blockAlign;
    int numChannels;
    int sampleRate;
    int bitsPerSample;
    int subChunk2Size;

signals:
    void sendRawWave(int size, string data, string comment);
};

#endif // WAVEREADERCORE_H
