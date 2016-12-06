#include "core.h"

string dirpath = "/.xenoband";

static void mkpath()
{
    QDir dir(QDir::homePath() + QString::fromStdString(dirpath));
    if (!dir.exists()) {
        dir.mkpath(".");
    }
}

string Core::defaultWavePath()
{
    mkpath();

    string path = QDir::homePath().toStdString() + dirpath + "/tmp.wav";

    return path;
}

string Core::defaultDirPath()
{
    mkpath();

    string path = QDir::homePath().toStdString() + dirpath;

    return path;
}
