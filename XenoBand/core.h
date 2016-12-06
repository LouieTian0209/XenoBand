#ifndef CORE_H
#define CORE_H

// C
#include <math.h>
#include <string.h>

// C++
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

// Qt
#include <QComboBox>
#include <QColor>
#include <QColorDialog>
#include <QDebug>
#include <QFileDialog>
#include <QFormLayout>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHostInfo>
#include <QList>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QMessageBox>
#include <QScrollBar>
#include <QSettings>
#include <QStandardItemModel>
#include <QSound>
#include <QTimer>
#include <QUrl>
#include <QVector>
#include <QWidget>

using namespace std;

class Core
{
public:
    static string defaultWavePath();
    static string defaultDirPath();
};

#endif // CORE_H
