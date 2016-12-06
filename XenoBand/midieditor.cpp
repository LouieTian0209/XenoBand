#include "midieditor.h"
#include "ui_midieditor.h"

#include "midireadercore.h"
#include "midiwritercore.h"
#include "soundmappingcore.h"
#include "transportwidget.h"
#include "timewidget.h"

using namespace std;

MIDIEditor::MIDIEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MIDIEditor)
{
    ui->setupUi(this);

    // randomize color
    int colorR = rand() % 255;
    int colorG = rand() % 255;
    int colorB = rand() % 255;
    noteColor = QColor(colorR, colorG, colorB, 255);

    transportWidget = new TransportWidget;
    timeWidget = new TimeWidget;
    transportCore = new TransportCore;

    transportWidget->setTransportCore(transportCore);
    timeWidget->setTransportCore(transportCore);

    ui->transportBox->layout()->addWidget(transportWidget);
    ui->timeBox->layout()->addWidget(timeWidget);
    // mEvent = new QMouseEvent(QEvent::MouseButtonPress, QPointF(scene->width(), scene->height()), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    scene = new QGraphicsScene(this);
    scaleX = 1;
    scaleY = 1;
    // added graphics view onto ui and configure it
    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui->graphicsView->setScene(scene);
    // prevent the bug where first added item is not visible
    scene->addItem(new QGraphicsRectItem(0, 0, 0, 0));
    // construct and configure a cursor
    cursor = new QGraphicsLineItem(0, 0, 0, 1280);
    cursor->setFlag(QGraphicsItem::ItemIsMovable, false);
    cursor->setFlag(QGraphicsItem::ItemIsSelectable, false);
    cursor->setAcceptedMouseButtons(Qt::LeftButton);
    // setCursorWithMouse(mEvent);
    // cursor->setAcceptDrops(true);
    // cursor->acceptDrops();
    scene->addItem(cursor);

    playIndex = 0;

    readerCore = NULL;

    // connect transport core with transport widget on MIDI editor
    connect(transportCore, SIGNAL(notifyTimeout()), this, SLOT(on_timeout()));
    connect(transportCore, SIGNAL(notifyRecord()), this, SLOT(on_record()));

    midiTrackCore = new MIDIEditorCore;

    // vertical length of each note (rectangle)
    yPerPitch = ui->zoomYBox->value();
    xPerLength = ui->zoomXBox->value();

    smCore = new SoundMappingCore;

    keyboard = new Keyboard;

    connect(keyboard, SIGNAL(sendMIDI(int)), this, SLOT(on_MIDI_received(int)));

    connect(keyboard, SIGNAL(instChanged(QString)), this, SLOT(onInstChanged(QString)));

    // set instrument
    // instname = SoundMappingCore::getMapping(rand() % SoundMappingCore::numMappings());
    instname = "Piano";
    if (instname.isEmpty())
    {
        qDebug() << "MIDIEditor: instname is empty";
    }
    else
    {
        setWindowTitle("MIDI Editor - " + instname);

        keyboard->setInstrument(instname);
    }
}

MIDIEditor::~MIDIEditor()
{
    delete ui;

    delete midiTrackCore;

    delete smCore;
}

void MIDIEditor::setInstrument(QString instname)
{
    this->instname = instname;

    setWindowTitle("MIDI Editor - " + instname);

    keyboard->setInstrument(instname);
}

void MIDIEditor::on_timeout()
{
    int pos = transportCore->getPosition();

    if (pos == 0) {
        playIndex = 0;
    }

    if (playIndex >= midiTrackCore->noteEventList.size())
    {
        // [YZ] stop playing
        transportCore->timer->stop();
        return;
    }

    while (playIndex < midiTrackCore->noteEventList.size() &&
          pos >= midiTrackCore->noteEventList.at(playIndex)->position)
    {
        MIDINoteEvent* event = midiTrackCore->noteEventList[playIndex];
        if (event->onoff == 1)
        {
            smCore->playSound(event->note); // [YZ] QMediaPlayer to remove noise
        }
        playIndex++;
    }

    cursor->setPos(pos, 0);
}

void MIDIEditor::on_record()
{
    int pos = transportCore->getPosition();

    cursor->setPos(pos, 0);
}


void MIDIEditor::on_lengthBox_valueChanged(int value)
{
    ui->lengthSlider->setValue(value);
    if(scene->selectedItems().size() == 1) // respond only when one item is selected
        {
            // change the horizontal length of selected note
            QGraphicsRectItem *item = ((QGraphicsRectItem *)scene->selectedItems()[0]);
            item->setRect(item->boundingRect().x(), item->boundingRect().y(), value, yPerPitch);
        }
}

void MIDIEditor::on_lengthSlider_valueChanged(int value)
{
    ui->lengthBox->setValue(value);

}

void MIDIEditor::on_pitchBox_valueChanged(int value)
{
    ui->pitchSlider->setValue(value);
    if(scene->selectedItems().size() == 1) // respond only when one item is selected
        {
            // change the pitch (height) of each note
            QGraphicsRectItem *item = ((QGraphicsRectItem *)scene->selectedItems()[0]);
            item->setRect(item->boundingRect().x(), (127 - value) * yPerPitch, item->boundingRect().width(), yPerPitch);
        }

}

void MIDIEditor::on_pitchSlider_valueChanged(int value)
{
    ui->pitchBox->setValue(value);
}

void MIDIEditor::on_addNoteButton_clicked()
{
    // get needed information for adding a new note
    int pitch = ui->pitchBox->value();
    int length = ui->lengthBox->value();
    int velocity = ui->velocityBox->value();
    QString start = ui->timeEdit->text();
    bool isNumber;
    int stime = start.toInt(&isNumber);
    // if start time is empty or not a number or less than 0, set it to 0
    if(start.isEmpty() || !isNumber || stime < 0)
        stime = 0;
    // draw the note according to given information
    drawRect(stime, pitch, length, velocity);
    // each note contains an on event and an off event, so add them into
    // corresponding places
    MIDINoteEvent *onEvent = new MIDINoteEvent(stime, 1, pitch, velocity);
    MIDINoteEvent *offEvent = new MIDINoteEvent(stime + length, 0, pitch, 0);
    // event will be added and sorted automatically
    midiTrackCore->insertNoteEvent(onEvent);
    midiTrackCore->insertNoteEvent(offEvent);
}

void MIDIEditor::on_removeNoteButton_clicked()
{
    while(scene->selectedItems().size() != 0) //only selected notes can be deleted
    {
        // remove rectangles from graphics view as well as remove all relative
        // note events from note event lists
        QGraphicsRectItem *item = (QGraphicsRectItem*) scene->selectedItems()[0];
        scene->removeItem(scene->selectedItems()[0]);
        midiTrackCore->removeNoteEvent(midiTrackCore->getOnEvent(item));
        midiTrackCore->removeNoteEvent(midiTrackCore->getOffEvent(item));
    }
}

void MIDIEditor::on_velocitySlider_valueChanged(int value)
{
    ui->velocityBox->setValue(value);
    if(scene->selectedItems().size() >= 1) // color can be changed for multiple notes at the same time
        {
        for(int i = 0; i < scene->selectedItems().size(); i++)
            {
                // render the note with selected color
                QGraphicsRectItem *item = ((QGraphicsRectItem *)scene->selectedItems()[i]);
                item->setBrush(QBrush(QColor(noteColor.red(), noteColor.green(), noteColor.blue(), value * 2)));
            }
        }
}

void MIDIEditor::on_velocityBox_valueChanged(int value)
{
    ui->velocitySlider->setValue(value);
}

void MIDIEditor::on_loadButton_clicked()
{  
    // generate random numbers from 0 to 255 that will be used as
    // R, G and B component for the color
    int colorR = rand() % 255;
    int colorG = rand() % 255;
    int colorB = rand() % 255;
    noteColor = QColor(colorR, colorG, colorB, 255);

    QString path = QFileDialog::getOpenFileName(this, tr("Load MIDI File"), Core::defaultWavePath().c_str(), tr("MIDI File (*.mid)"));

    if (!path.isEmpty()) {
        // qDebug() << path;

        clear();
        drawCursor();
    }
    else
    {
        // do nothing
        return;
    }

    ifstream ifs(path.toStdString().c_str(), ios::binary);

    readerCore = new MIDIReaderCore;
    readerCore->readMIDI(ifs);

    // [YZ] udpate tempo
    transportCore->setTempo(readerCore->tempo);
    timeWidget->setTempo(QString::number(readerCore->tempo));

    for(int i = 0; i < readerCore->noteEventList.size(); i++)
    {
        midiTrackCore->insertNoteEvent(readerCore->noteEventList.at(i));
    }

    for(int i = 0; i < midiTrackCore->noteEventList.size(); i++)
    {
        if(midiTrackCore->noteEventList.at(i)->onoff == 1)
        {
            MIDINoteEvent* onEvent = midiTrackCore->noteEventList.at(i);
            MIDINoteEvent* offEvent = onEvent->sibling;

            // [YZ] skip if there is no off event
            if (!offEvent)
                continue;

            int position = onEvent->position;
            int velocity = onEvent->velocity;
            int pitch = onEvent->note;
            int length = offEvent->position - onEvent->position;
            drawRect(position, pitch, length, velocity);
        }
    }
    ui->graphicsView->setVerticalScrollBar(new QScrollBar(this));
    ui->graphicsView->verticalScrollBar()->setValue(60 * yPerPitch);
    ifs.close();

}

// draw note (rectangle) onto graphics scene
void MIDIEditor::drawRect(int pos, int pitch, int length, int velocity)
{
    QGraphicsRectItem *rItem = new QGraphicsRectItem(pos * xPerLength / 10, (127 - pitch) * yPerPitch, length * xPerLength / 10, yPerPitch);
    rItem->setPen(Qt::NoPen);
    //rItem->setBrush(QBrush(QColor(255 - velocity * 2, 255, 255 - velocity * 2, 255)));
    rItem->setBrush(QBrush(QColor(noteColor.red(), noteColor.green(), noteColor.blue(), velocity * 2)));
    rItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
    rItem->setFlag(QGraphicsItem::ItemIsMovable, false);
    // rItem->setAcceptDrops(true);
    // rItem->acceptDrops();
    rItem->setZValue(1);
    scene->addItem(rItem);
}

// function to draw cursor
void MIDIEditor::drawCursor()
{
    scene->addItem(new QGraphicsRectItem(0, 0, 0, 0));
    cursor = new QGraphicsLineItem(0, 0, 0, 1280);
    cursor->setFlag(QGraphicsItem::ItemIsMovable, false);
    cursor->setFlag(QGraphicsItem::ItemIsSelectable, false);
    //cursor->setAcceptDrops(true);
    //cursor->acceptDrops();
    cursor->setZValue(2);
    scene->addItem(cursor);
}

// function to clear the scene
void MIDIEditor::clear()
{
    scene->clear();
    midiTrackCore->clear();
}

void MIDIEditor::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() == Qt::LeftButton)
    {
        // Link rectangle info with sliders and spinboxes.
        // Not working right now.
        if(scene->selectedItems().size() == 1)
        {
            QGraphicsRectItem *item = ((QGraphicsRectItem *)scene->selectedItems()[0]);
            int stime = item->boundingRect().x();
            QString start = QString::number(stime);
            int pitch = item->boundingRect().y() / yPerPitch;
            int length = item->boundingRect().width();
            int velocity = 255 - item->brush().color().red() * 2;
            ui->timeEdit->setText(start);
            ui->pitchBox->setValue(pitch);
            ui->lengthSlider->setValue(length);
            ui->velocitySlider->setValue(velocity);
        }
    }
}

// save file onto local file system
void MIDIEditor::on_saveButton_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save MIDI File"), QDir::homePath(), tr("MIDI File (*.mid)"));

    if (path.isEmpty())
        return;

    ofstream ofs(path.toStdString().c_str(), ios::binary);
    MIDIWriterCore *writerCore = new MIDIWriterCore;
    writerCore->setNoteEventList(midiTrackCore->noteEventList);
    writerCore->writeMIDI(ofs);
    ofs.close();
}

void MIDIEditor::closeEvent(QCloseEvent *event)
{
    // [YZ] unnecessary
    /*
    QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("Application"), tr("Quit MIDI Track Editor?"), QMessageBox::Yes | QMessageBox::Cancel);
    if(ret == QMessageBox::Yes)
    {
        midiTrackCore->clear();
        // delete midiTrackCore;
        event->accept();
    }
    else
        event->ignore();
    */
    transportCore->stop();
    transportCore->setPosition(0);
}

// set note color
void MIDIEditor::setColor(QColor color)
{
    noteColor = color;
}

// get current note color
QColor MIDIEditor::getColor()
{
    return QColor(noteColor.red(), noteColor.green(), noteColor.blue(), 255);
}

// use system color picker to change note color
// this effect is immediate
void MIDIEditor::on_colorButton_clicked()
{
    noteColor = QColorDialog::getColor(Qt::white, this);

    QList<QGraphicsItem *> items = scene->items();

    for (int i = 0; i < items.size(); i++)
    {
        if (items[i]->type() == QGraphicsRectItem::Type)
        {
            QGraphicsRectItem *item = ((QGraphicsRectItem *) items[i]);
            item->setBrush(QBrush(QColor(noteColor.red(), noteColor.green(), noteColor.blue(), 255)));
        }
    }
}

void MIDIEditor::on_zoomXBox_valueChanged(int value)
{
    xPerLength = value;
    scene->clear();
    for(int i = 0; i < midiTrackCore->noteEventList.size(); i++)
    {
        if(midiTrackCore->noteEventList.at(i)->onoff == 1)
        {
            MIDINoteEvent* onEvent = midiTrackCore->noteEventList.at(i);
            MIDINoteEvent* offEvent = onEvent->sibling;

            // [YZ] skip if there is no off event
            if (!offEvent)
                continue;

            int position = onEvent->position;
            int velocity = onEvent->velocity;
            int pitch = onEvent->note;
            int length = offEvent->position - onEvent->position;
            drawRect(position, pitch, length, velocity);
        }
    }
    ui->graphicsView->setVerticalScrollBar(new QScrollBar(this));
    ui->graphicsView->verticalScrollBar()->setValue(60 * yPerPitch);
}

void MIDIEditor::on_zoomYBox_valueChanged(int value)
{
    yPerPitch = value;
    scene->clear();
    for(int i = 0; i < midiTrackCore->noteEventList.size(); i++)
    {
        if(midiTrackCore->noteEventList.at(i)->onoff == 1)
        {
            MIDINoteEvent* onEvent = midiTrackCore->noteEventList.at(i);
            MIDINoteEvent* offEvent = onEvent->sibling;

            // [YZ] skip if there is no off event
            if (!offEvent)
                continue;

            int position = onEvent->position;
            int velocity = onEvent->velocity;
            int pitch = onEvent->note;
            int length = offEvent->position - onEvent->position;
            drawRect(position, pitch, length, velocity);
        }
    }
    ui->graphicsView->setVerticalScrollBar(new QScrollBar(this));
    ui->graphicsView->verticalScrollBar()->setValue(60 * yPerPitch);

}

QGraphicsScene *MIDIEditor::getQGraphicsScene()
{
    return scene;
}

void MIDIEditor::on_keyboardButton_clicked()
{
    keyboard->show();
}

void MIDIEditor::onInstChanged(QString instname)
{
    this->instname = instname;
    this->smCore->initPathMapping(instname);

    this->setWindowTitle("MIDI Editor - " + instname);
}

void MIDIEditor::on_MIDI_received(int note)
{
    if (!transportCore->recording)
        return;

    // get needed information for adding a new note
    int pitch = note;
    int length = 10;
    int velocity = 100;
    int stime = transportCore->getPosition();
    // draw the note according to given information
    drawRect(stime, pitch, length, velocity);
    // each note contains an on event and an off event, so add them into
    // corresponding places
    MIDINoteEvent *onEvent = new MIDINoteEvent(stime, 1, pitch, velocity);
    MIDINoteEvent *offEvent = new MIDINoteEvent(stime + length, 0, pitch, 0);
    // event will be added and sorted automatically
    midiTrackCore->insertNoteEvent(onEvent);
    midiTrackCore->insertNoteEvent(offEvent);
}
