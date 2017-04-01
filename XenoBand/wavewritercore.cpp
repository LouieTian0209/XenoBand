#include "wavewritercore.h"

static double defaultAM(double)
{
    return 1;
}

static double defaultFM(double)
{
    return 1;
}

WaveWriterCore::WaveWriterCore(int sampleRate, int milliSeconds, double (*function)(double))
{
    this->sampleRate = sampleRate;
    this->milliSeconds = milliSeconds;
    this->function = function;

    this->functionAM = defaultAM;
    this->functionFM = defaultFM;

    ax = 0.0005;
    sx = 0.0015;
}

void WaveWriterCore::setFunction(double (*function)(double))
{
    this->function = function;
}

void WaveWriterCore::setFunctionAM(double (*functionAM)(double))
{
    this->functionAM = functionAM;
}

void WaveWriterCore::setFunctionFM(double (*functionFM)(double))
{
    this->functionFM = functionFM;
}

void WaveWriterCore::writeWave(ofstream &ofs, double freq)
{
    int numSamples = sampleRate * milliSeconds / 1000;
    int numBitsPerSample = 16;
    int numChannels = 1;

    writeString(ofs, "RIFF", 4);

    // chunk size = 4 + (8 + sub chunk size) + (8 + sub chunk size)
    writeInt(ofs, 36 + numSamples * numBitsPerSample / 8);

    writeString(ofs, "WAVE", 4);

    writeString(ofs, "fmt ", 4);
    // sub chunk size
    writeInt(ofs, 16);
    // audio format
    writeShort(ofs, 1);
    // number of channels
    writeShort(ofs, numChannels);
    // sample rate
    writeInt(ofs, sampleRate);
    // byte rate = sample rate * number of channels * bits per sample / 8
    writeInt(ofs, sampleRate * numChannels * numBitsPerSample / 8);
    // block align = number of channels * bits per sample / 8
    writeShort(ofs, numChannels * numBitsPerSample / 8);
    // bits per sample
    writeShort(ofs, numBitsPerSample);

    writeString(ofs, "data", 4);
    // sub chunk size
    writeInt(ofs, numSamples * numBitsPerSample / 8);

    // cerr << M_PI << endl;

    sup = 0x7000;

    for (int i = 0; i < numSamples; i++)
    {
        double value = function(i * 2 * M_PI * (freq * functionFM(i)) / (double)sampleRate) * (sup);

        if (functionAM)
        {
            value = functionAM(i) * value;
        }

        // Attack
        if (i < ax * numSamples)
        {
            value = value * i / (ax * numSamples);
        }
        // Release
        else if (i > sx * numSamples)
        {
            value = value * (double)(numSamples - i) / ((1-sx) * numSamples);
        }

        if (value > sup)
        {
            cerr << "WaveWrtierCore: exceed supreme" << endl;
            break;
        }

        writeShort(ofs, value);
    }

}

void WaveWriterCore::writeWave(ofstream &ofs, vector<int> &buffer, int numChannels, int sampleRate, int bitsPerSample)
{
    // [YZ] sample rate may change the pitch
    // int sampleRate = 44100;

    // unused
    // int milliSeconds = buffer.size() * 1000 / sampleRate;

    int numSamples = buffer.size();

    writeString(ofs, "RIFF", 4);

    // chunk size = 4 + (8 + sub chunk size) + (8 + sub chunk size)
    writeInt(ofs, 36 + numSamples * bitsPerSample / 8);

    writeString(ofs, "WAVE", 4);

    writeString(ofs, "fmt ", 4);
    // sub chunk size
    writeInt(ofs, 16);
    // audio format
    writeShort(ofs, 1);
    // number of channels
    writeShort(ofs, numChannels);
    // sample rate
    writeInt(ofs, sampleRate);
    // byte rate = sample rate * number of channels * bits per sample / 8
    writeInt(ofs, sampleRate * numChannels * bitsPerSample / 8);
    // block align = number of channels * bits per sample / 8
    writeShort(ofs, numChannels * bitsPerSample / 8);
    // bits per sample
    writeShort(ofs, bitsPerSample);

    writeString(ofs, "data", 4);
    // sub chunk size
    writeInt(ofs, numSamples * bitsPerSample / 8);

    // cerr << M_PI << endl;

    // int sup = 0x7000;

    for (int i = 0; i < (int) buffer.size(); i++)
    {
        writeShort(ofs, buffer[i]);
    }
}


void WaveWriterCore::writeString(ofstream &ofs, const char *str, const int size)
{
    ofs.write(str, size);
}

void WaveWriterCore::writeInt(ofstream &ofs, int n)
{
    char buffer[4];
    for (int i=0; i<4; i++)
        buffer[i] = (n >> i * 8);
    ofs.write(buffer, 4);
}

void WaveWriterCore::writeShort(ofstream &ofs, short n)
{
    char buffer[2];
    for (int i=0; i<2; i++)
        buffer[i] = (n >> i * 8);
    ofs.write(buffer, 2);
}
