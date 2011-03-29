#include <QtCore>
#include "dqwhere.h"
#include "dqwhere_p.h"

/* Test cases:

  coretests::where()

  sqlitetests::querySelectWhere()

 */

static int typeId = qMetaTypeId<DQWhere>();

static QString variantToString(QVariant v,bool quoteString);

DQWhereDataPriv::DQWhereDataPriv(){
    m_type = None;
}

DQWhereDataPriv::DQWhereDataPriv(Type type) : m_type(type){
}

DQWhereDataPriv& DQWhereDataPriv::operator<<(QVariant v){
    m_list << v;
    return *this;
}

QList<QVariant> DQWhereDataPriv::list(){
    return m_list;
}

void DQWhereDataPriv::setList(QList<QVariant> list){
    m_list = list;
}

DQWhereDataPriv::Type DQWhereDataPriv::type(){
    return m_type;
}

/// A private datastructure to represent the database field in DQWhere
class DQWhereFieldPriv : public QString {
};

Q_DECLARE_METATYPE(DQWhereFieldPriv)

static int fieldTypeId = qMetaTypeId<DQWhereFieldPriv>();

DQWhere::DQWhere()
{
    m_isNull = true;
}

DQWhere::DQWhere(QString field,QString op, QVariant right)
    : m_op(op),m_right(right){

    m_left = DQWhere(field);

    m_isNull = false;
}

DQWhere::DQWhere(const DQWhere &other){
    m_left = other.m_left;
    m_right = other.m_right;
    m_op = other.m_op;
    m_isNull = other.m_isNull;
}

DQWhere::DQWhere(QString fieldAndOp , QVariant right)  : m_right(right){
    QRegExp rx("^\\s*[a-zA-Z0-9]+");
    int pos = rx.indexIn(fieldAndOp);

    if (pos < 0){
        qWarning() << QString("DQWhere() : can not parse %1").arg(fieldAndOp);
        return;
    }
    int len = rx.matchedLength();

    QString str = fieldAndOp.mid(pos,len);
//    m_left = str.trimmed();
    m_left = DQWhere(str.trimmed());

    str = fieldAndOp.mid(pos + len);

    m_op = str.trimmed();

    m_isNull = false;
}

DQWhere::DQWhere(QString field){
    DQWhereFieldPriv f;
    f.append(field);
    m_left.setValue<DQWhereFieldPriv>(f);
    m_isNull = false;
}


QVariant DQWhere::left(){
    return m_left;
}

QVariant DQWhere::right(){
    return m_right;
}

QString DQWhere::op(){
    return m_op;
}


bool DQWhere::isNull(){
    return m_isNull;
}

bool DQWhere::isField() {
    return m_right.isNull() && m_left.userType() == fieldTypeId;
}

QString DQWhere::toString() {
    if (isField()) {
        return variantToString(m_left,false);
    }

    if (m_left.isNull() || m_right.isNull())
        return "";

    QString op1,op2;

    op1 = variantToString(m_left,false);

    op2 = variantToString(m_right,true);

    return QString("%1 %2 %3").arg(op1).arg(m_op).arg(op2);
}

DQWhere DQWhere::expr(QString op,QVariant right){
    DQWhere w;

    w.m_left.setValue<DQWhere>(*this);
    w.m_right = right;
    w.m_op = op;
    w.m_isNull = false;

    return w;
}

/*
DQWhere DQWhere::operator&(const DQWhere other) {
    DQWhere w;

    w.m_left.setValue<DQWhere>(*this);
    w.m_right.setValue<DQWhere>( other);
    w.m_op = "and";
    w.m_isNull = false;

    return w;
}
*/

DQWhere DQWhere::operator&&(const DQWhere other) {
    DQWhere w;

    w.m_left.setValue<DQWhere>(*this);
    w.m_right.setValue<DQWhere>( other);
    w.m_op = "and";
    w.m_isNull = false;

    return w;
}

/*
DQWhere DQWhere::operator|(const DQWhere other) {
    DQWhere w;

    w.m_left.setValue<DQWhere>(*this);
    w.m_right.setValue<DQWhere>( other);
    w.m_op = "or";
    w.m_isNull = false;

    return w;
}
*/

DQWhere DQWhere::operator||(const DQWhere other) {
    DQWhere w;

    w.m_left.setValue<DQWhere>(*this);
    w.m_right.setValue<DQWhere>( other);
    w.m_op = "or";
    w.m_isNull = false;

    return w;
}

DQWhere DQWhere::operator< (QVariant right){
    return expr("<",right);
}

DQWhere DQWhere::operator<= (QVariant right){
    return expr("<=",right);
}

DQWhere DQWhere::operator> (QVariant right){
    return expr(">",right);
}

DQWhere DQWhere::operator>= (QVariant right){
    return expr(">=",right);
}

DQWhere DQWhere::operator==(QVariant right){
    return expr("=",right);
}

DQWhere DQWhere::operator!=(QVariant right){
    return expr("<>",right);
}

DQWhere DQWhere::operator+(QVariant right){
    return expr("*",right);
}

DQWhere DQWhere::operator-(QVariant right){
    return expr("*",right);
}

DQWhere DQWhere::operator*(QVariant right){
    return expr("*",right);
}

DQWhere DQWhere::operator/(QVariant right){
    return expr("/",right);
}

DQWhere DQWhere::operator%(QVariant right){
    return expr("%",right);
}

DQWhere DQWhere::equal(QVariant right){
    return expr("=",right);
}

DQWhere DQWhere::notEqual(QVariant right){
    return expr("<>",right);
}

DQWhere DQWhere::between(QVariant v1,QVariant v2){
    DQWhereDataPriv data(DQWhereDataPriv::Between);
    data << v1 << v2;
    QVariant v;
    v.setValue<DQWhereDataPriv>(data);

    return expr("between",v);
}

DQWhere DQWhere::in(QList<QVariant> list){
    DQWhereDataPriv data(DQWhereDataPriv::In);
    data.setList(list);
    QVariant v;
    v.setValue<DQWhereDataPriv>(data);

    return expr("in",v);
}

DQWhere DQWhere::notIn (QList<QVariant> list) {
    DQWhereDataPriv data(DQWhereDataPriv::In);
    data.setList(list);
    QVariant v;
    v.setValue<DQWhereDataPriv>(data);

    return expr("not in",v);
}

DQWhere DQWhere::like (QVariant other){
    return expr("like",other);
}

DQWhere DQWhere::glob (QVariant other){
    return expr("glob",other);
}

DQWhere DQWhere::is (QVariant other){
    return expr("is",other);
}

DQWhere DQWhere::isNot(QVariant other){
    return expr("is not",other);
}

DQWhere::operator QVariant() const{
    QVariant v;
    v.setValue<DQWhere>(*this);
    return v;
}

QString variantToString(QVariant v,bool quoteString){
    QString res;
    /// @todo Implement QVariant::convert()
//    qDebug() << v.userType() << v;

    if (v.type() == QVariant::UserType &&
        v.userType() == typeId) {

        DQWhere w = v.value<DQWhere>();
        QString pattern = "( %1 )";
        if (w.isField()) {
            pattern = "%1";
        }
        res = QString(pattern).arg(w.toString() );
    } else if (v.userType() == fieldTypeId) {
        DQWhereFieldPriv f;
        f = v.value<DQWhereFieldPriv>();
        res = f;
    } else if (v.type() == QVariant::String
               && quoteString) {
        res = QString("\"%1\"").arg(v.toString());
    } else {
        res = v.toString();
    }

    return res;
}

