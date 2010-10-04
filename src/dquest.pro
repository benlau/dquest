INCLUDEPATH += $$PWD

QT += sql

QMAKE_CXXFLAGS += -Wno-invalid-offsetof

TEMPLATE = lib
TARGET = dquest
CONFIG += staticlib
VERSION = 0.1

include (dquest.pri)

isEmpty(PREFIX) {
    PREFIX = /usr
}

LIBDIR=$$PREFIX/lib/dquest
INCDIR=$$PREFIX/include/dquest

INSTALLS += target headers
target.path = $$LIBDIR

headers.files = $$DQUEST_HEADERS
headers.path = $$INCDIR
headers.extra = cp $$PWD/dquest-install.pri ${INSTALL_ROOT}$$PREFIX/include/dquest/dquest.pri

