#ifndef INSTANTCHAT_H
#define INSTANTCHAT_H

#include <QWidget>

namespace Ui {
class InstantChat;
}

class InstantChat : public QWidget
{
    Q_OBJECT

public:
    explicit InstantChat(QWidget *parent = 0);
    ~InstantChat();

private slots:
    void on_messageEdit_returnPressed();

    // pull chat messages
    void pull();

private:
    Ui::InstantChat *ui;

    QTimer *timer;
};

#endif // INSTANTCHAT_H
