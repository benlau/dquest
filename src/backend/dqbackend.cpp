#include <QMap>
#include <backend/dqbackend.h>

class DQBackendPrivItem {
public:
    DQBackendEngineCreatorFunc func;
    QString name;
    QString driver;
};

static QMap<QString,DQBackendPrivItem>* nameMap = 0;
static QMap<QString,QString>* driverToName = 0;
/// If it is set, it will always use this mapping instead of driverToName which is determined by registration order.
static QMap<QString,QString>* defaultDriverToName = 0;

/// Private data structure of DQBackend. It is mainly used for initialization and release memory on quit
class DQBackendPriv {
public:

    DQBackendPriv(){
        init();
    }

    ~DQBackendPriv() {
        if (nameMap != 0)
            delete nameMap;
        if (driverToName != 0)
            delete driverToName;
        if (defaultDriverToName != 0)
            delete defaultDriverToName;
    }

    void init() {
        if (nameMap == 0)
            nameMap = new QMap<QString,DQBackendPrivItem>();
        if (driverToName == 0)
            driverToName = new QMap<QString,QString>();
        if (defaultDriverToName == 0)
            defaultDriverToName = new QMap<QString,QString>();
    }
};

static DQBackendPriv priv;

bool DQBackend::registerEngine(DQBackendEngineCreatorFunc func,QString name, QString driver)
{
    // Since registerEngine can be called before main() and initialization of priv data structure,
    // this will ensure those QMap datastrucutre has been initialization before use.
    priv.init();

    if (nameMap->contains(name))
        return false;

    DQBackendPrivItem item;
    item.func = func;
    item.name = name;
    item.driver = driver;

    (*nameMap)[name] = item;

    if (!driverToName->contains(driver)) {
        (*driverToName)[driver] = name;
    }

    return true;
}

QStringList DQBackend::listEngine()
{
    return nameMap->keys();
}

bool DQBackend::setDefaultEngine(QString name, QString driver)
{
    if (defaultDriverToName->contains(driver))
        return false;
    (*defaultDriverToName)[driver] = name;

    return true;
}

DQBackendEngine* DQBackend::createEngineForDriver(QString driver)
{
    QString name;

    if (defaultDriverToName->contains(driver)) {
        name = (*defaultDriverToName)[driver];
    } else if (driverToName->contains(driver)) {
        name = (*driverToName)[driver];
    }

    return createEngine(name);
}

DQBackendEngine* DQBackend::createEngine(QString name)
{
    DQBackendEngine* res = 0;
    if (nameMap->contains(name))
        res = (*nameMap)[name].func();
    return res;
}

bool DQBackend::isDriverSupported(QString driver)
{
    return driverToName->contains(driver);
}
