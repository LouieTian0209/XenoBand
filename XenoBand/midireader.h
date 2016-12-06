#ifndef MIDIREADER_H
#define MIDIREADER_H

#include "core.h"
#include "midireadercore.h"

using namespace std;

namespace Ui {
class MIDIReader;
}

class MIDIReader : public QWidget
{
    Q_OBJECT

public:
    explicit MIDIReader(QWidget *parent = 0);
    ~MIDIReader();

public slots:
    void receiveRawMIDI(int, string comment);

private slots:
    void on_loadMIDIButton_clicked();

private:
    Ui::MIDIReader *ui;

    QStandardItemModel *model;

    int row, pos;
    MIDIReaderCore *readerCore;

    void readnbytes(ifstream&, int n, int row, string comment);
    void resetTable();
    void readTrack(ifstream &ifs);

    void updateRow(int n, int row, string comment);

    string toBinary(char);
};

#endif // MIDIREADER_H
