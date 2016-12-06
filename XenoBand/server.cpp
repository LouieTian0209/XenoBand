
#include "connectwindow.h"
#include "server.h"
#include "thread.h"

Server::Server(QObject *parent):
    QTcpServer(parent)
{
}

bool Server::start(int port)
{
    this->port = port;
    qDebug() << "start server";
    if (!this->listen(QHostAddress::Any,port))
    {
        qDebug() << "Could not start server";
        return false;
    }
    else
    {
        qDebug() << "Listening...";
        return true;
    }
}

void Server::incomingConnection(qintptr socketDescriptor){
    thread = new Thread(socketDescriptor, this);

    // [YZ] hack for getting ConnectWindow
    // thread->connectWindow = (ConnectWindow *) parent();

    ConnectWindow *cw = (ConnectWindow *) parent();

    connect(thread, SIGNAL(sendMessage(QString)), cw, SLOT(receiveMessage(QString)));

    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}




