#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

#include "core.h"
#include "instantchat.h"
#include "networkutil.h"
#include "ui_instantchat.h"

InstantChat::InstantChat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InstantChat)
{
    ui->setupUi(this);

    ui->nameEdit->setText(QHostInfo::localHostName());

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(pull()));
    timer->start(1000);
}

InstantChat::~InstantChat()
{
    delete ui;
    delete timer;
}

QString prevdata;

void InstantChat::pull() {
    QString data = NetworkUtil::sendMessage("", "");

    if (prevdata != data) {
        ui->chatBrowser->clear();
        ui->chatBrowser->append(data);
        prevdata = data;
    }
}

void InstantChat::on_messageEdit_returnPressed()
{
    if (ui->nameEdit->text().isEmpty()) {
        // error dialog
        QMessageBox msgBox;
        msgBox.setText("Name must not be empty.");
        msgBox.exec();
        return;
    }

    QString name = ui->nameEdit->text();
    QString message = ui->messageEdit->text();
    ui->messageEdit->clear();

    // send message
    QString data = NetworkUtil::sendMessage(name, message);

    // append message
    ui->chatBrowser->clear();
    ui->chatBrowser->append(data);
}
