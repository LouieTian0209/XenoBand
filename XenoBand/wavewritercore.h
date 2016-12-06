#ifndef WAVEWRITERCORE_H
#define WAVEWRITERCORE_H

#include "core.h"

class WaveWriterCore
{
public:
    WaveWriterCore(int sampleRate, int milliSeconds, double (*function) (double));

    void writeWave(ofstream &ofs, double freq);

    static void writeWave(ofstream &ofs, vector<int> &buffer, int numChannels, int sampleRate, int bitsPerSample);

    void setFunction(double (*function) (double));
    void setFunctionAM(double (*functionAM) (double));
    void setFunctionFM(double (*functionFM) (double));

    double ax; // attack
    // double dx; // decay
    double sx; // sustain
    // double rx; // release

private:
    int sampleRate;
    int milliSeconds;

    int sup; // supreme

    double (*function) (double);
    double (*functionAM) (double);
    double (*functionFM) (double);

    static void writeString(ofstream &ofs, const char *str, const int size);
    static void writeInt(ofstream &ofs, int);
    static void writeShort(ofstream &ofs, short);
};

#endif // WAVEWRITERCORE_H
