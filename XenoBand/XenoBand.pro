#-------------------------------------------------
#
# Project created by QtCreator 2016-01-30T23:40:18
#
#-------------------------------------------------

QT       += core gui multimedia network websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XenoBand
TEMPLATE = app

ICON = XenoBand.icns

RC_FILE = icon.rc

SOURCES += main.cpp core.cpp mainwindow.cpp \
    audiorecorder.cpp \
    binaryutil.cpp \
    connectwindow.cpp \
    instantchat.cpp \
    keyboard.cpp \
    librarywindow.cpp \
    loop.cpp \
    midieditor.cpp \
    midieditorcore.cpp \
    midinoteevent.cpp \
    midireader.cpp \
    midireadercore.cpp \
    midiwriter.cpp \
    midiwritercore.cpp \
    networkutil.cpp \
    oscillator.cpp \
    pitchshifter.cpp \
    scale.cpp \
    server.cpp \
    soundmapping.cpp \
    soundmappingcore.cpp \
    synthesizer.cpp \
    thread.cpp \
    timewidget.cpp \
    trackpreview.cpp \
    transportcore.cpp \
    transportwidget.cpp \
    wavereader.cpp \
    wavewidget.cpp \
    wavewriter.cpp \
    wavewritercore.cpp \
    wavereadercore.cpp \
    waveeditor.cpp \
    chatroom.cpp \

HEADERS  += core.h mainwindow.h \
    audiorecorder.h \
    binaryutil.h \
    connectwindow.h \
    instantchat.h \
    keyboard.h \
    librarywindow.h \
    loop.h \
    midieditor.h \
    midieditorcore.h \
    midinoteevent.h \
    midireader.h \
    midireadercore.h \
    midiwriter.h \
    midiwritercore.h \
    networkutil.h \
    oscillator.h \
    pitchshifter.h \
    scale.h \
    server.h \
    soundmapping.h \
    soundmappingcore.h \
    synthesizer.h \
    thread.h \
    timewidget.h \
    trackpreview.h \
    transportcore.h \
    transportwidget.h \
    wavereader.h \
    wavewidget.h \
    wavewriter.h \
    wavewritercore.h \
    wavereadercore.h \
    waveeditor.h \
    chatroom.h \

FORMS    += mainwindow.ui \
    audiorecorder.ui \
    connectwindow.ui \
    instantchat.ui \
    keyboard.ui \
    librarywindow.ui \
    loop.ui \
    midieditor.ui \
    midireader.ui \
    midiwriter.ui \
    oscillator.ui \
    pitchshifter.ui \
    soundmapping.ui \
    synthesizer.ui \
    timewidget.ui \
    trackpreview.ui \
    transportwidget.ui \
    wavereader.ui \
    wavewidget.ui \
    wavewriter.ui \
    waveeditor.ui \
    chatroom.ui \

RESOURCES += \
    icon.qrc
