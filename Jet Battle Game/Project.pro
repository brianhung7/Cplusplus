#-------------------------------------------------
#
# Project created by QtCreator 2015-06-04T12:24:33
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Project
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gameboard.cpp

HEADERS  += mainwindow.h \
    gameboard.h

FORMS    += mainwindow.ui \
    gameboard.ui

RESOURCES += \
    images.qrc

CONFIG += c++11
