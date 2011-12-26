# Verify the installed dquest

QT       += testlib

QT       -= gui

TARGET = tst_installtests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_installtests.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

include(/usr/include/dquest/dquest.pri)

HEADERS += \
    models.h
