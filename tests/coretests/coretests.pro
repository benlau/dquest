#-------------------------------------------------
#
# Project created by QtCreator 2010-09-14T18:26:15
#
#-------------------------------------------------

QT       += sql testlib

QT       -= gui

TARGET = tst_coreteststest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_coreteststest.cpp \
    dummy.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

include (../../src/dquest.pri)
include(../models/models.pri)
