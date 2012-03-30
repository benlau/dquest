#-------------------------------------------------
#
# Multi-thread sqlite engine test
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_mtsqlitetest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_mtsqlitetest.cpp \
    dqproxybackendengine.cpp \
    dqmtsqlitebackendengine.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

include(../../src/dquest.pri)
include(../models/models.pri)

HEADERS += \
    dqproxybackendengine.h \
    dqmtsqlitebackendengine.h

