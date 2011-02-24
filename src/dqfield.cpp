#include <QtCore>
#include "dqfield.h"

/// Separator for StringList storage
#define SEP " & "

/// Replace & and " by HTML entities.

static QString escape(QString value) {
    return value;
}

template <>
bool DQField<QStringList>::set(QVariant value){
    bool res ;
    QString str;

    if (value.type() == QVariant::String) {
        str = value.toString();
        QStringList list = str.split(SEP);

        value = list;
    }

//    qDebug() << __func__ << str;
    return DQBaseField::set(value);;
}

template <>
QVariant DQField<QStringList>::get(bool convert) const {
    QVariant val = DQBaseField::get(convert);

    if (convert && val.type() == QVariant::StringList ) {
        QStringList list = val.toStringList();
        QString str;

        str = list.join(SEP);
        val = str;
    }
//    qDebug() << __func__ << str << list;

    return val;
}


DQPrimaryKey::DQPrimaryKey(){
}

DQClause DQPrimaryKey::clause(){
    return DQClause(DQClause::PRIMARY_KEY);
}
