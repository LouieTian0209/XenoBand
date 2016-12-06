#include "pitchshifter.h"
#include "scale.h"
#include "ui_pitchshifter.h"
#include "wavewritercore.h"

PitchShifter::PitchShifter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PitchShifter)
{
    ui->setupUi(this);

    waveReaderCore = new WaveReaderCore;

    waveBeforeWidget = new WaveWidget;
    ui->beforeLayout->addWidget(waveBeforeWidget);

    waveAfterWidget = new WaveWidget;
    ui->afterLayout->addWidget(waveAfterWidget);
}



PitchShifter::~PitchShifter()
{
    delete ui;

    delete waveReaderCore;
    delete waveBeforeWidget;
    delete waveAfterWidget;
}

vector<int> PitchShifter::pitchShift(vector<int> &input, double ratio)
{
    vector<int> output;

    int outputsize = int((double) input.size() / ratio);

    for (int i = 0; i<outputsize; i++)
    {
        int data = input[(int) i * ratio];
        output.push_back(data);
    }

    return output;
}

void PitchShifter::on_importButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Find Wave Files"), QString::fromStdString(Core::defaultWavePath()), tr("MIDI Files (*.wav)"));

    if (!path.isEmpty()) {
        ui->lineEdit->setText(path);
    }
    else
    {
        // do nothing
        return;
    }

    waveBeforeWidget->clear();
    waveAfterWidget->clear();
    buffer.clear();

    const char *str = path.toStdString().c_str();

    ifstream ifs(str, ios::binary);

    waveReaderCore->readWave(ifs);

    buffer.clear();
    for (unsigned long i = 0; i<waveReaderCore->samples.size(); i++)
        buffer.push_back(waveReaderCore->samples[i]);

    waveBeforeWidget->displayBuffer(buffer, 0.005, 0.005);

    ifs.close();
}

void PitchShifter::on_shiftButton_clicked()
{
    bufferShifted.clear();
    double ratio = ui->ratioBox->value();

    bufferShifted = pitchShift(buffer, ratio);

    waveAfterWidget->displayBuffer(bufferShifted, waveBeforeWidget->xratio, waveBeforeWidget->yratio);
}

void PitchShifter::on_playButton_clicked()
{
    ofstream ofs(Core::defaultWavePath().c_str(), ios::binary);
    WaveWriterCore::writeWave(ofs, bufferShifted, waveReaderCore->numChannels, waveReaderCore->sampleRate, waveReaderCore->bitsPerSample);
    ofs.close();

    QSound::play(Core::defaultWavePath().c_str());
}

void PitchShifter::on_scaleButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Export Scale"),
                                                     QDir::currentPath(),
                                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (path.isEmpty())
        return;

    qDebug() << path;

    double baseFreq = ui->baseFreqBox->value();

    for (int note = 21; note <= 108; note++) // [YZ] 21 - 108 mapped to piano 1 to 88
    {
        int frequency = Scale::getFrequency(note);
        ofstream ofs((path + "/" + QString::number(note) + ".wav").toStdString().c_str(), ios::binary);
        vector<int> shifted = pitchShift(buffer, frequency/baseFreq);

        WaveWriterCore::writeWave(ofs, shifted, waveReaderCore->numChannels, waveReaderCore->sampleRate, waveReaderCore->bitsPerSample);
        ofs.close();
    }
}

void PitchShifter::on_zoomInButton_clicked()
{
    waveBeforeWidget->zoom(2, 1);
    waveAfterWidget->zoom(2, 1);
}

void PitchShifter::on_zoomOutButton_clicked()
{
    waveBeforeWidget->zoom(0.5, 1);
    waveAfterWidget->zoom(0.5, 1);
}

void PitchShifter::on_spinBox_valueChanged(int value)
{
    ui->ratioBox->setValue(pow(2, value / 12.0));
}
