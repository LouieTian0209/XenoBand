#include "connectwindow.h"
#include "core.h"
#include "server.h"
#include "thread.h"
#include "ui_connectwindow.h"

static QString MIDI_EVENT = "MIDI ";

ConnectWindow::ConnectWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectWindow)
{
    ui->setupUi(this);

    ui->nickname->setText(QHostInfo::localHostName());

    client = new QTcpSocket;

    QString myIP;

    // get my pc address
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
            ui->myIPLayout->addWidget(new QLabel(address.toString()));
    }

    bandKeyboard = new Keyboard;

    connect(bandKeyboard, SIGNAL(sendMIDI(int)), this, SLOT(receiveMIDI(int)));

    assWidget = new QWidget;
}

ConnectWindow::~ConnectWindow()
{
    delete ui;

    delete bandKeyboard;
}

void ConnectWindow::closeEvent(QCloseEvent *)
{
    // send message to server
    QByteArray array;
    QDataStream out(&array,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);

    QString message = "[" + ui->nickname->text() + "] has left...";

    // send to server
    out << QString(message);
    out.device()->seek(0);
    if (client)
        client->write(array);
}

void ConnectWindow::on_sendMessage_clicked()
{
    // send message to server
    QByteArray array;
    QDataStream out(&array,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);

    QString message = "[" + ui->nickname->text() + "] " + ui->sendText->text();

    // send to server
    out << QString(message);
    out.device()->seek(0);
    client->write(array);

    // append to chat window
    ui->textBrowser->append(message);

    // [YZ] clear message after send
    ui->sendText->setText("");
}

void ConnectWindow::connected(){
    char message[50];
    sprintf(message, "Successfully connected to %s:%s",
            ui->connectIP->text().toStdString().c_str(), ui->connectPort->text().toStdString().c_str());
    ui->textBrowser->append(message);

    // [YZ] tell my name to server
    QByteArray array;
    QDataStream out(&array,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);

    QString firstMessage = "[" + ui->nickname->text() + "] has joined...";

    out << QString(firstMessage);
    out.device()->seek(0);
    client->write(array);
}

void ConnectWindow::on_connectToServer_clicked()
{
    // connect to server
    connect(client, SIGNAL(connected()), this, SLOT(connected()));
    connect(client, SIGNAL(disconnected()), this, SLOT(deleteLater()));

    client->connectToHost(QHostAddress(ui->connectIP->text()),ui->connectPort->text().toInt());

    client->waitForConnected();

    char messageFailed[50];
    sprintf(messageFailed, "Failed to connect %s:%s",
            ui->connectIP->text().toStdString().c_str(), ui->connectPort->text().toStdString().c_str());

    if (client->state() != QAbstractSocket::ConnectedState){
        delete client;
        ui->textBrowser->append(messageFailed);
        return;
    }
}

void ConnectWindow::on_startServer_clicked()
{
    // start server
    qDebug() << "server";
    server = new Server(this);
    bool success = server->start(ui->serverPort->text().toInt());

    if (success)
        ui->textBrowser->append("Server started...");
    else
        ui->textBrowser->append("Server could not start...");
}

// [YZ] receive MIDI note from keyboard, then send it out to server.
void ConnectWindow::receiveMIDI(int note)
{
    QByteArray array;
    QDataStream out(&array,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);

    QString message = "[" + ui->nickname->text() + "] " + MIDI_EVENT + QString::number(note);

    // send to server
    out << message;
    out.device()->seek(0);
    client->write(array);

    // append to GUI
    ui->textBrowser->append(message);

    // scroll to bottom
    QScrollBar *sb = ui->textBrowser->verticalScrollBar();
    sb->setValue(sb->maximum());
}

void ConnectWindow::receiveMessage(QString message)
{
    ui->textBrowser->append(message);

    if (message.startsWith('['))
    {
        QString nickname = message.split(" ")[0];

        if (!assMap.contains(nickname))
        {
            int size = SoundMappingCore::numMappings();

            QString instname;

            if (size == 0)
            {
                instname = "";
                return;
            }

            instname = SoundMappingCore::getMapping(assMap.size() % size);

            qDebug() << nickname << "assigned to play" << instname;

            SoundMappingCore *smCore = new SoundMappingCore;
            smCore->initPathMapping(instname);

            assMap.insert(nickname, instname);
            smCoreMap.insert(nickname, smCore);
        }

        // [YZ] check if message is a MIDI message
        if (message.contains(MIDI_EVENT))
        {
            QString note = message.split(" ")[message.split(" ").length() - 1];

            if (smCoreMap.contains(nickname))
                smCoreMap[nickname]->playSound(note.toInt());
        }
    }

    // scroll to bottom
    QScrollBar *sb = ui->textBrowser->verticalScrollBar();
    sb->setValue(sb->maximum());
}

void ConnectWindow::on_keyboardButton_clicked()
{
    bandKeyboard->show();
}

void ConnectWindow::on_assignButton_clicked()
{
    // [YZ] XXX
    assWidget = new QWidget;
    comboMap.clear();

    QFormLayout *formLayout = new QFormLayout;
    assWidget->setLayout(formLayout);

    formLayout->addRow(new QLabel("Nickname"), new QLabel("Instrument"));

    foreach (QString nickname , assMap.keys())
    {
        QString instname = assMap[nickname];

        QLabel *nicknameLabel = new QLabel(nickname);
        QComboBox *instnameComboBox = new QComboBox;

        comboMap.insert(nickname, instnameComboBox);

        QStringList list = SoundMappingCore::getMappings();

        foreach (QString inst , list)
        {
            instnameComboBox->addItem(inst);
        }

        instnameComboBox->setCurrentText(assMap[nickname]);

        connect(instnameComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(reassign(QString)));

        formLayout->addRow(nicknameLabel, instnameComboBox);
    }

    assWidget->show();
}

void ConnectWindow::reassign(const QString &s)
{
    qDebug() << "reassign" << s << "triggered";

    foreach (QString nickname , comboMap.keys())
    {
        QString instname = comboMap[nickname]->currentText();
        assMap[nickname] = instname;

        qDebug() << "reassign" << nickname << "to" << instname;

        smCoreMap[nickname] = new SoundMappingCore;
        smCoreMap[nickname]->initPathMapping(instname);
    }
}
