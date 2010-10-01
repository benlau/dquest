INCLUDEPATH += $$PWD

QT += sql

QMAKE_CXXFLAGS += -Wno-invalid-offsetof

TEMPLATE = lib
TARGET = dquest
CONFIG += staticlib
VERSION = 0.1

NORMAL_HEADERS += \
    dqclause.h \
    dqmodelmetainfo.h \
    dqmodel.h \
    dqconnection.h \
    dqabstractfield.h \
    dqsqlstatement.h \
    dqsqlitestatement.h \
    dqwhere.h \
    dqentity.h \
    dqsql.h \
    dqfield.h \
    dqabstractquery.h \
    dqquery.h \
    dqqueryrules.h

PRIV_HEADERS += \
    dqabstractquery_p.h

HEADERS += $$NORMAL_HEADERS \
    dqabstractmodel.h

SOURCES += \
    dqclause.cpp \
    dqmodelmetainfo.cpp \
    dqmodel.cpp \
    dqconnection.cpp \
    dqabstractfield.cpp \
    dqsqlstatement.cpp \
    dqsqlitestatement.cpp \
    dqwhere.cpp \
    dqsql.cpp \
    dqfield.cpp \
    dqabstractquery.cpp \
    dqqueryrules.cpp \
    dqabstractmodel.cpp

isEmpty(PREFIX) {
    PREFIX = /usr
}

LIBDIR=$$PREFIX/lib
INCDIR=$$PREFIX/include

INSTALLS += target headers
target.path = $$LIBDIR

headers.files = $$NORMAL_HEADERS
headers.path = $$INCDIR
