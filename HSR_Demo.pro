#-------------------------------------------------
#
# Project created by QtCreator 2017-04-06T11:25:25
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HSR_Demo
TEMPLATE = app


SOURCES += main.cpp\
        uibase.cpp \
    ConfigParse/configparse.cpp

HEADERS  += uibase.h \
    ConfigParse/configparse.h
