#include <QSound>

#include "core.h"
#include "soundmappingcore.h"

SoundMappingCore::SoundMappingCore()
{
}

SoundMappingCore::~SoundMappingCore()
{

}

// QString SoundMappingCore::pathArr[128];

void SoundMappingCore::playSound(int note){
    // play sound
    QString filepath = pathArr[note];
    qDebug() << "SoundMappingCore: play" << filepath;

    if (filepath.endsWith(".wav"))
    {
        QSound::play(filepath);
    }
    else if (filepath.endsWith(".mp3"))
    {
        QMediaPlayer *player = new QMediaPlayer;
        player->setMedia(QUrl::fromLocalFile(filepath));
        player->play();
    }
}

void SoundMappingCore::initPathMapping(const QString &mappingName) {
    QSettings setting("myCompany", mappingName);
    setting.beginGroup("midi#-filepath Mapping");

    foreach(const QString &num, setting.allKeys()){
        QString filePath = setting.value(num).toString();
        pathArr[num.toInt()] = filePath;
    }
    setting.endGroup();
}

int SoundMappingCore::numMappings()
{
    QSettings setting("myCompany","sound mapping");
    setting.beginGroup("mappingName");
    int size = setting.allKeys().size();
    setting.endGroup();

    return size;
}

QString SoundMappingCore::getMapping(int index)
{
    QSettings setting("myCompany","sound mapping");
    setting.beginGroup("mappingName");
    int size = setting.allKeys().size();
    if (size <= index)
        return "";
    QString ret = setting.allKeys()[index];
    setting.endGroup();

    return ret;
}

QStringList SoundMappingCore::getMappings()
{
    QSettings setting("myCompany","sound mapping");
    setting.beginGroup("mappingName");
    QStringList mappings = setting.allKeys();
    setting.endGroup();
    return mappings;
}
