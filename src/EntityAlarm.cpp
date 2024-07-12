#include "EntityAlarm.hpp"

EntityAlarm* alarmEntityPtr = nullptr;

EntityAlarm::EntityAlarm(QSqlDatabase *db, QString tabname) : DataEntity(db, tabname)
{
    alarmEntityPtr = this;
}

EntityAlarm::~EntityAlarm()
{
}

bool EntityAlarm::createTable()
{
    QString sqlString = "CREATE TABLE alarm ( \
                        alarm_id Integer NOT NULL, \
                        serial_no Text NOT NULL, \
                        alarm_name Text NOT NULL, \
                        alarm_timestamp Integer NOT NULL )";

    return executeQuery(sqlString);
}

bool EntityAlarm::createAlarm(const Alarm &rec)
{
    QString sqlString = "insert into alarm (alarm_id, serial_no, alarm_name, alarm_timestamp) values(alarm_id__, 'serial_no__', 'alarm_name__', alarm_timestamp__)";

    sqlString = sqlString.replace("alarm_id__", QVariant(rec.AlarmId_).toString());
    sqlString = sqlString.replace("alarm_name__", rec.AlarmName_);
    sqlString = sqlString.replace("serial_no__", rec.SerialNo_);
    sqlString = sqlString.replace("alarm_timestamp__", QVariant(rec.AlarmTimestamp_).toString());

    return executeQuery(sqlString);
}

bool EntityAlarm::selectedAlarms(QList<Alarm> &list, QString keyname, QVariant value)
{
    QString str;
    QList<QSqlRecord> *reclist = getSelectedRecords(keyname, value, str);

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        Alarm struc;

        struc.AlarmId_ = rec.value(0).toInt();
        struc.SerialNo_ = rec.value(1).toString();
        struc.AlarmName_ = rec.value(2).toString();
        struc.AlarmTimestamp_ = rec.value(3).toInt();

        list.append(struc);
    }

    return true;

}


