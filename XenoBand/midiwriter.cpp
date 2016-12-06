#include "core.h"
#include "midiwriter.h"
#include "midiwritercore.h"
#include "ui_midiwriter.h"

MIDIWriter::MIDIWriter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MIDIWriter)
{
    ui->setupUi(this);

    ui->formatBox->addItem(QString("Single Track"));
    ui->formatBox->addItem(QString("Multiple"));
    ui->formatBox->addItem(QString("Song"));

    // [YZ] always multiple for compatibility with GarageBand
    ui->formatBox->setCurrentIndex(1);

    // one track for info, one track for instrument
    ui->numTracksBox->setValue(2);

    ui->divisionBox->setValue(24);
    ui->divisionBox->setRange(0, 960);

    ui->eventTable->setFont(QFont("Courier"));
    ui->eventTable->verticalHeader()->hide();

    ui->eventTable->setRowCount(0);
    ui->eventTable->setColumnCount(4);
    ui->eventTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->eventTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Delta"));
    ui->eventTable->setHorizontalHeaderItem(1, new QTableWidgetItem("On/Off"));
    ui->eventTable->setHorizontalHeaderItem(2, new QTableWidgetItem("Note"));
    ui->eventTable->setHorizontalHeaderItem(3, new QTableWidgetItem("Velocity"));

    addNoteEvent(0, 1, 60, 120);
    addNoteEvent(24, 0, 60, 0);

    // [YZ] examples, you can comment it out
//    QTableWidgetItem *item=new QTableWidgetItem ("Something");
//    ui->eventTable->setItem(0, 0, item);
//    QComboBox* combo = new QComboBox();
//    combo->addItem("MIDI Event");
//    combo->setFont(QFont("Arial"));
//    ui->eventTable->setCellWidget(0, 2, combo);
//    ui->eventTable->resizeColumnsToContents();
}

MIDIWriter::~MIDIWriter()
{
    delete ui;
}

void MIDIWriter::addNoteEvent(int delta, int onoff, int note, int velocity)
{
    int r = ui->eventTable->rowCount();
    ui->eventTable->insertRow(r); // insert empty row
    ui->eventTable->setItem(r, 0, new QTableWidgetItem(QString::number(delta)));
    ui->eventTable->setItem(r, 1, new QTableWidgetItem(QString::number(onoff)));
    ui->eventTable->setItem(r, 2, new QTableWidgetItem(QString::number(note)));
    ui->eventTable->setItem(r, 3, new QTableWidgetItem(QString::number(velocity)));

    char length[10];
    sprintf(length, "%d", (r+1) * 4);
    ui->MTrkLength->setText(length);
}

void MIDIWriter::on_addButton_clicked()
{
    addNoteEvent(0, 0, 0, 0);
}

void MIDIWriter::on_removeButton_clicked()
{
    QItemSelectionModel *select = ui->eventTable->selectionModel();

    // [YZ] remove all selected rows
    while (select->selectedRows().length())
        ui->eventTable->removeRow(select->selectedRows().last().row());

    // update length label
    char length[10];
    sprintf(length, "%d", ui->eventTable->rowCount() * 2);
    ui->MTrkLength->setText(length);
}

void MIDIWriter::on_saveButton_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save MIDI"), QDir::currentPath(), tr("MIDI Files (*.mid)"));

    if (path.isEmpty())
        return;

    // TODO
    ofstream ofs(path.toStdString().c_str(), ios::binary);

    // MThd
    MIDIWriterCore::writeHeader(ofs, ui->formatBox->currentIndex(), ui->numTracksBox->value(), ui->divisionBox->value());

    // MTrk
    MIDIWriterCore::writeMTrk(ofs, 27); // 4 + 4 + 4 (name) + 7 + 8

    // Track Name
    MIDIWriterCore::writeMetaTrackName(ofs, "Hack");

    // Tempo
    MIDIWriterCore::writeMetaTempo(ofs, 120);

    // Time Signature
    MIDIWriterCore::writeMetaTimeSignature(ofs, 4, 2, 24, 8);

    // End
    MIDIWriterCore::writeMetaTrackEnd(ofs);

    int length = ui->MTrkLength->text().toInt() + 1000; // XXX plus some extra

    // MTrk
    MIDIWriterCore::writeMTrk(ofs, length);

    // Track Name
    MIDIWriterCore::writeMetaTrackName(ofs, "Piano");

    // Program Change
    MIDIWriterCore::writeProgramChange(ofs, 0);

    // MIDI Note Events
    for (int i = 0; i<ui->eventTable->rowCount(); i++)
    {
        int delta = ui->eventTable->item(i, 0)->text().toInt();
        int onoff = ui->eventTable->item(i, 1)->text().toInt();
        int note = ui->eventTable->item(i, 2)->text().toInt();
        int velocity = ui->eventTable->item(i, 3)->text().toInt();

        if (onoff) // if note on
            MIDIWriterCore::writeNoteOn(ofs, delta, note, velocity);
        else
            MIDIWriterCore::writeNoteOff(ofs, delta, note);
    }

    // End
    MIDIWriterCore::writeMetaTrackEnd(ofs);

    ofs.close();
}

