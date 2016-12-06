#ifndef SOUNDMAPPINGCORE_H
#define SOUNDMAPPINGCORE_H

#include <QDebug>
#include <QMediaPlayer>
#include <QMap>
#include <QSettings>
#include <QStringList>

class SoundMappingCore
{
public:
    SoundMappingCore();
    ~SoundMappingCore();

    void playSound(int);
    void playSoundQMediaPlayer(int);
    void initPathMapping(const QString &mappingName);

    QString pathArr[128];

    static int numMappings();
    static QString getMapping(int);
    static QStringList getMappings();
};

#endif // SOUNDMAPPINGCORE_H

