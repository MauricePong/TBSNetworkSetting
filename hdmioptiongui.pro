#-------------------------------------------------
#
# Project created by QtCreator 2017-11-21T18:38:15
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = TBSNetConfigurationTool
TEMPLATE    = app
MOC_DIR     = temp/moc
RCC_DIR     = temp/rcc
UI_DIR      = temp/ui
OBJECTS_DIR = temp/obj
DESTDIR     = $$PWD/../bin

SOURCES     += main.cpp \
    tbsfunc.cpp \
    threadtbs.cpp \
    Descriptor.c \
    GetPidPackage.c \
    GetSection.c \
    ParseEIT.c \
    ParseNIT.c \
    ParsePAT.c \
    ParsePMT.c \
    ParseSDT.c \
    ParseTS_Length.c \
    Program.c \
	MainTest.c
SOURCES     += iconhelper.cpp
SOURCES     += appinit.cpp
SOURCES     += hdmioptiongui.cpp

HEADERS     += iconhelper.h \
    tbsfunc.h \
    threadtbs.h \
    Descriptor.h \
    GetPidPackage.h \
    GetSection.h \
    ParseEIT.h \
    ParseNIT.h \
    ParsePAT.h \
    ParsePMT.h \
    ParseSDT.h \
    ParseTS_Length.h \
    Program.h\
	MainTest.h
HEADERS     += appinit.h
HEADERS     += hdmioptiongui.h

FORMS       += hdmioptiongui.ui

RESOURCES   += main.qrc
RESOURCES   += qss.qrc
CONFIG      += qt warn_off
INCLUDEPATH += $$PWD
#LIBS        +=-lpthread -ldl $$PWD/libpci.so
RC_FILE = hdmioptiongui.rc
#DEFINES += QT_NO_WARNING_OUTPUT\
 #                QT_NO_DEBUG_OUTPUT


