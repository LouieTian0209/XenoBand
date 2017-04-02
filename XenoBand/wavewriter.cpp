#include <QInputDialog>
#include <QSound>

#include "networkutil.h"
#include "scale.h"
#include "soundmapping.h"
#include "wavewriter.h"
#include "wavewritercore.h"
#include "ui_wavewriter.h"

static double functionDefault(double)
{
    return 1;
}

static double functionSawtooth(double x)
{
    x = x - 2 * M_PI * floor( x / (2 * M_PI) );
    double result = x < M_PI ? x / M_PI : x / M_PI - 2;
    return result / 2;
}

static double functionTriangle(double x)
{
    x = x - 2 * M_PI * floor( x / (2 * M_PI) );
    if (x < M_PI / 2)
        return x * 2 / M_PI;
    else if (x > 3 * M_PI / 2)
        return x * 2 / M_PI - 4;
    else
        return 2 - x * 2 / M_PI;
}

static double functionSquare(double x)
{
    return sin(x) > 0 ? 0.5 : -0.5;
}

// [YZ] can be passed from synthesizer
double (*functionSynthesis)(double);

static double functionSynthesisExample(double x)
{
    return (sin(x) + sin(2*x) + sin(4*x))/4;
}

static double deltaAM = 0;
static double deltaFM = 0;

static double ratioAM = 1000;
static double ratioFM = 1000;

static double functionAM(double x)
{
    double delta = deltaAM;
    return 1 + sin(x / ratioAM) * delta - delta;
}

static double functionFM(double x)
{
    double delta = deltaFM;
    return 1 + sin(x / ratioFM) * delta - delta;
}

WaveWriter::WaveWriter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaveWriter)
{
    ui->setupUi(this);

    // [YZ] init parameters
    // on_ratioAM_valueChanged(ratioAM);
    // on_ratioFM_valueChanged(ratioFM);

    fp = functionDefault;

    waveWidget = new WaveWidget;

    // update displayed wave when selected wave form has changed
    connect(ui->waveCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onWaveChanged(int)));

    // default
    waveWidget->displayFunction(functionSynthesis = functionSynthesisExample);

    ui->msBox->setValue(2000);

    this->layout()->addWidget(waveWidget);

    NetworkUtil::sendUsage("", "opened wave creator");
}

WaveWriter::~WaveWriter()
{
    delete ui;

    delete waveWidget;
}

void WaveWriter::setFunctionSynthesis(double (*fp)(double))
{
    this->fp = fp;

    functionSynthesis = fp;

    waveWidget->displayFunction(fp);
}

double (*WaveWriter::getFunctionByIndex(int)) (double)
{
    if (ui->waveCombo->currentIndex()==0)
        return functionSynthesis;
    else if (ui->waveCombo->currentIndex()==1)
        return sin;
    else if (ui->waveCombo->currentIndex()==2)
        return functionSquare;
    else if (ui->waveCombo->currentIndex()==3)
        return functionSawtooth;
    else if (ui->waveCombo->currentIndex()==4)
        return functionTriangle;

    return NULL; // XXX
}

void WaveWriter::onWaveChanged(int i)
{
    waveWidget->displayFunction(getFunctionByIndex(i));
}

void WaveWriter::on_playButton_clicked()
{
    fp = getFunctionByIndex(ui->waveCombo->currentIndex());

    waveWidget->displayFunction(fp);

    WaveWriterCore *writerCore = new WaveWriterCore(44100, ui->msBox->value(), fp); // 2000 milliseconds

    deltaAM = ui->deltaAM->value();
    deltaFM = ui->deltaFM->value();

    ratioAM = ui->ratioAM->value();
    ratioFM = ui->ratioFM->value();

    writerCore->ax = ui->attack->value() / 1000.0;
    writerCore->sx = ui->sustain->value() / 1000.0;

    if (ui->applyAM->isChecked())
        writerCore->setFunctionAM(functionAM);
    if (ui->applyFM->isChecked())
        writerCore->setFunctionFM(functionFM);

    ofstream ofs(Core::defaultWavePath().c_str(), ios::binary);
    writerCore->writeWave(ofs, 440);
    ofs.close();

    delete writerCore;

    QSound::play(Core::defaultWavePath().c_str());
}

void WaveWriter::on_ratioAMDial_sliderMoved(int position)
{
    ui->ratioAM->setValue(pow(10, 2 + position * 0.02));
}

void WaveWriter::on_ratioFMDial_sliderMoved(int position)
{
    ui->ratioFM->setValue(pow(10, 2 + position * 0.02));
}

void WaveWriter::on_deltaAMDial_sliderMoved(int position)
{
    ui->deltaAM->setValue(pow(10, 1 + position * 0.04) / 100000.0);
}

void WaveWriter::on_deltaFMDial_sliderMoved(int position)
{
    ui->deltaFM->setValue(pow(10, 1 + position * 0.04) / 100000.0);
}

void WaveWriter::on_ratioAM_valueChanged(double value)
{
    ui->ratioAMDial->setValue((log10(value) - 2) / 0.02);
}

void WaveWriter::on_ratioFM_valueChanged(double value)
{
    ui->ratioFMDial->setValue((log10(value) - 2) / 0.02);
}

void WaveWriter::on_deltaAM_valueChanged(double value)
{
    ui->deltaAMDial->setValue((log10(value * 100000.0) - 1) / 0.04);
}

void WaveWriter::on_deltaFM_valueChanged(double value)
{
    ui->deltaFMDial->setValue((log10(value * 100000.0) - 1) / 0.04);
}

void WaveWriter::saveInstrument(QString &path)
{
    if (path.isEmpty())
        return;

    qDebug() << "save to " + path;

    // make path if it does not exist
    QDir dir(path);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    fp = getFunctionByIndex(ui->waveCombo->currentIndex());

    waveWidget->displayFunction(fp);

    WaveWriterCore *writerCore = new WaveWriterCore(44100, ui->msBox->value(), fp);

    deltaAM = ui->deltaAM->value();
    deltaFM = ui->deltaFM->value();

    ratioAM = ui->ratioAM->value();
    ratioFM = ui->ratioFM->value();

    writerCore->ax = ui->attack->value() / 1000.0;
    writerCore->sx = ui->sustain->value() / 1000.0;

    if (ui->applyAM->isChecked())
        writerCore->setFunctionAM(functionAM);
    if (ui->applyFM->isChecked())
        writerCore->setFunctionFM(functionFM);

    for (int note = 21; note <= 108; note++) // [YZ] 21 - 108 mapped to piano 1 to 88
    {
        int frequency = Scale::getFrequency(note);
        ofstream ofs((path + "/" + QString::number(note) + ".wav").toStdString().c_str(), ios::binary);
        writerCore->writeWave(ofs, frequency);
        ofs.close();
    }
}

void WaveWriter::on_scaleButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Export Scale"),
                                                     QString::fromStdString(Core::defaultWavePath()),
                                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    saveInstrument(path);
}

void WaveWriter::on_attack_valueChanged(int value)
{
    if (ui->sustain->value() < value)
        ui->sustain->setValue(value);
}

void WaveWriter::on_exportButton_clicked()
{
    bool ok;
    QString instrumentName = QInputDialog::getText(this, tr("Export Instrument"),
                                         tr("Instrument Name"), QLineEdit::Normal,
                                         "My Instrument", &ok);
    if (ok && !instrumentName.isEmpty()) {
        QString path = QString::fromStdString(Core::defaultDirPath());
        path += QDir::separator() + instrumentName;

        saveInstrument(path);

        NetworkUtil::sendUsage("", "created an instrument named \"" + instrumentName + "\"");

        // XXX [YZ] hack to save key mapping
        // not a proper solution
        SoundMapping *sm = new SoundMapping;
        sm->groupImport(path);
        sm->saveMapping();
        delete sm;
    }
}
