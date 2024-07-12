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

    return executeQuery(sqlString);
}

bool EntityMeterSettings::createMeterSettings(const MeterSettings &rec)
{
    QString sqlString = "insert into meter_settings (serial_no, key, value) values('serial_no__', key__, 'value__')";

    sqlString = sqlString.replace("serial_no__", rec.SerialNo_);
    sqlString = sqlString.replace("key__", QVariant(rec.Key_).toString());
    sqlString = sqlString.replace("value__", rec.Value_);

    return executeQuery(sqlString);
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
