/**
    @author Ben Lau
 */

#ifndef ACCESSTESTS_H
#define ACCESSTESTS_H

#include <QObject>
#include <dquest.h>
#include "model1.h"
#include "model2.h"
#include "model3.h"
#include "model4.h"
#include "model5.h"
#include "config.h"
#include "misc.h"

/// Database access test for different backend

class AccessTests : public QObject
{
    Q_OBJECT
public:
    explicit AccessTests(QObject *parent = 0);
    
signals:
    
public slots:

protected:
    DQConnection conn1,conn2;
    QSqlDatabase db,db2;

private slots:

    /// Test DQModel::save()
    void dqModelSave();

    /// Test DQQuery::deletFrom without filter
    /**
      It will clear all the record made by previous operation.
     */
    void deleteAll();

    /// Insert pre-defined records for each model. They may needed for following tests
    /**
     */
    void prepareInitRecords();

    /// test basic function of DQQuery / DQSharedQuery
    void query();

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

    /// test second connection
    void secondConnection();


    /// Test can DQConnection return the correct lastQuery object
    void lastQuery();

    /// Test transaction
    void transaction();



};

#endif // ACCESSTESTS_H
