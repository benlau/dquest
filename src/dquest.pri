INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD $$PWD/priv $$PWD/backend
QT += sql

QMAKE_CXXFLAGS += -Wno-invalid-offsetof

DQUEST_HEADERS += \
    $$PWD/dqclause.h \
    $$PWD/dqmodelmetainfo.h \
    $$PWD/dqmodel.h \
    $$PWD/dqconnection.h \
    $$PWD/dqbasefield.h \
    $$PWD/dqwhere.h \
    $$PWD/dqfield.h \
    $$PWD/dqforeignkey.h \
    $$PWD/dqsharedquery.h \
    $$PWD/dqquery.h \
    $$PWD/dqlist.h \
    $$PWD/dqabstractmodel.h \
    $$PWD/dqsharedlist.h \
    $$PWD/dqindex.h \
    $$PWD/dqstream.h \
    $$PWD/dqlistwriter.h \
    $$PWD/dquest.h

DQUEST_PRIV_HEADERS = \
    $$PWD/priv/dqsqliteengine.h \
    $$PWD/priv/dqsqlitestatement.h \
    $$PWD/priv/dqwhere_p.h \
    $$PWD/priv/dqsharedquery_p.h \
    $$PWD/priv/dqmetainfoquery_p.h

DQUEST_BACKEND_HEADERS = \
    $$PWD/backend/dqsql.h \
    $$PWD/backend/dqqueryrules.h \
    $$PWD/backend/dqexpression.h \
    $$PWD/backend/dqqueryengine.h \
    $$PWD/backend/dqengine.h \
    $$PWD/backend/dqbackendquery.h \
    $$PWD/backend/dqsqlstatement.h

HEADERS += $$DQUEST_HEADERS

HEADERS += $$DQUEST_BACKEND_HEADERS

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
    $$PWD/dqlistwriter.cpp \
    $$PWD/dqsqliteengine.cpp \
    $$PWD/dqengine.cpp \
    $$PWD/dqqueryengine.cpp \
    $$PWD/backend/dqbackendquery.cpp
