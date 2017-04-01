#include "core.h"
#include "keyboard.h"
#include "ui_keyboard.h"

#include <QPalette>

Keyboard::Keyboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Keyboard)
{
    ui->setupUi(this);

    // [YZ] init so that keyboard knows where the files are
    smCore = new SoundMappingCore;

    // keboard background color
    ui->pushButton_C->setStyleSheet("QPushButton{ background-color: white; }");
    ui->pushButton_D->setStyleSheet("QPushButton{ background-color: white; }");
    ui->pushButton_E->setStyleSheet("QPushButton{ background-color: white; }");
    ui->pushButton_F->setStyleSheet("QPushButton{ background-color: white; }");
    ui->pushButton_G->setStyleSheet("QPushButton{ background-color: white; }");
    ui->pushButton_A->setStyleSheet("QPushButton{ background-color: white; }");
    ui->pushButton_B->setStyleSheet("QPushButton{ background-color: white; }");
    ui->pushButton_C2->setStyleSheet("QPushButton{ background-color: white; }");
    ui->pushButton_D2->setStyleSheet("QPushButton{ background-color: white; }");
    ui->pushButton_E2->setStyleSheet("QPushButton{ background-color: white; }");
    ui->pushButton_F2->setStyleSheet("QPushButton{ background-color: white; }");

    QSettings setting("XenoBand","sound mapping");
    setting.beginGroup("midi#-key Mapping");
    ui->pushButton_C->setText(setting.value("60").toString());
    ui->pushButton_D->setText(setting.value("62").toString());
    ui->pushButton_E->setText(setting.value("64").toString());
    ui->pushButton_F->setText(setting.value("65").toString());
    ui->pushButton_G->setText(setting.value("67").toString());
    ui->pushButton_A->setText(setting.value("69").toString());
    ui->pushButton_B->setText(setting.value("71").toString());
    ui->pushButton_C2->setText(setting.value("72").toString());
    ui->pushButton_D2->setText(setting.value("74").toString());
    ui->pushButton_E2->setText(setting.value("76").toString());
    ui->pushButton_F2->setText(setting.value("77").toString());

    setting.endGroup();

    octave=ui->octave->text().toInt();

    setting.beginGroup("mappingName");
    foreach(const QString &name,setting.allKeys()){
        ui->comboBox->addItem(name);
    }
    setting.endGroup();

    if (ui->comboBox->children().size()==0)
    {
        qDebug() << "Keyboard: no instrument!";
        return;
    }

    smCore->initPathMapping(ui->comboBox->itemText(qrand() % ui->comboBox->children().size()));
}


Keyboard::~Keyboard()
{
    delete ui;

    delete smCore;
}

void Keyboard::keyPressEvent(QKeyEvent *keyevent){
    QString key=keyevent->text().toUpper();

    QSettings setting("XenoBand",ui->comboBox->itemText(0));
    setting.beginGroup("key-midi# Mapping");

    if (!setting.contains(key))
    {
        qDebug() << "Keyboard: no key mapping for" << key;
        return;
    }

    int note = setting.value(key).toInt();

    switch(note) {
        case 60:ui->pushButton_C->setStyleSheet("QPushButton{ background-color: gray; }");break;
        case 62:ui->pushButton_D->setStyleSheet("QPushButton{ background-color: gray; }");break;
        case 64:ui->pushButton_E->setStyleSheet("QPushButton{ background-color: gray; }");break;
        case 65:ui->pushButton_F->setStyleSheet("QPushButton{ background-color: gray; }");break;
        case 67:ui->pushButton_G->setStyleSheet("QPushButton{ background-color: gray; }");break;
        case 69:ui->pushButton_A->setStyleSheet("QPushButton{ background-color: gray; }");break;
        case 71:ui->pushButton_B->setStyleSheet("QPushButton{ background-color: gray; }");break;
        case 72:ui->pushButton_C2->setStyleSheet("QPushButton{ background-color: gray; }");break;
        case 74:ui->pushButton_D2->setStyleSheet("QPushButton{ background-color: gray; }");break;
        case 76:ui->pushButton_E2->setStyleSheet("QPushButton{ background-color: gray; }");break;
        case 77:ui->pushButton_F2->setStyleSheet("QPushButton{ background-color: gray; }");break;
    }

    // [YZ] disable timer, for some reason this caused keyboard to delay a lot
    // timer->start();

    playSound(note);
}

void Keyboard::keyReleaseEvent(QKeyEvent *keyevent){
    QString key=keyevent->text().toUpper();

    QSettings setting("XenoBand",ui->comboBox->itemText(0));
    setting.beginGroup("key-midi# Mapping");
    int MIDInum=setting.value(key).toInt();

    setting.endGroup();
    switch(MIDInum) {
    case 60:on_pushButton_C_released();break;
    case 62:on_pushButton_D_released();break;
    case 64:on_pushButton_E_released();break;
    case 65:on_pushButton_F_released();break;
    case 67:on_pushButton_G_released();break;
    case 69:on_pushButton_A_released();break;
    case 71:on_pushButton_B_released();break;
    case 72:on_pushButton_C2_released();break;
    case 74:on_pushButton_D2_released();break;
    case 76:on_pushButton_E2_released();break;
    case 77:on_pushButton_F2_released();break;
    }

}

void Keyboard::playSound(int note)
{
    smCore->playSound(note = note + 12 * octave);

    emit sendMIDI(note);
}

void Keyboard::on_pushButton_C_pressed()
{
    ui->pushButton_C->setStyleSheet("QPushButton{ background-color: gray; }");
    playSound(60);
}

void Keyboard::on_pushButton_D_pressed()
{
    ui->pushButton_D->setStyleSheet("QPushButton{ background-color: gray; }");
    playSound(62);
}

void Keyboard::on_pushButton_E_pressed()
{
    ui->pushButton_E->setStyleSheet("QPushButton{ background-color: gray; }");
    playSound(64);
}

void Keyboard::on_pushButton_F_pressed()
{
    ui->pushButton_F->setStyleSheet("QPushButton{ background-color: gray; }");
    playSound(65);
}

void Keyboard::on_pushButton_G_pressed()
{
    ui->pushButton_G->setStyleSheet("QPushButton{ background-color: gray; }");
    playSound(67);
}

void Keyboard::on_pushButton_A_pressed()
{
    ui->pushButton_A->setStyleSheet("QPushButton{ background-color: gray; }");
    playSound(69);
}

void Keyboard::on_pushButton_B_pressed()
{
    ui->pushButton_B->setStyleSheet("QPushButton{ background-color: gray; }");
    playSound(71);
}

void Keyboard::on_pushButton_C2_pressed()
{
    ui->pushButton_C2->setStyleSheet("QPushButton{ background-color: gray; }");
    playSound(72);
}

void Keyboard::on_pushButton_D2_pressed()
{
    ui->pushButton_D2->setStyleSheet("QPushButton{ background-color: gray; }");
    playSound(74);
}

void Keyboard::on_pushButton_E2_pressed()
{
    ui->pushButton_E2->setStyleSheet("QPushButton{ background-color: gray; }");
    playSound(76);
}

void Keyboard::on_pushButton_F2_pressed()
{
    ui->pushButton_F2->setStyleSheet("QPushButton{ background-color: gray; }");
    playSound(77);
}

void Keyboard::on_pushButton_C_released()
{
    ui->pushButton_C->setStyleSheet("QPushButton{ background-color: white; }");
}

void Keyboard::on_pushButton_D_released()
{
    ui->pushButton_D->setStyleSheet("QPushButton{ background-color: white; }");
}

void Keyboard::on_pushButton_E_released()
{
    ui->pushButton_E->setStyleSheet("QPushButton{ background-color: white; }");
}

void Keyboard::on_pushButton_F_released()
{
    ui->pushButton_F->setStyleSheet("QPushButton{ background-color: white; }");
}

void Keyboard::on_pushButton_G_released()
{
    ui->pushButton_G->setStyleSheet("QPushButton{ background-color: white; }");
}

void Keyboard::on_pushButton_A_released()
{
    ui->pushButton_A->setStyleSheet("QPushButton{ background-color: white; }");
}

void Keyboard::on_pushButton_B_released()
{
    ui->pushButton_B->setStyleSheet("QPushButton{ background-color: white; }");
}

void Keyboard::on_pushButton_C2_released()
{
    ui->pushButton_C2->setStyleSheet("QPushButton{ background-color: white; }");
}

void Keyboard::on_pushButton_D2_released()
{
    ui->pushButton_D2->setStyleSheet("QPushButton{ background-color: white; }");
}

void Keyboard::on_pushButton_E2_released()
{
    ui->pushButton_E2->setStyleSheet("QPushButton{ background-color: white; }");
}

void Keyboard::on_pushButton_F2_released()
{
    ui->pushButton_F2->setStyleSheet("QPushButton{ background-color: white; }");
}

void Keyboard::on_octave_valueChanged(int value)
{
    octave = value;
}

void Keyboard::on_comboBox_activated(const QString &name)
{
    smCore->initPathMapping(name);

    emit instChanged(name);
}

void Keyboard::setInstrument(QString instname)
{
    smCore->initPathMapping(instname);

    emit instChanged(instname);

    ui->comboBox->setCurrentText(instname);
}
