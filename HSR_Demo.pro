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
    ConfigParse/configparse.cpp \
    UI/page0.cpp \
    UI/page1.cpp \
    UI/page2.cpp \
    UI/page3.cpp \
    UI/page4.cpp \
    UI/page5.cpp \
    UI/page6.cpp \
    uimanager.cpp

HEADERS  += uibase.h \
    ConfigParse/configparse.h \
    UI/page1.h \
    UI/page0.h \
    UI/page2.h \
    UI/page3.h \
    UI/page4.h \
    UI/page5.h \
    UI/page6.h \
    uimanager.h
