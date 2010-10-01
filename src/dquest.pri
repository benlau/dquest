INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
QT += sql

QMAKE_CXXFLAGS += -Wno-invalid-offsetof

HEADERS += \
    $$PWD/dqclause.h \
    $$PWD/dqmodelmetainfo.h \
    $$PWD/dqmodel.h \
    $$PWD/dqconnection.h \
    $$PWD/dqabstractfield.h \
    $$PWD/dqsqlstatement.h \
    $$PWD/dqsqlitestatement.h \
    $$PWD/dqwhere.h \
    $$PWD/dqsql.h \
    $$PWD/dqfield.h \
    $$PWD/dqforeignkey.h \
    $$PWD/dqabstractquery.h \
    $$PWD/dqquery.h \
    $$PWD/dqabstractquery_p.h \
    $$PWD/dqqueryrules.h \
    $$PWD/dqmetainfoquery_p.h \
    $$PWD/dqexpression.h

SOURCES += \
    $$PWD/dqclause.cpp \
    $$PWD/dqmodelmetainfo.cpp \
    $$PWD/dqmodel.cpp \
    $$PWD/dqconnection.cpp \
    $$PWD/dqabstractfield.cpp \
    $$PWD/dqsqlstatement.cpp \
    $$PWD/dqsqlitestatement.cpp \
    $$PWD/dqwhere.cpp \
    $$PWD/dqsql.cpp \
    $$PWD/dqfield.cpp \
    $$PWD/dqabstractquery.cpp \
    $$PWD/dqqueryrules.cpp \
    $$PWD/dqexpression.cpp
