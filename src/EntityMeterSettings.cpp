#include "EntityMeterSettings.hpp"

int ACCOUNT_TYPE = 1;
int ACCOUNT_CURRENCY = 2;
int ACCOUNT_CURRENCY_SCALAR = 3;
int ACTIVE_ENERGY_SCALAR = 4;
int INSTANTENOUS_POWER_SCALAR = 5;
int ACTIVE_IMPORT_SCALAR = 6;
int ACTIVE_EXPORT_SCALAR = 7;
int REACTIVE_EXPORT_SCALAR = 8;
int REACTIVE_IMPORT_SCALAR = 9;


EntityMeterSettings* meterSettingsEntityPtr = nullptr;

EntityMeterSettings::EntityMeterSettings(QSqlDatabase *db, QString tabname) : DataEntity(db, tabname)
{
    meterSettingsEntityPtr = this;
}

EntityMeterSettings::~EntityMeterSettings()
{
}

bool EntityMeterSettings::createTable()
{
    QString sqlString = "CREATE TABLE meter_settings ( \
                        serial_no Text NOT NULL, \
                        key Integer NOT NULL, \
                        value Text NOT NULL, \
                        PRIMARY KEY (serial_no, key) \
                        )";

    QSqlQuery qryresult;

    try
    {
        _Database->transaction();

        qryresult = _Database->exec(sqlString);

        _Database->commit();
    }
    catch(QException e)
    {
       _Database->rollback();
       QString errString = _Database->lastError().text();
       return false;
    }

    return true;

}

bool EntityMeterSettings::createMeterSettings(const MeterSettings &rec)
{
    QSqlQuery qryresult;
    QString err;

    QString sql = "insert into meter_settings (serial_no, key, value) values('serial_no__', key__, 'value__')";

    sql = sql.replace("serial_no__", rec.SerialNo_);
    sql = sql.replace("key__", QVariant(rec.Key_).toString());
    sql = sql.replace("value__", rec.Value_);

    try
    {
        _Database->transaction();

        qryresult = _Database->exec(sql);

        _Database->commit();
    }
    catch(QException e)
    {
       _Database->rollback();
       err = _Database->lastError().text();
       return false;
    }

    return true;
}

bool EntityMeterSettings::allMetersSettings(QList<MeterSettings> &list)
{
    QString str = "select serial_no, key, value from meter_settings";
    QList<QSqlRecord> *reclist = getAllRecords(str);

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        MeterSettings struc;

        struc.SerialNo_ = rec.value(0).toString();
        struc.Key_ = rec.value(1).toInt();
        struc.Value_ = rec.value(2).toString();

        list.append(struc);
    }

    return true;
}

bool EntityMeterSettings::selectedMetersSettings(QList<MeterSettings> &list, QString keyname, QVariant value)
{
    QString str;
    QList<QSqlRecord> *reclist = getSelectedRecords(keyname, value, str);

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        MeterSettings struc;

        struc.SerialNo_ = rec.value(0).toString();
        struc.Key_ = rec.value(1).toInt();
        struc.Value_ = rec.value(2).toString();

        list.append(struc);
    }

    return true;
}
