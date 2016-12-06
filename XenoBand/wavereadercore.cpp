#include "wavereadercore.h"
#include "binaryutil.h"

char buffer[50];

WaveReaderCore::WaveReaderCore()
{

}


void WaveReaderCore::readWave(ifstream &ifs)
{
    samples.clear();

    char buf[50];
    int n;

    // RIFF
    n = 4;
    ifs.read(buffer, n);
    emit sendRawWave(n, string(buffer), "RIFF");

    // chunk size
    ifs.read(buffer, n);
    sprintf(buf, "%d", BinaryUtil::toInt(n, buffer));
    emit sendRawWave(n, string(buffer), "chunk size: " + string(buf));

    // WAVE
    ifs.read(buffer, n);
    emit sendRawWave(n, string(buffer), "WAVE");

    // fmt
    ifs.read(buffer, n);
    emit sendRawWave(n, string(buffer), "fmt");

    // sub-chunk size
    ifs.read(buffer, n);
    sprintf(buf, "%d", BinaryUtil::toInt(n, buffer));
    emit sendRawWave(n, string(buffer), "sub-chunk size: " + string(buf));

    // audio format
    n = 2;
    ifs.read(buffer, n);
    emit sendRawWave(n, string(buffer), "audio format");

    // number of channels
    ifs.read(buffer, n);
    sprintf(buf, "%d", numChannels = BinaryUtil::toInt(n, buffer));
    emit sendRawWave(n, string(buffer), "number of channels: " + string(buf));

    // sample rate
    n = 4;
    ifs.read(buffer, n);
    sprintf(buf, "%d", sampleRate = BinaryUtil::toInt(n, buffer));
    emit sendRawWave(n, string(buffer), "sample rate: " + string(buf));

    // byte rate
    ifs.read(buffer, n);
    sprintf(buf, "%d", BinaryUtil::toInt(n, buffer));
    emit sendRawWave(n, string(buffer), "byte rate: " + string(buf));

    // block align
    n = 2;
    ifs.read(buffer, n);
    sprintf(buf, "%d", blockAlign = BinaryUtil::toInt(n, buffer));
    emit sendRawWave(n, string(buffer), "block align: " + string(buf));

    // bits per sample
    ifs.read(buffer, n);
    sprintf(buf, "%d", bitsPerSample = BinaryUtil::toInt(n, buffer));
    emit sendRawWave(n, string(buffer), "bits per sample: " + string(buf));

    // data
    n = 4;
    ifs.read(buffer, n);
    emit sendRawWave(n, string(buffer), "data");

    // sub-chunk size
    ifs.read(buffer, n);
    sprintf(buf, "%d", subChunk2Size = BinaryUtil::toInt(n, buffer));
    emit sendRawWave(n, string(buffer), "sub-chunk size: " + string(buf));

    int bytesPerSample = bitsPerSample / 8;

    for (int i = 0; !ifs.eof() && i < subChunk2Size / bytesPerSample; i++)
    {
        ifs.read(buffer, bytesPerSample);
        short s;
        sprintf(buf, "%d", s = BinaryUtil::toShort(bytesPerSample, buffer));
        samples.push_back(s);
        emit sendRawWave(bytesPerSample, string(buffer), "sample: " + string(buf));
    }
}
