/**
    @author Ben Lau
 */

#ifndef DQBACKENDREGISTERHELPER_H
#define DQBACKENDREGISTERHELPER_H

#include <QtCore>
#include <backend/dqbackend.h>

/// A helper class for backend registration

template <typename T>
class DQBackendRegisterHelper {
public:
    /// Construct a register helper. It will register the backend automatically
    /**
      @param name The name of the backend engine
      @param driver The QSqlDatabase driver that is wrapped by this backend
     */
    DQBackendRegisterHelper(QString name , QString driver);

private:
    /// It is called after registration. Using template partial specialization could override the behaviour
    /**
      Reference : The implementation of DQSqliteEngine
     */
    void postProcess(QString name,QString driver);
};

template <typename T>
DQBackendRegisterHelper<T>::DQBackendRegisterHelper(QString name,QString driver) {
    if (!DQBackend::registerEngine<T>(name,driver)){
        qWarning() << QString("Failed to register backend \"%1\" for %2").arg(name).arg(driver);
    } else {
        postProcess(name,driver);
    }
}


template <typename T>
void DQBackendRegisterHelper<T>::postProcess(QString name,QString driver) {
    // Empty implementation
}

#endif // DQBACKENDREGISTERHELPER_H
