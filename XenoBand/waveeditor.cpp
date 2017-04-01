
#include "waveeditor.h"
#include "wavewritercore.h"
#include "ui_waveeditor.h"

WaveEditor::WaveEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaveEditor)
{
    ui->setupUi(this);
    waveWidget = new WaveWidget;
    waveReaderCore = new WaveReaderCore;
    height = waveWidget->size().height();
    // construct and configure a cursor onto wave widget
    cursor = new QGraphicsLineItem(0, 0, 0, 600);
    cursor->setFlag(QGraphicsItem::ItemIsMovable, false);
    cursor->setFlag(QGraphicsItem::ItemIsSelectable, false);
    waveWidget->getQGraphicsView()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    waveWidget->getQGraphicsView()->setScene(waveWidget->getQGraphicsScene());
    ui->waveLayout->addWidget(waveWidget);
    ui->waveLayout->setAlignment(Qt::AlignLeft);
    //waveWidget->getQGraphicsScene()->addItem(cursor);
    baseRatioX = 0.001;
    baseRatioY = 0.001;
    base = 1.1;

    transportWidget = new TransportWidget;
    timeWidget = new TimeWidget;
    transportCore = new TransportCore;

    // set up transport and time components onto corresponding widgets
    transportWidget->setTransportCore(transportCore);
    timeWidget->setTransportCore(transportCore);

    ui->transportBox->layout()->addWidget(transportWidget);
    ui->timeBox->layout()->addWidget(timeWidget);

    // [YZ] receive play button clicked signal
    connect(transportWidget, SIGNAL(notifyPlay()), this, SLOT(onPlay()));
}

WaveEditor::~WaveEditor()
{
    delete ui;
}

void WaveEditor::onPlay() {
    int numChannels = waveReaderCore->numChannels;
    int sampleRate = waveReaderCore->sampleRate;
    int bitsPerSample = waveReaderCore->bitsPerSample;

    // save file onto local file system
    ofstream ofs(Core::defaultWavePath().c_str(), ios::binary);
    WaveWriterCore::writeWave(ofs, buffer, numChannels, sampleRate, bitsPerSample);
    ofs.close();

    QSound::play(Core::defaultWavePath().c_str());
}

void WaveEditor::on_loadButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Find Wave Files"), QString::fromStdString(Core::defaultWavePath()), tr("MIDI Files (*.wav)"));

    if (!path.isEmpty()) {
        ui->lineEdit->setText(path);
        //clear();
        //drawCursor();
    }
    else
    {
        // do nothing
        return;
    }

    const char *str = path.toStdString().c_str();

    ifstream ifs(str, ios::binary);

    // read the selected wave file
    waveReaderCore->readWave(ifs);

    ifs.close();

    buffer.clear();

    clear();

    // draw all samples of the wave file onto wave widget
    for (int i = 0; i < (int) waveReaderCore->samples.size(); i++)
    {
        buffer.push_back(waveReaderCore->samples[i]);
    }

    //waveWidget->displayBuffer(buffer, baseRatioX * ui->xZoomBox->value(), baseRatioY * ui->yZoomBox->value());
    updateDisplay();
    waveWidget->getQGraphicsView()->setHorizontalScrollBar(new QScrollBar(this));
    waveWidget->getQGraphicsView()->horizontalScrollBar()->setValue(0);

    // [YZ] XXX disable cursor since it is not working
    //drawCursor();
}

// draw cursor onto graphics scene
void WaveEditor::drawCursor()
{
    height = waveWidget->size().height();
    cursor = new QGraphicsLineItem(0, 0, 0, 600);
    cursor->setFlag(QGraphicsItem::ItemIsMovable, false);
    cursor->setFlag(QGraphicsItem::ItemIsSelectable, false);
    //cursor->setAcceptDrops(true);
    //cursor->acceptDrops();
    cursor->setZValue(2);
    waveWidget->getQGraphicsScene()->addItem(cursor);
}

// clear the graphics scene
void WaveEditor::clear()
{
    waveWidget->getQGraphicsScene()->clear();
}

void WaveEditor::on_deleteButton_clicked()
{
    // determine start and end time for deletion
    int start = ui->startEdit->text().toInt();
    int end = ui->endEdit->text().toInt();

    // delete samples onto selected area
    for (int i = start; i < end; i++)
    {
        buffer[i] = 0;
    }
    //waveWidget->displayBuffer(buffer, baseRatioX * ui->xZoomBox->value(), baseRatioY * ui->yZoomBox->value());
    updateDisplay();
}

void WaveEditor::on_copyButton_clicked()
{
    // determine start and end time for copy
    int start = ui->startEdit->text().toInt();
    int end = ui->endEdit->text().toInt();

    // clear local copy buffer
    cbuffer.clear();

    // copy selected sample area onto local copy buffer
    for (int i = start; i < end; i++)
    {
        cbuffer.push_back(buffer[i]);
    }
}

void WaveEditor::on_cutButton_clicked()
{
    // determine start and end time for cut
    int start = ui->startEdit->text().toInt();
    int end = ui->endEdit->text().toInt();

    // clear local copy buffer
    cbuffer.clear();

    // copy selected sample area onto local copy buffer and delete the area
    for (int i = start; i < end; i++)
    {
        cbuffer.push_back(buffer[i]);
        buffer[i] = 0;
    }
    //waveWidget->displayBuffer(buffer, baseRatioX * ui->xZoomBox->value(), baseRatioY * ui->yZoomBox->value());
    updateDisplay();
}

void WaveEditor::on_pasteButton_clicked()
{
    // select start time for paste
    // Note: no need for end time since stuff in local copy buffer
    // would just be pasted from start time to start time + length of content in local copy buffer
    int start = ui->startEdit->text().toInt();
    //int end = ui->endEdit->text().toInt();

    // copy content from local copy buffer onto main buffer
    for (int i = start; i < (int) (start + cbuffer.size()); i++)
    {
        buffer[i] = cbuffer[i - start];
    }
    //waveWidget->displayBuffer(buffer, baseRatioX * ui->xZoomBox->value(), baseRatioY * ui->yZoomBox->value());
    updateDisplay();
}

// zooming in and out on horizontal direction
void WaveEditor::on_xZoomBox_valueChanged(int)
{
    //waveWidget->displayBuffer(buffer, baseRatioX * arg1, baseRatioY * ui->yZoomBox->value());
    updateDisplay();
}

// zooming in and out on vertical direction
void WaveEditor::on_yZoomBox_valueChanged(int)
{
    //waveWidget->displayBuffer(buffer, baseRatioX * ui->xZoomBox->value(), baseRatioY * arg1);
    updateDisplay();
}

// general function to handle display update
void WaveEditor::updateDisplay()
{
    waveWidget->displayBuffer(buffer, baseRatioX * 10 * pow(base, ui->xZoomBox->value() - 10), baseRatioY * 10 * pow(base, ui->yZoomBox->value() - 10));
}

// select area that is determined by start time and end time
void WaveEditor::on_selectButton_clicked()
{
    waveWidget->displayBuffer(buffer, baseRatioX * 10 * pow(base, ui->xZoomBox->value() - 10), baseRatioY * 10 * pow(base, ui->yZoomBox->value() - 10), ui->startEdit->text().toInt(), ui->endEdit->text().toInt());
}

void WaveEditor::on_saveButton_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save Wave File"), QString::fromStdString(Core::defaultWavePath()), tr("Wave File (*.wav)"));

    if (path.isEmpty())
        return;

    ui->saveEdit->setText(path);

    int numChannels = waveReaderCore->numChannels;
    int sampleRate = waveReaderCore->sampleRate;
    int bitsPerSample = waveReaderCore->bitsPerSample;

    // save file onto local file system
    ofstream ofs(path.toStdString().c_str(), ios::binary);
    WaveWriterCore::writeWave(ofs, buffer, numChannels, sampleRate, bitsPerSample);
    ofs.close();
}

void WaveEditor::on_shiftCutButton_clicked()
{
    int start = ui->startEdit->text().toInt();
    int end = ui->endEdit->text().toInt();

    // prevent index out of bound.
    if (end >= (int) (buffer.size()))
        end = (int) (buffer.size());

    cbuffer.clear();
    // copy selected area into copy buffer
    for (int i = start; i < end; i++)
    {
        cbuffer.push_back(buffer[i]);
    }

    // remove selected area and shift content afterwards to left
    // for length of copy buffer
    buffer.erase(buffer.begin() + start, buffer.begin() + end);

    updateDisplay();
}

void WaveEditor::on_shiftPasteButton_clicked()
{
    int start = ui->startEdit->text().toInt();
    //int end = ui->endEdit->text().toInt();

    //int oldSize = buffer.size();

    // expand buffer size
    //buffer.resize(buffer.size() + cbuffer.size());

    // insert content in local copy buffer at start time
    // and shift afterwards content to right for the length of local copy buffer
    buffer.insert(buffer.begin() + start, cbuffer.begin(), cbuffer.end());

    updateDisplay();
}

// get custom color from system color picker
// effect is immediate
void WaveEditor::on_colorButton_clicked()
{
    QColor waveColor = QColorDialog::getColor(Qt::white, this);

    waveWidget->setColor(waveColor);
}

QGraphicsScene *WaveEditor::getQGraphicsScene()
{
    return waveWidget->getQGraphicsScene();
}

void WaveEditor::on_fadeButton_clicked()
{
    int start = ui->startEdit->text().toInt();
    int end = ui->endEdit->text().toInt();

    for (int i = start; i < end; i++)
    {
        buffer[i] = buffer[i] * (i - start) / double(end - start);
    }
    updateDisplay();
}
