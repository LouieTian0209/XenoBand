#ifndef TEST_H
#define TEST_H

#include <QWidget>
#include <QTcpSocket>
#include <QDataStream>
#include <QHostAddress>
#include <QMessageBox>
#include <QtNetwork>
#include <QHostInfo>

#include "keyboard.h"
#include "server.h"
#include "thread.h"

namespace Ui {
class ConnectWindow;
}

class ConnectWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectWindow(QWidget *parent = 0);
    ~ConnectWindow();

    QMap<QString, QString> assMap;

    QMap<QString, SoundMappingCore *> smCoreMap;

public slots:
    void connected();

    void receiveMessage(QString message);

private slots:
    void on_startServer_clicked();

    void on_connectToServer_clicked();

    void on_sendMessage_clicked();

    void on_keyboardButton_clicked();

    void receiveMIDI(int note);

    void on_assignButton_clicked();

    void reassign(const QString &);

private:
    Ui::ConnectWindow *ui;
    QTcpSocket *client;
    Server *server;
    QString IPaddress;
    int port;
    Thread *thread;

    Keyboard *bandKeyboard;

    QWidget *assWidget;
    QMap<QString, QComboBox *> comboMap;

    void closeEvent(QCloseEvent *);
};

#endif // TEST_H
