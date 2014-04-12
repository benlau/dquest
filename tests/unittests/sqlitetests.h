#ifndef SQLITETESTS_H
#define SQLITETESTS_H

#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

#include <QSqlError>
#include <dqconnection.h>
#include <dqsqlitestatement.h>
#include <dqquery.h>
#include <dqsql.h>
#include <dqlistwriter.h>

#include "model1.h"
#include "model2.h"
#include "model3.h"
#include "model4.h"
#include "model5.h"
#include "config.h"
#include "misc.h"

class SqliteTests : public QObject
{
    Q_OBJECT

public:
    SqliteTests(QObject* parent = 0);

    /// Verify the create table stmt
    void verifyCreateTable();

    /// Test the basic operation of DQForeignKey
    void foreignKey();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void insertInto();

    /// Test DQModel::save()
    void dqModelSave();

    /// Test DQQuery::deletFrom without filter
    /**
      It will clear all the record made by previous operation.
     */
    void deleteAll();

    /// Insert pre-defined records for each model. They may needed for following tests
    /**
      @todo Should be part of initTestCase
     */
    void prepareInitRecords();

    void select();

    void queryAll();
    void querySelect();

    /// test different combination of where
    void querySelectWhere();

    /// Test can it load model through foreign key
    void foreignKeyLoad();

    /// Test Model4 access
    void model4();

    /// Test date time access
    void datetime();

    /// Verify the save and load for specific type
    void checkTypeSaveAndLoad();

    void queryOrderBy();

    void secondConnection();

private:
    DQConnection conn1,conn2;
    QSqlDatabase db;
};

#endif // SQLITETESTS_H
