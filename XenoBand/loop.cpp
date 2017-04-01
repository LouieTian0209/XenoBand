#include "core.h"
#include "loop.h"
#include "ui_loop.h"

Loop::Loop(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Loop)
{    
    ui->setupUi(this);

    transportCore = new TransportCore;
    transportWidget = new TransportWidget;

    transportWidget->setTransportCore(transportCore);

    ui->transportBox->layout()->addWidget(transportWidget);

    QSettings setting("XenoBand","sound mapping");
    setting.beginGroup("mappingName");
    foreach (const QString &name, setting.allKeys()) {
        ui->instCombo->addItem(name);
    }
    setting.endGroup();

    smCore = new SoundMappingCore;
    smCore->initPathMapping(ui->instCombo->currentText());

    connect(transportWidget, SIGNAL(notifyPlay()), this, SLOT(onPlay()));
    connect(transportWidget, SIGNAL(notifyPause()), this, SLOT(onStop()));
    connect(transportWidget, SIGNAL(notifyRewind()), this, SLOT(onStop()));

    connect(transportCore, SIGNAL(notifyTimeout()), this, SLOT(onTimeout()));

    playIndex = 0;

    initDone = true;
}

Loop::~Loop()
{
    delete ui;
}

void Loop::onTimeout()
{
    if (transportCore->getPosition() % 12 != 0)
        return;

    playPattern(ui->p0->text());
    playPattern(ui->p1->text());
    playPattern(ui->p2->text());
    playPattern(ui->p3->text());

    playIndex++;

    QString roots = ui->roots->text();
    if (roots.isEmpty())
        return;

    QStringList rootsl = roots.split(" ");

    if (playIndex % ui->p0->text().split(" ").length() == 0)
    {
        rootIndex++;
        rootIndex %= rootsl.length();

        root = rootsl[rootIndex].toInt();
    }
}

void Loop::playPattern(QString pattern)
{
    if (pattern.isEmpty())
        return;

    QStringList sl = pattern.split(" ");

    int size = sl.size();

    int note = sl[playIndex % size].toInt() + root;

    smCore->playSound(note);
}

void Loop::onPlay()
{
    if (ui->roots->text().isEmpty())
        return;

    root = ui->roots->text().split(" ")[0].toInt();
}

void Loop::onStop()
{
    playIndex = 0;
    rootIndex = 0;
}

void Loop::on_instCombo_currentIndexChanged(const QString &instname)
{
    if (initDone)
        smCore->initPathMapping(instname);
}

void Loop::on_tempBox_valueChanged(int value)
{
    transportCore->setTempo(value);
}

void Loop::closeEvent(QCloseEvent *)
{
    transportCore->stop();
}
