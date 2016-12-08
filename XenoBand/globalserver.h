#ifndef GLOBALSERVER_H
#define GLOBALSERVER_H

#include <QWidget>

namespace Ui {
class GlobalServer;
}

class GlobalServer : public QWidget
{
    Q_OBJECT

public:
    explicit GlobalServer(QWidget *parent = 0);
    ~GlobalServer();

private:
    Ui::GlobalServer *ui;

    QTimer *timer;

private slots:
    void pull();
};

#endif // GLOBALSERVER_H
