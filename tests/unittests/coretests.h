#ifndef CORETESTS_H
#define CORETESTS_H

#include <QObject>

#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>
#include "dqwhere.h"
#include "dqclause.h"
#include "priv/dqsqlitestatement.h"
#include "model1.h"
#include "model2.h"
#include "model3.h"
#include "model4.h"
#include "model5.h"
#include "dqqueryrules.h"
#include "dqquery.h"
#include "dqexpression.h"
#include "dqlist.h"
#include "misc.h"
#include "dqstream.h"
#include "dqlistwriter.h"

/// A set of tests which don't involve database access

class CoreTests : public QObject
{
    Q_OBJECT

public:
    CoreTests(QObject* parent = 0);

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void metaInfo();

    void modelCast();

    void sqliteColumnConstraint();
    void sqlCreateTable();

    /// test DSIndex
    void index();

    /// Test DQClause
    void clause();

    void where();

    void expression();

    /// Test Model1 declaration
    void mode1l();

    void model1_accessFields();
    void model1_equalTo_model2();

    void model2();

    void model3();

    void model4();

    void model5();

    void queryrules();

    void dqList();

    /// Test DQField<QStringList>
    void stringlistField();
    void stringlistField_data();

    /// test DQStream
    void stream();

    /// Test DQListWriter
    void listWriter();

};



#endif // CORETESTS_H
