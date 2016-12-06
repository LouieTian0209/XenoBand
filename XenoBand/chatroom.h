#ifndef CHATROOM_H
#define CHATROOM_H

#include <QWidget>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QTextStream>
#include <QMap>

namespace Ui {
class Chatroom;
}

class Chatroom : public QWidget
{
    Q_OBJECT

public:
    explicit Chatroom(QWidget *parent = 0);
    ~Chatroom();
public slots:
    void connected();
    void disconnected();
    void readyRead();
    void sendData();
    void createFormatFile(QString);
    void display();
    void saveFile();
private slots:
    void on_ConnectToServer_clicked();

    void on_SendMessage_clicked();

    void on_disconnect_clicked();

    void on_fileBrowse_clicked();

    void on_sendFile_clicked();

private:
    Ui::Chatroom *ui;
    QTcpSocket *clientSocket;
    int port;
    QString ipaddress;
    QString fileFormat;
    QMap<QString, QByteArray> data;
};

#endif // CHATROOM_H
