#-------------------------------------------------
#
# Project created by QtCreator 2017-04-06T11:25:25
#
#-------------------------------------------------

QT       += core gui xml multimedia network

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
<<<<<<< HEAD
    uimanager.cpp \
    UI/page7.cpp \
    UI/page8.cpp \
    GunneboGate/gunnebo.cpp \
    OmronTRE/omrontre.cpp \
    VOS_CSSa/player.cpp \
    EWReader/ewreader.cpp
=======
    uimanager.cpp
>>>>>>> 510673eb2a1318b344da31a5a36100bef37f4bb9

HEADERS  += uibase.h \
    ConfigParse/configparse.h \
    UI/page1.h \
    UI/page0.h \
    UI/page2.h \
    UI/page3.h \
    UI/page4.h \
    UI/page5.h \
    UI/page6.h \
<<<<<<< HEAD
    uimanager.h \
    publicdef.h \
    UI/page7.h \
    UI/page8.h \
    GunneboGate/gunnebo.h \
    OmronTRE/omrontre.h \
    OmronTRE/MAG_CssWeb.h \
    VOS_CSSa/VOS_CSSa.h \
    VOS_CSSa/player.h \
    EWReader/ewreader.h \
    EWReader/EWReaderCommon.h \
    GunneboGate/GAT_CssWeb.h \
    PSEMRSIV_GAT.h

#LIBS += $$PWD/GAT_CSSa.dll
LIBS += C:/AFC/agdriver/GAT_CSSa.dll  \
    C:/AFC/agdriver/MAG_CSSa.dll \
    C:/AFC/agdriver/VOS_CSSa.dll \
    C:/AFC/agdriver/EWReader.dll\
    C:/AFC/agdriver/PSEMRSIV_GAT.dll
=======
    uimanager.h
>>>>>>> 510673eb2a1318b344da31a5a36100bef37f4bb9
