#include "DataEntity.hpp"

DataEntity::DataEntity(QSqlDatabase *db, QString tabname)
{
    _Database = db;
    _TableName = tabname;
}

DataEntity::~DataEntity()
{
}

void DataEntity::clear()
{
    _RecordList.clear();
}

void DataEntity::truncate()
{
    QString querystring;
    querystring = "delete from " + _TableName;

    QSqlQuery query = _Database->exec(querystring);
}

QList<QSqlRecord>* DataEntity::getAllRecords(QString str)
{
    _RecordList.clear();

    QString querystring;

    if(str.length()< 1 || str.isEmpty())
    {
        querystring = "select * from " + _TableName;
    }
    else
    {
        querystring = str;
    }

    QSqlQuery query = _Database->exec(querystring);

    while(query.next())
    {
        _RecordList.append(query.record());
    }

    return &_RecordList;
}

QList<QSqlRecord>* DataEntity::getSelectedRecords(QString keyname, QVariant value, QString str)
{
    _RecordList.clear();

    QString valsub = "";
    substitueValue(value, valsub);

    QString querystring;

    if(str.length()< 1 || str.isEmpty())
    {
        querystring = "select * from " + _TableName + " where " + keyname + " = " + valsub;
    }
    else
    {
        querystring = str;
        querystring += " where " + keyname + " = " + valsub;
    }

    QSqlQuery query = _Database->exec(querystring);

    while(query.next())
    {
        _RecordList.append(query.record());
    }

    return &_RecordList;
}

QList<QSqlRecord>* DataEntity::getCustomRecords(QString str)
{
    _RecordList.clear();

    QSqlQuery query = _Database->exec(str);

    while(query.next())
    {
        _RecordList.append(query.record());
    }

    return &_RecordList;
}


bool DataEntity::remove(QString keyname, QVariant value)
{
    QSqlQuery qryresult;
    QString err;
    QString valsub = "";

    substitueValue(value, valsub);

    QString querystring = "delete from " + _TableName + " where " + keyname + " = " + valsub;

    try
    {
        _Database->transaction();

        qryresult = _Database->exec(querystring);

        _Database->commit();
    }
    catch(QException e)
    {
       _Database->rollback();
       err = _Database->lastError().text();
       qDebug() << err;
       return false;
    }

    return true;
}

bool DataEntity::update(QString uniquekeyname, QVariant uniquevalue, QString keyname, QVariant value)
{
    QSqlQuery qryresult;
    QString err;
    QString valsub = "";
    QString uniquevalsub = "";

    substitueValue(value, valsub);
    substitueValue(uniquevalue, uniquevalsub);

    QString querystring = "update " + _TableName + " set " + keyname + " = " + valsub+ " where " + uniquekeyname + " = " + uniquevalsub;

    try
    {
        _Database->transaction();

        qryresult = _Database->exec(querystring);

        _Database->commit();
    }
    catch(QException e)
    {
       _Database->rollback();
       err = _Database->lastError().text();
       qDebug() << err;
       return false;
    }

    return true;
}

QVariant DataEntity::maximumValue(QString keyname)
{
    QVariant retval;

    QString sqlstring = "select distinct _key_ from _table_ order by _key_  desc limit 1";
    sqlstring = sqlstring.replace("_table_", _TableName);
    sqlstring = sqlstring.replace("_key_", keyname);

    QSqlQuery query = _Database->exec(sqlstring);

    if(query.next())
    {
        retval = query.record().value(0);
    }

    return retval;
}

QVariant DataEntity::maximumValue(QString keyname, QString filter, QVariant filterval)
{
    QVariant retval;

    QString valsub = "";
    substitueValue(filterval, valsub);

    QString sqlstring = "select _key_ from _table_ where _filter_ = _filterval_ order by _key_ desc limit 1";
    sqlstring = sqlstring.replace("_table_", _TableName);
    sqlstring = sqlstring.replace("_key_", keyname);
    sqlstring = sqlstring.replace("_filter_", filter);
    sqlstring = sqlstring.replace("_filterval_", valsub);

    QSqlQuery query = _Database->exec(sqlstring);

    if(query.next())
    {
        retval = query.record().value(0);
    }

    return retval;
}

void DataEntity::substitueValue(QVariant &var, QString &str)
{
    switch(var.type())
    {
        case QVariant::Char:
        case QVariant::String:
        {
            str = "'" + var.toString() + "'";
        }
        break;

        case QVariant::Double:
        case QVariant::Int:
        case QVariant::LongLong:
        case QVariant::UInt:
        case QVariant::ULongLong:
        {
            str = var.toString();
        }
        break;

        default:
        {
        }
        break;
    }
}

QString DataEntity::getFloatString(const float val)
{
    char temp[24] = {0};
    sprintf(temp, "%0.4f", val);
    return temp;
}

void DataEntity::normalizeDateString(QString &str)
{
    QDate dt = QDate::fromString(str, "dd/MM/yyyy");
    str = dt.toString("yyyy-MM-dd");
}

void DataEntity::normalizeDateString(uint32_t val, QString &str)
{
    QDateTime dt = QDateTime::fromSecsSinceEpoch(val);
    str = dt.toString("yyyy-MM-dd");
}

QString DataEntity::getDayofWeek(QString str)
{
    if(str == "0")
    {
        return "Sun";
    }

    if(str == "1")
    {
        return "Mon";
    }

    if(str == "2")
    {
        return "Tue";
    }

    if(str == "3")
    {
        return "Wed";
    }

    if(str == "4")
    {
        return "Thu";
    }

    if(str == "5")
    {
        return "Fri";
    }

    if(str == "6")
    {
        return "Sat";
    }

    return "";
}

QString DataEntity::getMonth(QString str)
{
    if(str == "01")
    {
        return "Jan";
    }

    if(str == "02")
    {
        return "Feb";
    }

    if(str == "03")
    {
        return "Mar";
    }

    if(str == "04")
    {
        return "Apr";
    }

    if(str == "05")
    {
        return "May";
    }

    if(str == "06")
    {
        return "Jun";
    }

    if(str == "07")
    {
        return "Jul";
    }

    if(str == "08")
    {
        return "Aug";
    }

    if(str == "09")
    {
        return "Sep";
    }

    if(str == "10")
    {
        return "Oct";
    }

    if(str == "11")
    {
        return "Nov";
    }

    if(str == "12")
    {
        return "Dec";
    }

    return "";
}

