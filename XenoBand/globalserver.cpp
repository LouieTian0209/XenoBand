#include "globalserver.h"
#include "networkutil.h"
#include "soundmappingcore.h"
#include "ui_globalserver.h"

GlobalServer::GlobalServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GlobalServer)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(pull()));
    timer->start(1000);
}

GlobalServer::~GlobalServer()
{
    qDebug() << "Global Server destructed";

    delete ui;
    delete timer;
}

void GlobalServer::closeEvent(QCloseEvent *)
{
    qDebug() << "Global Server closed";

    timer->stop();
    delete ui;
    delete timer;
}

void GlobalServer::pull() {
    QString data = NetworkUtil::sendMessage("", "");

    ui->textBrowser->clear();
    ui->textBrowser->append(data);

    // init demo for global server
    QString instname = SoundMappingCore::getMapping(0);
    SoundMappingCore *smCore = new SoundMappingCore;
    smCore->initPathMapping(instname);
    smCore->playSound(72);
}
