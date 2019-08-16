#-------------------------------------------------
#
# Project created by QtCreator 2019-07-23T15:18:17
#
#-------------------------------------------------

QT       += core network quick
QT       -= gui


LIBS += $(QTDIR)/lib/libQt5SerialPort.so
INCLUDEPATH += $(QTDIR)/include/QtSerialPort

TARGET = ../Server
CONFIG   += console
CONFIG   += -serialport
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

HEADERS += \
    qport.h \
    qserver.h
