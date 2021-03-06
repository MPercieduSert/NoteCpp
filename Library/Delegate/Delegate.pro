#-------------------------------------------------
#
# Project created by QtCreator 2015-11-14T20:56:40
#
#-------------------------------------------------

QT += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Delegate
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++11
SOURCES += \
    ListeNoteDelegate.cpp \
    ListeElevesDelegate.cpp \
    MotCleDelegate.cpp

HEADERS += \
    ListeNoteDelegate.h \
    ListeElevesDelegate.h \
    MotCleDelegate.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
