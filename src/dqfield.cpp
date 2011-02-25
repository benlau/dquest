#include <QtCore>
#include "dqfield.h"

/// Separator for StringList storage
#define SEP " & "

/// Replace & and " by HTML entities.

static QString escape(QString value) {
    QString result;
    QMap<char , QString> map;
    map['&'] = "&amp;";
    map['"'] = "&quot;";

    result.reserve(value.size() * 1.1);
    int n = value.size();
    for (int i = 0 ; i < n;i++) {
        QChar c = value.at(i);
        char l = c.toLatin1();
        if (map.contains(l)) {
            result += map[l];
        } else {
            result += c;
        }
    }
    result.squeeze();
    return result;
}

static QString unescape(QString value) {
    QMap<QString,char> map;
    map["&amp;"] = '&';
    map["&quot;"] = '"';

    QString result;
    result.reserve(value.size());

    int n = value.size();

    for (int i = 0 ; i<n;i++){
        QChar q = value.at(i) ;

        if (q == '&') {
            bool found = false;
            QMap<QString, char>::const_iterator iter = map.constBegin();
            while (iter!= map.end()) {
                QString key = iter.key();
                int len = key.size();

                QStringRef ref(&value,i,len);
                if (ref == key) {
                    q = iter.value();
                    i+=len-1;
                    found = true;
                    break;
                }
                iter++;
            }

            if (!found) {
                qWarning() << QString("Invalid escaped string : %1").arg(value);
            }
        }

        result += q;
    }

    result.squeeze();
    return result;
}

template <>
bool DQField<QStringList>::set(QVariant value){
    QString str;

    if (value.type() == QVariant::String) {
        str = value.toString();
        QStringList list = str.split(SEP);
        QStringList result;

        foreach (str,list) {
            result << unescape(str);
        }

        value = result;
    }

//    qDebug() << __func__ << str;
    return DQBaseField::set(value);;
}

template <>
QVariant DQField<QStringList>::get(bool convert) const {
    QVariant val = DQBaseField::get(convert);

    if (convert && val.type() == QVariant::StringList ) {
        QStringList list = val.toStringList();
        QStringList result;
        QString str;

        foreach (str,list) {
            result << escape(str);
        }

        str = result.join(SEP);
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
