#include "mainwindow.h"
#include "trackpreview.h"
#include "ui_trackpreview.h"

TrackPreview::TrackPreview(QWidget *parent, bool isWaveTrack) :
    QWidget(parent),
    ui(new Ui::TrackPreview)
{
    ui->setupUi(this);

    this->isWaveTrack = isWaveTrack;

    ui->typeLabel->setText(isWaveTrack ? "Wave" : "MIDI");

    editorMIDI = new MIDIEditor;
    editorWave = new WaveEditor;

    if (isWaveTrack)
    {
        ui->graphicsView->setScene(editorWave->getQGraphicsScene());
    }
    else
    {
        ui->graphicsView->setScene(editorMIDI->getQGraphicsScene());
    }
    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

TrackPreview::~TrackPreview()
{
    delete ui;

    delete editorMIDI;
    delete editorWave;
}

void TrackPreview::on_openButton_clicked()
{
    if (isWaveTrack)
        editorWave->show();
    else
        editorMIDI->show();
}

TrackPreview *TrackPreview::activeTrackPreview = NULL;

void TrackPreview::on_infoButton_clicked()
{
    activeTrackPreview = this;

    emit activatePreview();
}
