/**
    @author Ben Lau
 */

#ifndef DQMTSQLITEBACKENDENGINE_H
#define DQMTSQLITEBACKENDENGINE_H

#include <dqproxybackendengine.h>

/// Multi-thread Sqlite backend engine

class DQMtSqliteBackendEngine : public DQProxyBackendEngine
{
public:
    DQMtSqliteBackendEngine();

    /// Get the name of the engine
    virtual QString name();

    /// Open a database connection
    virtual bool open(QSqlDatabase db);

    /// Is it opened connection to database?
    virtual bool isOpen() const;

    /// Close the connection to database
    virtual void close();

};

#endif // DQMTSQLITEBACKENDENGINE_H
