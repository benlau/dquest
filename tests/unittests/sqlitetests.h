#ifndef SQLITETESTS_H
#define SQLITETESTS_H

#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

#include <QSqlError>
#include <dqconnection.h>
#include <priv/dqsqlitestatement.h>
#include <dqquery.h>
#include <backend/dqsql.h>
#include <backend/dqbackend.h>
#include <priv/dqsqliteengine.h>
#include <dqlistwriter.h>

#include "accesstests.h"
#include "model1.h"
#include "model2.h"
#include "model3.h"
#include "model4.h"
#include "model5.h"
#include "config.h"
#include "misc.h"

class SqliteTests : public AccessTests
{
    Q_OBJECT

public:
    SqliteTests(QObject* parent = 0);

    /// Test the backend system
    void backend();

    /// Verify the create table stmt
    void verifyCreateTable();

    /// Test the basic operation of DQForeignKey
    void foreignKey();

private Q_SLOTS:
    void initTestCase();

    void cleanupTestCase();

    void insertInto();

    /// Describe a table
    void describe();

    /// Verify DQBackendEngine
    void engine();

};

#endif // SQLITETESTS_H
