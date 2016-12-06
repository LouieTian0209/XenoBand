#ifndef WAVEREADER_H
#define WAVEREADER_H

#include <QWidget>

#include "core.h"
#include "wavereadercore.h"

namespace Ui {
class WaveReader;
}

class WaveReader : public QWidget
{
    Q_OBJECT

public:
    explicit WaveReader(QWidget *parent = 0);
    ~WaveReader();

private slots:
    void on_loadWaveFile_clicked();
    void receiveRawWave(int, string data, string comment);

private:
    Ui::WaveReader *ui;

    QStandardItemModel *model;
    WaveReaderCore *readerCore;

    void resetTable();
    void updateRow(int row, int size, string data, string comment);

};

#endif // WAVEREADER_H
