INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
QT += sql

QMAKE_CXXFLAGS += -Wno-invalid-offsetof

DQUEST_HEADERS += \
    $$PWD/dqclause.h \
    $$PWD/dqmodelmetainfo.h \
    $$PWD/dqmodel.h \
    $$PWD/dqconnection.h \
    $$PWD/dqbasefield.h \
    $$PWD/dqsqlstatement.h \
    $$PWD/dqsqlitestatement.h \
    $$PWD/dqwhere.h \
    $$PWD/dqsql.h \
    $$PWD/dqfield.h \
    $$PWD/dqforeignkey.h \
    $$PWD/dqsharedquery.h \
    $$PWD/dqquery.h \
    $$PWD/dqqueryrules.h \
    $$PWD/dqexpression.h \
    $$PWD/dqlist.h \
    $$PWD/dqabstractmodel.h \
    $$PWD/dqsharedlist.h \
    $$PWD/dqindex.h \
    $$PWD/dqstream.h \
    $$PWD/dqlistwriter.h \
    $$PWD/dquest.h

DQUEST_PRIV_HEADERS = \
    $$PWD/dqwhere_p.h \
    $$PWD/dqsharedquery_p.h \
    $$PWD/dqmetainfoquery_p.h

HEADERS += $$DQUEST_HEADERS
HEADERS += $$DQUEST_PRIV_HEADERS

SOURCES += \
    $$PWD/dqclause.cpp \
    $$PWD/dqmodelmetainfo.cpp \
    $$PWD/dqmodel.cpp \
    $$PWD/dqconnection.cpp \
    $$PWD/dqbasefield.cpp \
    $$PWD/dqsqlstatement.cpp \
    $$PWD/dqsqlitestatement.cpp \
    $$PWD/dqwhere.cpp \
    $$PWD/dqsql.cpp \
    $$PWD/dqfield.cpp \
    $$PWD/dqsharedquery.cpp \
    $$PWD/dqqueryrules.cpp \
    $$PWD/dqexpression.cpp \
    $$PWD/dqabstractmodel.cpp \
    $$PWD/dqsharedlist.cpp \
    $$PWD/dqindex.cpp \
    $$PWD/dqstream.cpp \
    $$PWD/dqlistwriter.cpp
