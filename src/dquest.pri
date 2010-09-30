INCLUDEPATH += $$PWD

QT += sql

QMAKE_CXXFLAGS += -Wno-invalid-offsetof

HEADERS += \
    ../../src/dqclause.h \
    ../../src/dqmodelmetainfo.h \
    ../../src/dqmodel.h \
    ../../src/dqconnection.h \
    ../../src/dqabstractfield.h \
    ../../src/dqsqlstatement.h \
    ../../src/dqsqlitestatement.h \
    ../../src/dqwhere.h \
    ../../src/dqsql.h \
    ../../src/dqfield.h \
    ../../src/dqforeignkey.h \
    ../../src/dqabstractquery.h \
    ../../src/dqquery.h \
    ../../src/dqabstractquery_p.h \
    ../../src/dqqueryrules.h \
    ../../src/dqmetainfoquery_p.h \
    ../../src/dqexpression.h

SOURCES += \
    ../../src/dqclause.cpp \
    ../../src/dqmodelmetainfo.cpp \
    ../../src/dqmodel.cpp \
    ../../src/dqconnection.cpp \
    ../../src/dqabstractfield.cpp \
    ../../src/dqsqlstatement.cpp \
    ../../src/dqsqlitestatement.cpp \
    ../../src/dqwhere.cpp \
    ../../src/dqsql.cpp \
    ../../src/dqfield.cpp \
    ../../src/dqabstractquery.cpp \
    ../../src/dqqueryrules.cpp \
    ../../src/dqexpression.cpp
