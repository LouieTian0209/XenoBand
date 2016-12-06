#include "connectwindow.h"
#include "thread.h"

QString Thread::text;
bool Thread::receiving;
Thread::Thread(qintptr ID, QObject *parent):
    QThread(parent)
{
    this->socketDescriptor = ID;
}

void Thread::run()
{
    // start thread
    socket = new QTcpSocket();
    if (!socket->setSocketDescriptor(this->socketDescriptor)){
        emit notifyError(socket->error());
        return;
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::DirectConnection);

    exec();
}

void Thread::readyRead(){
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_5);
    QString message;

    while (!in.atEnd())
    {
        in >> message;
        qDebug() << socketDescriptor << "-" << message;
        text = message;
        receiving = true;

        // connectWindow->receiveMessage(message);
        emit sendMessage(message);
    }
}

void Thread::disconnected(){
    qDebug() << socketDescriptor << "disconnected";
    // socket->deleteLater();
    socket->close();
}
