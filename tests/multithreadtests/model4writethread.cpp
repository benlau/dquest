/**
    @author Ben Lau
 */

#include <QSqlError>
#include "model4writethread.h"
#include "model4.h"

Model4WriteThread::Model4WriteThread(QObject *parent) :
    QThread(parent)
{
    number = 100;

    written = 0;
}

void Model4WriteThread::setNumber(int val) {
    number = val;
}

void Model4WriteThread::run () {
    for (int i = 0 ; i < number ;i++ ) {
        Model4 model;
        model.key = QString("%1/%2").arg(objectName()).arg(i);
        model.description = objectName();
        QString order;
        order.setNum(i);
        model.help = order;
        model.setConnection(conn);
        bool res = model.save();
        if (!res) {
            qDebug() << conn.lastQuery().lastError();
        }
    }
}

void Model4WriteThread::setConnection(DQConnection val){
    conn = val;
}
