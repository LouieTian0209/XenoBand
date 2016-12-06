#ifndef PATHIMPORT_H
#define PATHIMPORT_H

#include <QWidget>
#include <QListWidgetItem>
namespace Ui {
class PathImport;
}

class PathImport : public QWidget
{
    Q_OBJECT

public:
    explicit PathImport(QWidget *parent = 0);
    ~PathImport();

private slots:
    void on_InstrumetList_itemDoubleClicked(QListWidgetItem *item);
    void showFilePath(QString instr);
    void on_pushButton_clicked();

private:
    Ui::PathImport *ui;
    bool piano;
};

#endif // PATHIMPORT_H
