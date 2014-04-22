# SQLITE test cases

QT       += testlib

QT       -= gui

TARGET = tst_sqliteteststest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_sqliteteststest.cpp \
    accesstests.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

include(../../src/dquest.pri)
include(../models/models.pri)

HEADERS += \
    accesstests.h

