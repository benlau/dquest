#include <QMap>
#include <backend/dqbackend.h>

class DQBackendPrivItem {
public:
    DQEngineCreatorFunc func;
    QString name;
    QString driver;
};

static QMap<QString,DQBackendPrivItem> nameMap;

static QMap<QString,QString> driverToName;

/// If it is set, it will always use this mapping instead of driverToName which is determined by registration order.
static QMap<QString,QString> defaultDriverToName;

bool DQBackend::registerEngine(DQEngineCreatorFunc func,QString name, QString driver)
{
    if (nameMap.contains(name))
        return false;

    DQBackendPrivItem item;
    item.func = func;
    item.name = name;
    item.driver = driver;

    nameMap[name] = item;

    if (!driverToName.contains(driver)) {
        driverToName[driver] = name;
    }

    return true;
}

QStringList DQBackend::listEngine()
{
    return nameMap.keys();
}

bool DQBackend::setDefaultEngine(QString name, QString driver)
{
    if (defaultDriverToName.contains(driver))
        return false;
    defaultDriverToName[driver] = name;

    return true;
}

DQEngine* DQBackend::createEngineForDriver(QString driver)
{
    DQEngine* res = 0;
    QString name;

    if (defaultDriverToName.contains(driver)) {
        name = defaultDriverToName[driver];
    } else if (driverToName.contains(driver)) {
        name = driverToName[driver];
    }

    return createEngine(name);
}

DQEngine* DQBackend::createEngine(QString name)
{
    DQEngine* res = 0;
    if (nameMap.contains(name))
        res = nameMap[name].func();
    return res;
}

bool DQBackend::isDriverSupported(QString driver)
{
    return driverToName.contains(driver);
}
