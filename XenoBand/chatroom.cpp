#include "chatroom.h"
#include "ui_chatroom.h"

Chatroom::Chatroom(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chatroom)
{

    ui->setupUi(this);
    ui->PortNumber->setText("8001");
    ui->IPaddress->setText("pyrite.cs.iastate.edu");
    fileFormat = "foramtfile.txt";
    ui->disconnect->setEnabled(false);

    connect(ui->fileList,SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(saveFile()));

}

Chatroom::~Chatroom()
{
    delete ui;
}

void Chatroom::on_ConnectToServer_clicked()
{
    clientSocket = new QTcpSocket(this);

    connect(clientSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(clientSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    port = ui->PortNumber->text().toInt();
    ipaddress = ui->IPaddress->text();
    clientSocket->connectToHost(ipaddress, port);
    clientSocket->waitForConnected();
    if(clientSocket->state() != QAbstractSocket::ConnectedState){
        qDebug() << "Failed to connect.\n";
        delete clientSocket;
        return;
    }
}

void Chatroom::on_SendMessage_clicked()
{
    createFormatFile(QString::fromStdString("TEXT"));

}
void Chatroom::on_sendFile_clicked()
{
    createFormatFile(QString::fromStdString("FILE"));
}
void Chatroom::createFormatFile(QString type){
    QFile file(fileFormat);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "Cannot open" << file.fileName() << "\n";
        return;
    }
    QTextStream out(&file);
    out << "//BEGIN\n";
    out << "//ID\n";
    out << ui->userName->text() << "\n";
    out << "//TYPE\n";
    out << type<<"\n";
    if(type.compare("TEXT") == 0){
        out << "//DATA\n";
        out << ui->Message->text() << "\n";
        out << "//END";
    }
    else{
        QString filepath = ui->Message->text();
        out << "//FILENAME\n";
        out << filepath.section("/",-1,-1) << "\n";//filename
        out << "//DATA\n";
        QFile sendfile(filepath);
        if(!sendfile.open(QIODevice::ReadOnly)){
            qDebug()<<"Cannot read" << sendfile.fileName() << "\n";
            return;
        }
        QTextStream in(&sendfile);
        QString line = in.readLine();
        while(!line.isNull()){
            out << line << "\n";
            line = in.readLine();
        }
        out << "//END";
        sendfile.close();


    }
    file.close();

    sendData();

}

void Chatroom::sendData(){
    QFile file(fileFormat);
    QByteArray buffer;

    file.open(QIODevice::ReadOnly);

    while(1){
        buffer.clear();
        buffer = file.read(1000000);

        qDebug() << "Reading:" <<buffer.size();
        if(buffer.size() == 0){

            break;
        }
        clientSocket->write(buffer);

        clientSocket->flush();
        clientSocket->waitForBytesWritten();


        buffer.clear();
    }


    file.close();
    ui->Message->setText("");
}

void Chatroom::connected(){
    qDebug() << "Connecting...\n";
    ui->ConnectToServer->setEnabled(false);
    ui->disconnect->setEnabled(true);

}
void Chatroom::disconnected(){
    qDebug() << "Disconnecting...\n";

    ui->ConnectToServer->setEnabled(true);
    ui->disconnect->setEnabled(false);
}

void Chatroom::readyRead(){
    qDebug() << "Received message.\n";

    QFile file("temporary.txt");
    file.remove();
    file.open(QIODevice::ReadWrite);
    // QByteArray buffer=clientSocket->read(clientSocket->bytesAvailable());
    QByteArray buffer=clientSocket->readAll();
    QString string = buffer;

    qDebug() << "Read:" << buffer.size();
    file.write(buffer);
    qDebug() << "String:" << string;
    if(string.contains(QString::fromStdString("//END")) == true){
        file.close();
        display();
        //file.remove();

    }

}
void Chatroom::display(){
    QString name, type, filename, message;

    QFile file("temporary.txt");
    file.open(QIODevice::ReadOnly);

    QTextStream in(&file);
    QString line = in.readLine();
    while(!line.isNull()){
        line = in.readLine();
        qDebug() << line;
        if(line.compare(QString::fromStdString("//ID")) == 0){
            line = in.readLine();
            name = line;
            qDebug() << "Name:" << name;
        }
        else if(line.compare(QString::fromStdString("//TYPE")) == 0){
            line = in.readLine();
            type = line;
            qDebug() << "Type:" << type;
        }
        else if(line.compare(QString::fromStdString("//FIRENAME")) == 0){
            line = in.readLine();
            filename = line;
            qDebug() << "Filename:" << filename;
        }
        else if(line.compare(QString::fromStdString("//DATA")) == 0){
            if(type.compare(QString::fromStdString("TEXT")) == 0){
                line = in.readLine();
                message = line;
                qDebug() << "Message:" << message;
                break;
            }
            else{
                QByteArray array;
                while(true){
                    line = in.readLine();
                    if(line.compare(QString::fromStdString("//END")) == 0){
                        break;
                    }
                    array.append(line);
                }

                data.insert(filename, array);
            }

        }

    }
    if(type.compare(QString::fromStdString("TEXT")) == 0){
        ui->ChatMessage->append(name+": " + message);
    }
    else{
        ui->ChatMessage->append(name+": " + filename);
        ui->fileList->addItem(filename);
    }
    file.close();
    file.remove();

}
void Chatroom::saveFile(){
    QString filename=ui->fileList->currentItem()->text();
    // QStringList string=filename.split(".");
    // QString filetype=string.at(1);
    QString path=QFileDialog::getSaveFileName(this, tr("Save File"), "C:");
    qDebug() << path;
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    QByteArray array=data.value(filename);
    file.write(array);
    file.close();
}

void Chatroom::on_disconnect_clicked()
{
    qDebug() << "Disconnected.\n";
    clientSocket->close();
    ui->ConnectToServer->setEnabled(true);
    ui->disconnect->setEnabled(false);
}

void Chatroom::on_fileBrowse_clicked()
{
    // QString folderpath = QFileDialog::getExistingDirectory(this,tr("open file"),"C:",QFileDialog::DontResolveSymlinks);
    //qDebug()<<folderpath;
    QString filepath = QFileDialog::getOpenFileName(this, tr("Open File"), "C:");
    //QString directory=QFileDialog::getExistingDirectory(this,tr("Select a File"),QDir::currentPath());
    ui->Message->setText(filepath);
    // QFile file(filepath);
    qDebug() << filepath.section("/", -1, -1);//filename

}

