#ifndef TIMEINFO_H
#define TIMEINFO_H

#include <QDialog>

namespace Ui {
class TimeInfo;
}

class TimeInfo : public QDialog
{
    Q_OBJECT

public:
    explicit TimeInfo(QWidget *parent = 0);
    ~TimeInfo();
    TimeInfo *timeinfo;

private:
    Ui::TimeInfo *ui;
};

#endif // TIMEINFO_H
