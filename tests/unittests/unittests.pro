#-------------------------------------------------
#
# Project created by QtCreator 2014-02-19T17:14:29
#
#-------------------------------------------------

QT       += core
QT       += testlib
QT       -= gui

TARGET = unittests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    testobjectrunner.cpp \
    sqlitetests.cpp \
    coretests.cpp \
    multithreadtests.cpp

HEADERS += \
    testobjectrunner.h \
    coretests.h \
    sqlitetests.h \
    multithreadtests.h

include (../../src/dquest.pri)
include(../testlib/testlib.pri)
