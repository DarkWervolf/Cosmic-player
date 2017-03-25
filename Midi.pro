#-------------------------------------------------
#
# Project created by QtCreator 2017-02-18T18:41:24
#
#-------------------------------------------------

QT       += core gui
QT += multimedia

QT += core gui multimedia
QT += multimediawidgets


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Midi
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    midiplayer.cpp

HEADERS  += mainwindow.h \
    midiplayer.h

FORMS    += mainwindow.ui

include(qmidi/QMidi.pri)

RESOURCES += \
    resourses.qrc

DISTFILES +=

