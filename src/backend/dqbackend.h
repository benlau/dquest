#ifndef DQBackend_h
#define DQBackend_h

#include <backend/dqbackendengine.h>


/// DQuest database backend management
/**
 *
 *
 */
class DQBackend {

public:
    ///  Register a database backend engine.
    /** 
     *
     *  @param name The name of the database engine
     *  @param driver The supported database driver in QSqlDatabase
     *  
     */
    template <typename T>
    static bool registerEngine(QString name, QString driver) {
        return DQBackend::registerEngine(dqBackendEngineCreateFunc<T>,name,driver);
    }

    ///  Register a database backend engine.
    /**
     *  @param func The creation function of the DQBackendEngine class instance
     *  @param name The name of the database engine
     *  @param driver The supported database driver in QSqlDatabase
     *
     */
    static bool registerEngine(DQBackendEngineCreatorFunc func,QString name, QString driver);

    /// Get the list of registered database engine
    static QStringList listEngine();

    /// Set the named database backend engine be the default engine for driver
    static bool setDefaultEngine(QString name, QString driver);

    /// Create a DQBackendEngine class instance according to the name
    static DQBackendEngine* createEngine(QString name);

    /// Create a DQBackendEngine class instance according to the database driver
    static DQBackendEngine* createEngineForDriver(QString driver);

    /// Return TRUE if the driver is supported
    static bool isDriverSupported(QString driver);

    /// Given the name of a backend engine. Return the driver supported
    static QString supportedDriver(QString name);

};


#endif // DQBackend_h
