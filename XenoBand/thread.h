#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QDataStream>
#include <QMessageBox>

class ConnectWindow;

class Thread : public QThread
{
    Q_OBJECT

public:
    explicit Thread(qintptr ID,QObject *parent = 0);
    void run(); 

    ConnectWindow *connectWindow;

signals:
    void notifyError(QTcpSocket::SocketError socketError);

    void sendMessage(QString message);

public slots:
    void readyRead();
    void disconnected();

private:
    QTcpSocket *socket;
    qintptr socketDescriptor;

public:
    static bool receiving;
    static QString text;
};

#endif // CLIENT_H
