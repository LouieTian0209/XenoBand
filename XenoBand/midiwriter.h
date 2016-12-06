#ifndef MIDIWRITER_H
#define MIDIWRITER_H

#include "core.h"

using namespace std;

namespace Ui {
class MIDIWriter;
}

class MIDIWriter : public QWidget
{
    Q_OBJECT

public:
    explicit MIDIWriter(QWidget *parent = 0);
    ~MIDIWriter();

    void addNoteEvent(int delta, int onoff, int note, int velocity);

private slots:
    void on_addButton_clicked();

    void on_removeButton_clicked();

    void on_saveButton_clicked();

private:
    Ui::MIDIWriter *ui;

    QStandardItemModel *model;
};

#endif // MIDIWRITER_H
