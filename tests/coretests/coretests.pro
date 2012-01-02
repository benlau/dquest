# Test dquest core function without the involvement of any database backend

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
