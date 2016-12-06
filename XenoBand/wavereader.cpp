#include "binaryutil.h"
#include "core.h"
#include "wavereader.h"
#include "ui_wavereader.h"

int row = 0;

int bytePosition = 0;

WaveReader::WaveReader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaveReader)
{
    ui->setupUi(this);

    model = new QStandardItemModel(10, 3, this);

    resetTable();

    ui->tableView->setModel(model);
    ui->tableView->setFont(QFont("Courier"));
    ui->tableView->verticalHeader()->hide(); // [YZ] disable row count

    readerCore = new WaveReaderCore;

    connect(readerCore, SIGNAL(sendRawWave(int,string,string)), this, SLOT(receiveRawWave(int,string,string)));
}

WaveReader::~WaveReader()
{
    delete ui;

    delete model;
    delete readerCore;
}

void WaveReader::updateRow(int row, int size, string data, string comment)
{
    const char *buffer = data.c_str();

    // byte position
    model->setItem(row, 0, new QStandardItem(QString::number(bytePosition)));

    // binary
    string s;
    for (int i=0; i<size; i++)
    {
        s += BinaryUtil::toBinary(buffer[i]);
        if (i!=size-1)
            s += " ";
    }
    model->setItem(row, 1, new QStandardItem(QString::fromStdString(s)));

    // hex
    string hex;
    char hexbuffer[10];
    for(int i=0; i<size; i++)
    {
        sprintf(hexbuffer, "%02x", (unsigned char) buffer[i]);
        hex += hexbuffer;
        if (i!=size-1)
            hex += " ";
    }
    model->setItem(row, 2, new QStandardItem(QString::fromStdString(hex)));

    // comment
    model->setItem(row, 3, new QStandardItem(QString::fromStdString(comment)));

    bytePosition += size;
}

void WaveReader::receiveRawWave(int size, string data, string comment)
{
    updateRow(row++, size, data, comment);

    if (bytePosition <= 50 && bytePosition >= 25)
    {
        // [YZ] nice width
        ui->tableView->resizeColumnsToContents();
    }
}

void WaveReader::on_loadWaveFile_clicked()
{
    row = 0;
    bytePosition = 0;

    QString path = QFileDialog::getOpenFileName(this, tr("Find Wave Files"), QDir::currentPath(), tr("MIDI Files (*.wav)"));

    if (!path.isEmpty()) {
        ui->lineEdit->setText(path);
    }
    else
    {
        // do nothing
        return;
    }

    const char *str = path.toStdString().c_str();

    ifstream ifs(str, ios::binary);

    readerCore->readWave(ifs);

    ifs.close();
}

void WaveReader::resetTable()
{
    // clean previous load
    model->clear();
    row = 0;
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Byte")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Binary")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Hexadecimal")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("Comment")));
}
