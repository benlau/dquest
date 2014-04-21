#-------------------------------------------------
#
# Verify the mutliple thread ability of DQuest and
# the database engine
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_multithreadtests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_multithreadtests.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"


include(../../src/dquest.pri)
include(../models/models.pri)

HEADERS +=

