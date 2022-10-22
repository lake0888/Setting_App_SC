#-------------------------------------------------
#
# Project created by QtCreator 2017-01-09T15:18:05
#
#-------------------------------------------------

QT       += core network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql

TARGET = Setting_App_SC
TEMPLATE = app


SOURCES +=  main.cpp\
            server.cpp \
            daemon.cpp \
            socketthread.cpp

HEADERS +=  server.h \
            daemon.h \
            socketthread.h

include(QtService/qtservice.pri)
