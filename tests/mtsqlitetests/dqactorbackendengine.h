/**
    @author Ben Lau
 */

#ifndef DQACTORBACKENDENGINE_H
#define DQACTORBACKENDENGINE_H

#include <QObject>
#include <backend/dqbackendengine.h>

/// An actor backend engine
/** It is decorator of other backend engine that work as an actor. The
  backend engine is able to be called by another thread through
  the signal and slot mechanim in Qt
 */

class DQActorBackendEngine : public QObject ,public DQBackendEngine
{
    Q_OBJECT
public:
    explicit DQActorBackendEngine(QObject *parent);
    ~DQActorBackendEngine();

    /// Set the wrapped backend engine
    /**
      @param engine An instance of DQBackendEngine. Ownership will be taken
     */
    void setBackendEngine(DQBackendEngine *engine);

    DQBackendEngine *engine();

public slots:
    virtual bool open(QSqlDatabase  db);

    virtual bool open(QString name);

    virtual bool isOpen() const;

    virtual void close();

    virtual bool addModel(DQModelMetaInfo* info);

    virtual QList<DQModelMetaInfo*> modelList() const;

    virtual bool createModel(DQModelMetaInfo* info);

    virtual bool dropModel(DQModelMetaInfo* info);

    virtual bool existsModel(DQModelMetaInfo* info);

    virtual bool update(DQAbstractModel* model, QStringList fields = QStringList(), bool forceInsert = false);

    virtual bool createIndex(const DQBaseIndex &index);

    virtual bool dropIndex(QString name);

    virtual DQBackendQuery query(DQQueryRules rules);

    virtual bool transaction();

    virtual bool commit();

    virtual bool rollback();

private:
    DQBackendEngine *m_engine;
};

#endif // DQACTORBACKENDENGINE_H
