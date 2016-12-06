#include "oscillator.h"
#include "synthesizer.h"
#include "ui_oscillator.h"
#include "wavewidget.h"

double ratiox = 1, amplitude = 1;

double staticOscillatorFunction(double x)
{
    return sin(x * ratiox) * amplitude;
}

double Oscillator::oscillatorFunction(double x)
{
    return sin(x * ui->ratioBox->value()) * ui->amplitudeBox->value();
}

Oscillator::Oscillator(QWidget *parent, Synthesizer *synthesizer) :
    QWidget(parent),
    synthesizer(synthesizer),
    ui(new Ui::Oscillator)
{
    ui->setupUi(this);

    ui->frame->layout()->addWidget(waveWidget = new WaveWidget);

    updateFunctionDisplay(ui->amplitudeBox->value(), ui->ratioBox->value());
}

Oscillator::~Oscillator()
{
    delete ui;
}

void Oscillator::updateFunctionDisplay(double ampVal, double ratioVal)
{
    ratiox = ratioVal;
    amplitude = ampVal;

    waveWidget->displayFunction(staticOscillatorFunction);

    synthesizer->updateResultFunctionDisplay();
}


void Oscillator::on_amplitudeBox_valueChanged(double value)
{
    updateFunctionDisplay(value, ui->ratioBox->value());
}

void Oscillator::on_ratioBox_valueChanged(double value)
{
    updateFunctionDisplay(ui->amplitudeBox->value(), value);
}
