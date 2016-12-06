#ifndef SOUNDMAPPING_H
#define SOUNDMAPPING_H

#include <QWidget>
#include <QtCore>
#include <QDebug>
#include <QSettings>
#include <QMap>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include "soundmappingcore.h"
namespace Ui {
class SoundMapping;
}

class SoundMapping : public QWidget
{
    Q_OBJECT

public:
    explicit SoundMapping(QWidget *parent = 0);
    ~SoundMapping();

    void displayFormat();

    void saveMapping();
    void saveMapping(QString &);

    void groupImport(QString &);

    void setFilePath(QString,QString);

private slots:
    void loadMapping();

    void on_saveButton_clicked();

    void on_groupImportButton_clicked();

    void on_defaultKeyButton_clicked();

    void on_deleteMapping_clicked();

private:
    Ui::SoundMapping *ui;

    int numRows,numCols;

    QString currentMapping;

    QMap<int,QString> pathMapping;

    QString getNoteName(int note);
};

#endif // SOUNDMAPPING_H
