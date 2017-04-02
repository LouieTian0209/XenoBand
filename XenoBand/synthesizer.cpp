#include "networkutil.h"
#include "oscillator.h"
#include "scale.h"
#include "synthesizer.h"
#include "ui_synthesizer.h"
#include "wavewidget.h"
#include "wavewriter.h"
#include "wavewritercore.h"

vector<Oscillator *> oscillators;

static double resultFunction(double x)
{
    double result = 0;

    for (int i = 0; i < (int) oscillators.size(); i++)
    {
        result += oscillators[i]->oscillatorFunction(x);
    }

    return result;
}

Synthesizer::Synthesizer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Synthesizer)
{
    ui->setupUi(this);

    ui->resultAreaWidgetContents->layout()->addWidget(resultWaveWidget = new WaveWidget);

    NetworkUtil::sendUsage("", "opened sound synthesizer");
}

Synthesizer::~Synthesizer()
{
    oscillators.clear();

    delete ui;
}

void Synthesizer::closeEvent(QCloseEvent *)
{
    oscillators.clear();
}

void Synthesizer::updateResultFunctionDisplay()
{
    resultWaveWidget->displayFunction(resultFunction);
}

void Synthesizer::on_addOscillatorButton_clicked()
{
    Oscillator *oscillator = new Oscillator(this, this); // [YZ] XXX

    ui->oscillatorsLayout->addWidget(oscillator);

    oscillators.push_back(oscillator);

    updateResultFunctionDisplay();
}

void Synthesizer::on_playButton_clicked()
{
    WaveWriterCore *writerCore = new WaveWriterCore(44100, 2000, resultFunction); // 2000 milliseconds

    /*
    deltaAM = ui->deltaAM->value();
    deltaFM = ui->deltaFM->value();

    ratioAM = ui->ratioAM->value();
    ratioFM = ui->ratioFM->value();

    if (ui->applyAM->isChecked())
        writerCore->setFunctionAM(functionAM);
    if (ui->applyFM->isChecked())
        writerCore->setFunctionFM(functionFM);
    */

    ofstream ofs(Core::defaultWavePath().c_str(), ios::binary);
    writerCore->writeWave(ofs, 440);
    ofs.close();

    QSound::play(Core::defaultWavePath().c_str());

    delete writerCore;
}

/*
void Synthesizer::on_exportScaleButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Export Scale"),
                                                     QDir::currentPath(),
                                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (path.isEmpty())
        return;

    qDebug() << path;

    WaveWriterCore *writerCore = new WaveWriterCore(44100, 1000, resultFunction); // 1000 ms

    for (int note = 21; note <= 108; note++) // [YZ] 21 - 108 mapped to piano 1 to 88
    {
        int frequency = Scale::getFrequency(note);
        ofstream ofs((path + "/" + QString::number(note) + ".wav").toStdString().c_str(), ios::binary);
        writerCore->writeWave(ofs, frequency);
        ofs.close();
    }
}
*/

void Synthesizer::on_waveWriterButton_clicked()
{
    WaveWriter *waveWriter = new WaveWriter;
    waveWriter->setFunctionSynthesis(resultFunction);
    waveWriter->show();
}

void Synthesizer::on_clearButton_clicked()
{
    while (!oscillators.empty())
    {
        ui->oscillatorsLayout->removeWidget(oscillators[0]);
        delete oscillators[0];
        oscillators.erase(oscillators.begin());
    }

    oscillators.clear();

    updateResultFunctionDisplay();
}
