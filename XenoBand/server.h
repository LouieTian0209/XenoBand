
#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QDebug>

#include "thread.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    bool start(int port);

    Thread *thread;

signals:

public slots:

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    int port;
};

#endif // SERVER_H
