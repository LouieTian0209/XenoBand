#include "binaryutil.h"
#include "core.h"
#include "midireader.h"
#include "ui_midireader.h"

#define BUFFER_SIZE 10

using namespace std;

static char buffer[BUFFER_SIZE];

MIDIReader::MIDIReader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MIDIReader)
{
    ui->setupUi(this);

    model = new QStandardItemModel(10, 3, this);

    resetTable();

    ui->tableView->setModel(model);
    ui->tableView->setFont(QFont("Courier"));
    ui->tableView->verticalHeader()->hide(); // [YZ] disable row count
//    model->setItem(0, 0, new QStandardItem(QString("Hello")));
}

MIDIReader::~MIDIReader()
{
    delete ui;
}

// useful when calculating time signature
static int pow(int base, int pow)
{
    int result = 1;
    for (int i = 0; i < pow; i++)
    {
        result *= base;
    }
    return result;
}

void MIDIReader::on_loadMIDIButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Find MIDI Files"), QDir::currentPath(), tr("MIDI Files (*.mid)"));

    if (!path.isEmpty()) {
        ui->pathLineEdit->setText(path);
    }
    else
    {
        // do nothing
        return;
    }

    resetTable();

    // [YZ] convert to c string
    const char *str = path.toStdString().c_str();

    ifstream ifs(str, ios::binary);

    readerCore = new MIDIReaderCore;

    connect(readerCore, SIGNAL(sendRawMIDI(int,string)), this, SLOT(receiveRawMIDI(int,string)));

    readerCore->readMIDI(ifs);

    ifs.close();
}

void MIDIReader::receiveRawMIDI(int n, string comment)
{
    readerCore->copyBuffer(n, buffer);

    updateRow(n, row++, comment);

    if (row < 20) {
        ui->tableView->resizeColumnsToContents();
    }
}

void MIDIReader::updateRow(int n, int row, string comment)
{
    model->setItem(row, 0, new QStandardItem(QString::number(pos)));
    pos += n;

    stringstream ss;

    ss.str("");

    if (row>0 && model->item(row-1, 3)->text().startsWith("Meta")) // [YZ] hack
    {

    }
    else if (n <= 8)
    {
        // binary
        for(int i=0; i<n; i++)
        {
            ss << BinaryUtil::toBinary(buffer[i]);
            if (i!=n-1)
                ss << " ";
        }
        model->setItem(row, 1, new QStandardItem(QString(ss.str().c_str())));

        ss.str("");

        // hex
        char hexbuffer[10];
        for(int i=0; i<n; i++)
        {
            sprintf(hexbuffer, "%02x", (unsigned char) buffer[i]);
            ss << hexbuffer;
            if (i!=n-1)
                ss << " ";
        }
        model->setItem(row, 2, new QStandardItem(QString(ss.str().c_str())));
    }

    // comment
    model->setItem(row, 3, new QStandardItem(QString(comment.c_str())));
}

// [YZ] deprecated - don't call this method
void MIDIReader::readTrack(ifstream& ifs)
{
    char info[50];

    readnbytes(ifs, 4, row++, "MTrk");

    ifs.read(buffer, 4);
    int trackLength = ((unsigned char) buffer[0] << 24)
            | ((unsigned char) buffer[1] << 16)
            | ((unsigned char) buffer[2] << 8)
            | ((unsigned char) buffer[3] << 0);
    sprintf(info, "Track Length = %d", trackLength);
    updateRow(4, row++, info);

    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->resizeSection(3, QHeaderView::Stretch);

    /* Track Events */
    while (!ifs.eof())
    {
        int i = 0;
        unsigned char byte;

        // parse delta
        unsigned long delta = 0;
        for (i = 0;; i++)
        {
            ifs.read(buffer + i, 1);
            byte = buffer[i];
            delta = (delta << 7) | (byte & 0x7f);

            if (!(byte & 0x80))
            {
                break;
            }
        }

        int deltaLength = i + 1;
        sprintf(info, "Delta = %lu", delta);
        updateRow(deltaLength, row++, info);

        // parse event type
        while (!ifs.eof())
        {
            ifs.read(buffer, 1);
            byte = buffer[0];

            if (0x80 <= byte)
                break;
            else
                updateRow(1, row++, "<undefined event type>");
        }

        // note
        if ((0x80 <= byte) && (byte <= 0xaf))
        {
            bool noteOn = ((byte & 0xf0) == 0x90);

            ifs.read(buffer + 1, 2); // 2 more: note, velocity

            int note = (unsigned char) buffer[1];
            int velo = (unsigned char) buffer[2];

            if (noteOn)
                sprintf(info, "%03d %03d Note On ", note, velo);
            else
                sprintf(info, "%03d %03d Note Off", note, velo);

            updateRow(3, row++, info);
        }
        // control model change
        else if ((0xb0 <= byte) && (byte <= 0xbf))
        {
            ifs.read(buffer + 1, 2); // 2 more
            updateRow(3, row++, "Control Model Change");
        }
        // program change
        else if ((0xc0 <= byte) && (byte <= 0xdf))
        {
            ifs.read(buffer + 1, 1); // 1 more: type
            updateRow(2, row++, "Program Change");
        }
        // meta event
        else if (byte == 0xff)
        {
            ifs.read(buffer + 1, 2); // 2 more: meta type, length

            int eventLength = (int) buffer[2];

            char eventbuffer[eventLength + 1];
            eventbuffer[(int)eventLength] = '\0'; // to end the string
            ifs.read(eventbuffer, eventLength);

            // skip columns 1 and 2
            // ...

            unsigned char metatype = (unsigned char) buffer[1];

            if (metatype == 0x01)
            {
                updateRow(3, row++, "Meta: Text");

                // comment
                model->setItem(row, 3, new QStandardItem(eventbuffer));
            }
            else if (metatype == 0x03) // track name
            {
                updateRow(3, row++, "Meta: Track Name");

                // comment
                model->setItem(row, 3, new QStandardItem(eventbuffer));
            }
            else if (metatype == 0x2f)
            {
                // [YZ] nothing should be added
                updateRow(3, row++, "Meta: Track End");
                break;
            }
            else if (metatype == 0x51) // tempo
            {
                updateRow(3, row++, "Meta: Tempo");

                // hex
                stringstream ss;
                char hexbuffer[10];
                for(int i = 0; i < eventLength; i++)
                {
                    sprintf(hexbuffer, "%02x", (unsigned char) eventbuffer[i]);
                    ss << hexbuffer;
                    if (i!=eventLength-1)
                        ss << " ";
                }
                model->setItem(row, 2, new QStandardItem(QString(ss.str().c_str())));

                int microseconds_per_quarter_note = 0;
                for (int i = 0; i < eventLength; i++)
                {
                    microseconds_per_quarter_note |= (((unsigned char)eventbuffer[i]) << (8 * (2 - i)));
                }

                int tempo = 60000000 / microseconds_per_quarter_note;

                char info[50];
                sprintf(info, "%d", tempo);
                // comment
                model->setItem(row, 3, new QStandardItem(info));
            }
            else if (metatype == 0x58) // time signature
            {
                updateRow(3, row++, "Meta: Time Signature");

                int nn, dd;
                nn = eventbuffer[0];
                dd = eventbuffer[1];
                dd = pow(2, dd);
                //cc = eventbuffer[2];
                //bb = eventbuffer[3];

                char info[50];
                sprintf(info, "%02d/%02d", nn, dd);
                // comment
                model->setItem(row, 3, new QStandardItem(info));
            }
            else
            {
                updateRow(3, row++, "Meta: <unknown>");
            }

            // event content
            model->setItem(row, 0, new QStandardItem(QString::number(pos)));
            pos += eventLength;

            if (eventLength > 0)
                row++; // [YZ] increment row for meta event only when event has content
        } // end of meta
        else
        {
            updateRow(1, row++, "<unknow>");
        }
    }
}

// [YZ] deprecated - don't call this method
void MIDIReader::readnbytes(ifstream& ifs, int n, int row, string comment)
{
    if (ifs.eof())
        return;

    buffer[n] = '\0'; // to end the string
    ifs.read(buffer, n);

    model->setItem(row, 0, new QStandardItem(QString::number(pos)));

    updateRow(n, row, comment);
}

string MIDIReader::toBinary(char c)
{
    return BinaryUtil::toBinary(c);
}

void MIDIReader::resetTable()
{
    // clean previous load
    model->clear();
    row = 0;
    pos = 0;
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Byte")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Binary")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Hexadecimal")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("Comment")));
}
