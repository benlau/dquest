#include <backend/dqbackend.h>


bool DQBackend::registerEngine(DQEngineCreatorFunc func,QString name, QString driver)
{
    return false;
}

QStringList DQBackend::listEngine()
{
}

bool DQBackend::setDefaultEngine(QString name, QString driver)
{
    return false;
}

DQEngine* DQBackend::createEngineForDriver(QString driver)
{
}

DQEngine* DQBackend::createEngine(QString name)
{
    return 0;
}

bool DQBackend::isDriverSupported(QString driver)
{
    return false;
}
