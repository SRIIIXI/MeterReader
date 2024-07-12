#include "EntityMeter.hpp"

EntityMeter* meterEntityPtr = nullptr;

EntityMeter::EntityMeter(QSqlDatabase *db, QString tabname) : DataEntity(db, tabname)
{
    meterEntityPtr = this;
}

EntityMeter::~EntityMeter()
{
}

bool EntityMeter::createTable()
{
    QString sqlString = "CREATE TABLE meter ( \
                        serial_no Text NOT NULL PRIMARY KEY, \
                        meter_name Text NOT NULL, \
                        meter_model Text NOT NULL, \
                        system_title Text NOT NULL, \
                        customer_pin Text NOT NULL, \
                        pairing_pin Text NOT NULL, \
                        keys103 Text NOT NULL, \
                        keys104 Text NOT NULL, \
                        keys105 Text NOT NULL, \
                        last_sync Integer NOT NULL, \
                        last_rssi Text NOT NULL, \
                        meter_status Text NOT NULL, \
                        is_selected Integer NOT NULL, \
                        CONSTRAINT unique_serial_no UNIQUE (serial_no), \
                        CONSTRAINT unique_meter_name UNIQUE (meter_name) \
                        )";

    return executeQuery(sqlString);
}

bool EntityMeter::createMeter(const Meter &rec)
{
    QString sqlString = "insert into meter (serial_no, meter_name, meter_model, system_title, customer_pin, pairing_pin, keys103, keys104, keys105, last_sync, last_rssi,  meter_status, is_selected) \
                    values('serial_no__', 'meter_name__', 'meter_model__', 'system_title__', 'customer_pin__', 'pairing_pin__', 'keys103__', 'keys104__', 'keys105__', last_sync__, 'last_rssi__', 'meter_status__', is_selected__)";

    sqlString = sqlString.replace("serial_no__", rec.SerialNo_);
    sqlString = sqlString.replace("meter_name__", rec.MeterName_);
    sqlString = sqlString.replace("meter_model__", rec.ModelNo_);
    sqlString = sqlString.replace("system_title__", rec.SystemTitle_);
    sqlString = sqlString.replace("customer_pin__", rec.CustomerPIN_);
    sqlString = sqlString.replace("pairing_pin__", rec.PairingPIN_);
    sqlString = sqlString.replace("keys103__", rec.Keys103_);
    sqlString = sqlString.replace("keys104__", rec.Keys104_);
    sqlString = sqlString.replace("keys105__", rec.Keys105_);
    sqlString = sqlString.replace("last_sync__", QVariant(rec.LastSync_).toString());
    sqlString = sqlString.replace("last_rssi__", rec.RSSIStr_);
    sqlString = sqlString.replace("meter_status__", rec.StatusStr_);
    sqlString = sqlString.replace("is_selected__", QVariant(rec.IsSelected_).toString());

    return executeQuery(sqlString);
}

bool EntityMeter::allMeters(QList<Meter> &list, bool includedemo)
{
    QString str = "select * from meter";
    QList<QSqlRecord> *reclist = getAllRecords(str);

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        Meter struc;

        struc.SerialNo_ = rec.value(0).toString();

        if(includedemo == false)
        {
            if(struc.SerialNo_ == "EDM1DEMO" || struc.SerialNo_ == "EDM2DEMO")
            {
                continue;
            }
        }

        struc.MeterName_ = rec.value(1).toString();
        struc.ModelNo_ = rec.value(2).toString();
        struc.SystemTitle_ = rec.value(3).toString();
        struc.CustomerPIN_ = rec.value(4).toString();
        struc.PairingPIN_ = rec.value(5).toString();
        struc.Keys103_ = rec.value(6).toString();
        struc.Keys104_ = rec.value(7).toString();
        struc.Keys105_ = rec.value(8).toString();
        struc.LastSync_ = rec.value(9).toInt();
        struc.RSSIStr_ = rec.value(10).toString();
        struc.StatusStr_ = rec.value(11).toString();
        struc.IsSelected_ = rec.value(12).toInt();
        struc.LastSyncStr_ = QDateTime::fromSecsSinceEpoch(struc.LastSync_).toString("dd MMM yy hh:mm");

        list.append(struc);
    }

    return true;
}

bool EntityMeter::selectedMeters(QList<Meter> &list, QString keyname, QVariant value, bool includedemo)
{
    QString str;
    QList<QSqlRecord> *reclist = getSelectedRecords(keyname, value, str);

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        Meter struc;

        struc.SerialNo_ = rec.value(0).toString();

        if(includedemo == false)
        {
            if(struc.SerialNo_ == "EDM1DEMO" || struc.SerialNo_ == "EDM2DEMO")
            {
                continue;
            }
        }

        struc.MeterName_ = rec.value(1).toString();
        struc.ModelNo_ = rec.value(2).toString();
        struc.SystemTitle_ = rec.value(3).toString();
        struc.CustomerPIN_ = rec.value(4).toString();
        struc.PairingPIN_ = rec.value(5).toString();
        struc.Keys103_ = rec.value(6).toString();
        struc.Keys104_ = rec.value(7).toString();
        struc.Keys105_ = rec.value(8).toString();
        struc.LastSync_ = rec.value(9).toInt();
        struc.RSSIStr_ = rec.value(10).toString();
        struc.StatusStr_ = rec.value(11).toString();
        struc.IsSelected_ = rec.value(12).toInt();
        struc.LastSyncStr_ = QDateTime::fromSecsSinceEpoch(struc.LastSync_).toString("dd MMM yy hh:mm");

        list.append(struc);
    }

    return true;
}
