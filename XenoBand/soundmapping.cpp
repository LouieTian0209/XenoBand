#include "core.h"
#include "soundmapping.h"
#include "soundmappingcore.h"
#include "ui_soundmapping.h"

SoundMapping::SoundMapping(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SoundMapping)
{
    ui->setupUi(this);
    numCols=4;

    QSettings setting("myCompany","sound mapping");
    setting.beginGroup("mappingName");

    foreach(const QString &name,setting.allKeys()){
        ui->mappingList->addItem(name);
    }

    setting.endGroup();

    connect(ui->mappingList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(loadMapping()));

    displayFormat();
}

SoundMapping::~SoundMapping()
{
    delete ui;
}

QString SoundMapping::getNoteName(int note)
{
    int octave = note / 12 - 1;
    int offset = note % 12;

    switch(offset)
    {
    case 0: return QString("C") + QString::number(octave); break;
    case 1: return QString("C#") + QString::number(octave) + QString(" / ") + QString("Db") + QString::number(octave);
    case 2: return QString("D") + QString::number(octave); break;
    case 3: return QString("D#") + QString::number(octave) + QString(" / ") + QString("Eb") + QString::number(octave);
    case 4: return QString("E") + QString::number(octave); break;
    case 5: return QString("F") + QString::number(octave); break;
    case 6: return QString("F#") + QString::number(octave) + QString(" / ") + QString("Gb") + QString::number(octave);
    case 7: return QString("G") + QString::number(octave); break;
    case 8: return QString("G#") + QString::number(octave) + QString(" / ") + QString("Ab") + QString::number(octave);
    case 9: return QString("A") + QString::number(octave); break;
    case 10: return QString("A#") + QString::number(octave) + QString(" / ") + QString("Bb") + QString::number(octave);
    case 11: return QString("B") + QString::number(octave); break;
    }
}

void SoundMapping::displayFormat(){
    ui->mappingTable->setColumnCount(numCols);
    ui->mappingTable->setHorizontalHeaderItem(0,new QTableWidgetItem("Note"));
    ui->mappingTable->setHorizontalHeaderItem(1,new QTableWidgetItem("MIDI#"));
    ui->mappingTable->setHorizontalHeaderItem(2,new QTableWidgetItem("Key"));
    ui->mappingTable->setHorizontalHeaderItem(3,new QTableWidgetItem("File Path"));

    numRows=88;
    ui->mappingTable->setRowCount(numRows);

    for (int i = 0; i<numRows; i++)
        ui->mappingTable->setItem(i,0,new QTableWidgetItem(getNoteName(i+21)));

    for (int i = 0; i<numRows; i++)
        ui->mappingTable->setItem(i,1,new QTableWidgetItem(QString::number(i+21)));

    ui->mappingTable->verticalHeader()->hide();
    ui->mappingTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->mappingTable->resizeColumnsToContents();
    ui->mappingTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
}

void SoundMapping::saveMapping() {
    QString filename = ui->mappingName->text();

    ui->mappingList->addItem(filename);

    saveMapping(filename);
}

void SoundMapping::saveMapping(QString &instrumentName) {
    int i;
    QString MIDInum, key, filepath;

    qDebug() << "save sound mapping " << instrumentName;

    QSettings set("myCompany","sound mapping");
    set.beginGroup("mappingName");
    set.setValue(instrumentName, instrumentName);
    set.endGroup();

    QSettings setting("myCompany", instrumentName);
    setting.clear();

    setting.beginGroup("midi#-key Mapping");
    for (i=0; i<numRows; i++) {
        if (!ui->mappingTable->item(i,2) || ui->mappingTable->item(i,2)->text().isEmpty())
            continue;

        MIDInum=ui->mappingTable->item(i,1)->text();
        key=ui->mappingTable->item(i,2)->text();
        setting.setValue(MIDInum,key);

        qDebug()<<"save "+MIDInum+"-"+key ;
    }
    setting.endGroup();

    setting.beginGroup("midi#-filepath Mapping");
    for (i=0; i<numRows; i++) {
        if (!ui->mappingTable->item(i,3) || ui->mappingTable->item(i,3)->text().isEmpty())
            continue;
        MIDInum=ui->mappingTable->item(i,1)->text();
        filepath=ui->mappingTable->item(i,3)->text();
        setting.setValue(MIDInum,filepath);
    }
    setting.endGroup();

    setting.beginGroup("key-midi# Mapping");
    for (i=0; i<numRows; i++) {
        if (!ui->mappingTable->item(i,2) || ui->mappingTable->item(i,2)->text().isEmpty())
            continue;

        key=ui->mappingTable->item(i,2)->text();
        MIDInum=ui->mappingTable->item(i,1)->text();
        setting.setValue(key,MIDInum);
    }
    setting.endGroup();
}

void SoundMapping::loadMapping() {
    // [YZ] hack to fill the cells
    on_defaultKeyButton_clicked();

    QString filename=ui->mappingList->currentItem()->text();

    QSettings setting("myCompany",filename);

    int i=0;
    QString filePath;

    int nrow;
    if(setting.childGroups().contains("midi#-key Mapping")){
        setting.beginGroup("midi#-key Mapping");
        foreach(const QString &num, setting.allKeys()){

            filePath = setting.value(num).toString();

            for(i=0;i<numRows;i++){
                if(num==ui->mappingTable->item(i,1)->text()){
                    nrow=i;
                    break;
                }
            }

            ui->mappingTable->setItem(nrow,2,new QTableWidgetItem(filePath));
            //qDebug() << "load"+note+"-"+key;
        }
        setting.endGroup();
    }

    if(setting.childGroups().contains("midi#-filepath Mapping")){
        setting.beginGroup("midi#-filepath Mapping");
        foreach(const QString &num, setting.allKeys()){

            filePath = setting.value(num).toString();

            for(i=0;i<numRows;i++){
                if(num==ui->mappingTable->item(i,1)->text()){
                    nrow=i;
                    break;
                }
            }

             ui->mappingTable->setItem(nrow,3,new QTableWidgetItem(filePath));
        }
        setting.endGroup();
    }
    else{
        for(i=0;i<numRows;i++){
            ui->mappingTable->setItem(i,3,new QTableWidgetItem(""));
        }
    }

}

void SoundMapping::setFilePath(QString filename, QString path){
    int i;
    for(i=0;i<numRows;i++){
        if(filename.contains(ui->mappingTable->item(i,1)->text())){
            ui->mappingTable->setItem(i,3,new QTableWidgetItem(path));
            break;
        }
    }
}

void SoundMapping::on_saveButton_clicked()
{
    //save
    saveMapping();
}

void SoundMapping::on_groupImportButton_clicked()
{
    QString folderpath = QFileDialog::getExistingDirectory(this, tr("Group Import"),
                                                           QString::fromStdString(Core::defaultDirPath()),
                                                           QFileDialog::DontResolveSymlinks);

    groupImport(folderpath);
}

void SoundMapping::groupImport(QString &folderpath) {
    // [YZ] hack to fill the cells
    on_defaultKeyButton_clicked();

    QStringList sl = folderpath.split(QDir::separator());

    ui->mappingName->setText(sl[sl.length()-1]);

    QDir dir = QDir(folderpath);
    QFileInfoList file=dir.entryInfoList();
    QListIterator<QFileInfo> iter(file);
    while (iter.hasNext()) {
        QFileInfo fi = iter.next();
        if (fi.isFile()) {
            setFilePath(fi.fileName(), fi.absoluteFilePath());
        }
    }
}

void SoundMapping::on_defaultKeyButton_clicked()
{
    int offset = 21;

    // lower part
    ui->mappingTable->setItem(48-offset,2,new QTableWidgetItem("Z"));
    ui->mappingTable->setItem(50-offset,2,new QTableWidgetItem("X"));
    ui->mappingTable->setItem(52-offset,2,new QTableWidgetItem("C"));
    ui->mappingTable->setItem(53-offset,2,new QTableWidgetItem("V"));
    ui->mappingTable->setItem(55-offset,2,new QTableWidgetItem("B"));
    ui->mappingTable->setItem(57-offset,2,new QTableWidgetItem("N"));
    ui->mappingTable->setItem(59-offset,2,new QTableWidgetItem("M"));

    // center part
    ui->mappingTable->setItem(60-offset,2,new QTableWidgetItem("A"));
    ui->mappingTable->setItem(61-offset,2,new QTableWidgetItem("W"));
    ui->mappingTable->setItem(62-offset,2,new QTableWidgetItem("S"));
    ui->mappingTable->setItem(63-offset,2,new QTableWidgetItem("E"));
    ui->mappingTable->setItem(64-offset,2,new QTableWidgetItem("D"));
    ui->mappingTable->setItem(65-offset,2,new QTableWidgetItem("F"));
    ui->mappingTable->setItem(66-offset,2,new QTableWidgetItem("T"));
    ui->mappingTable->setItem(67-offset,2,new QTableWidgetItem("G"));
    ui->mappingTable->setItem(68-offset,2,new QTableWidgetItem("Y"));
    ui->mappingTable->setItem(69-offset,2,new QTableWidgetItem("H"));
    ui->mappingTable->setItem(70-offset,2,new QTableWidgetItem("U"));
    ui->mappingTable->setItem(71-offset,2,new QTableWidgetItem("J"));
    ui->mappingTable->setItem(72-offset,2,new QTableWidgetItem("K"));
    ui->mappingTable->setItem(73-offset,2,new QTableWidgetItem("O"));
    ui->mappingTable->setItem(74-offset,2,new QTableWidgetItem("L"));
    ui->mappingTable->setItem(75-offset,2,new QTableWidgetItem("P"));
    ui->mappingTable->setItem(76-offset,2,new QTableWidgetItem(";"));
    ui->mappingTable->setItem(77-offset,2,new QTableWidgetItem("'"));

    // upper part
    // ui->mappingTable->setItem(72-offset,2,new QTableWidgetItem("1"));
    // ui->mappingTable->setItem(74-offset,2,new QTableWidgetItem("2"));
    // ui->mappingTable->setItem(76-offset,2,new QTableWidgetItem("3"));
    // ui->mappingTable->setItem(77-offset,2,new QTableWidgetItem("4"));
    ui->mappingTable->setItem(79-offset,2,new QTableWidgetItem("5"));
    ui->mappingTable->setItem(81-offset,2,new QTableWidgetItem("6"));
    ui->mappingTable->setItem(83-offset,2,new QTableWidgetItem("7"));
    ui->mappingTable->setItem(84-offset,2,new QTableWidgetItem("8"));
    ui->mappingTable->setItem(86-offset,2,new QTableWidgetItem("9"));
    ui->mappingTable->setItem(88-offset,2,new QTableWidgetItem("0"));
    ui->mappingTable->setItem(89-offset,2,new QTableWidgetItem("-"));
    ui->mappingTable->setItem(91-offset,2,new QTableWidgetItem("="));
}

//void SoundMapping::on_addMapping_clicked()
//{
//    QString filename=ui->mappingName->text();
//    ui->mappingList->addItem(filename);
//}

void SoundMapping::on_deleteMapping_clicked()
{
    QString name=ui->mappingList->currentItem()->text();
    ui->mappingList->takeItem(ui->mappingList->row(ui->mappingList->currentItem()));

    QSettings setting("myCompany","sound mapping");
    setting.beginGroup("mappingName");
    setting.remove(name);
    setting.endGroup();
}


