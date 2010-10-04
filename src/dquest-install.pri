# pri file for installation target

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
QT += sql

QMAKE_CXXFLAGS += -Wno-invalid-offsetof

LIBS += -L$$PWD/../../lib/dquest -ldquest
