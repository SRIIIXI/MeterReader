#include "EntityAlarmFilter.hpp"

EntityAlarmFilter* alarmFilterEntityPtr = nullptr;

EntityAlarmFilter::EntityAlarmFilter(QSqlDatabase *db, QString tabname) : DataEntity(db, tabname)
{
    alarmFilterEntityPtr = this;
}

EntityAlarmFilter::~EntityAlarmFilter()
{
}

bool EntityAlarmFilter::createTable()
{
    QString sqlString = "CREATE TABLE alarm_filter( \
                        alarm_id Integer NOT NULL PRIMARY KEY, \
                        alarm_name Text NOT NULL, \
                        is_set Integer NOT NULL, \
                        CONSTRAINT unique_alarm_id UNIQUE ( alarm_id ))";

    return executeQuery(sqlString);
}

bool EntityAlarmFilter::createAlarmFilter(const AlarmFilter &rec)
{
    QString sqlString = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(alarm_id__, 'alarm_name__', is_set__)";

    sqlString = sqlString.replace("alarm_id__", QVariant(rec.AlarmId_).toString());
    sqlString = sqlString.replace("alarm_name__", rec.AlarmName_);
    sqlString = sqlString.replace("is_set__", QVariant(rec.IsSet_).toString());

    return executeQuery(sqlString);
}

bool EntityAlarmFilter::allAlarmFilters(QList<AlarmFilter> &list)
{
    QString str;
    QList<QSqlRecord> *reclist = getAllRecords(str);

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        AlarmFilter struc;

        struc.AlarmId_ = rec.value(0).toInt();
        struc.AlarmName_ = rec.value(1).toString();
        struc.IsSet_ = rec.value(2).toInt();

        list.append(struc);
    }

    return true;
}

bool EntityAlarmFilter::selectedAlarmFilters(QList<AlarmFilter> &list, QString keyname, QVariant value)
{
    QString str;
    QList<QSqlRecord> *reclist = getSelectedRecords(keyname, value, str);

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        AlarmFilter struc;

        struc.IsSet_ = rec.value(0).toInt();
        struc.AlarmName_ = rec.value(1).toString();
        struc.AlarmId_ = rec.value(2).toInt();

        list.append(struc);
    }

    return true;
}


