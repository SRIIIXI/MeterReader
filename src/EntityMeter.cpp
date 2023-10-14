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
                        last_sync Integer NOT NULL, \
                        keys103 Text NOT NULL, \
                        keys104 Text NOT NULL, \
                        keys105 Text NOT NULL, \
                        ble_identifier Text NOT NULL, \
                        system_title Text NOT NULL, \
                        ble_pin Text NOT NULL, \
                        customer_pin Text NOT NULL, \
                        last_rssi Text NOT NULL, \
                        meter_model Text NOT NULL, \
                        meter_status Text NOT NULL, \
                        CONSTRAINT unique_serial_no UNIQUE (serial_no), \
                        CONSTRAINT unique_meter_name UNIQUE (meter_name) \
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

bool EntityMeter::createMeter(const Meter &rec)
{
    QSqlQuery qryresult;
    QString err;

    QString sql = "insert into meter (serial_no, meter_name, last_sync, keys103, keys104, keys105, ble_identifier, \
                    system_title, ble_pin, customer_pin, last_rssi, meter_model, meter_status) values('serial_no__', 'meter_name__', last_sync__, \
                    'keys103__', 'keys104__', 'keys105__', 'ble_identifier__', 'system_title__', 'ble_pin__', 'customer_pin__', 'last_rssi__', \
                    'meter_model__', 'meter_status__')";

    sql = sql.replace("serial_no__", rec.SerialNo_);
    sql = sql.replace("meter_name__", rec.MeterName_);
    sql = sql.replace("last_sync__", QVariant(rec.LastSync_).toString());
    sql = sql.replace("keys103__", rec.Keys103_);
    sql = sql.replace("keys104__", rec.Keys104_);
    sql = sql.replace("keys105__", rec.Keys105_);
    sql = sql.replace("ble_identifier__", rec.BleIdentifier_);
    sql = sql.replace("system_title__", rec.SystemTitle_);
    sql = sql.replace("ble_pin__", rec.BlePIN_);
    sql = sql.replace("customer_pin__", rec.CIUPIN_);

    char temp1[9] = {0};
    sprintf(temp1, "%d", rec.RSSI_);
    sql = sql.replace("last_rssi__", temp1);

    sql = sql.replace("meter_model__", rec.ModelNo_);

    char temp2[9] = {0};
    sprintf(temp2, "%d", rec.Status_);
    sql = sql.replace("meter_status__", temp2);

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

bool EntityMeter::allMeters(QList<Meter> &list, bool includedemo)
{
    QString str = "select serial_no, meter_name, last_sync, keys103, keys104, keys105, ble_identifier, system_title, ble_pin, customer_pin, last_rssi, meter_model, meter_status from meter";
    QList<QSqlRecord> *reclist = getAllRecords(str);

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        Meter struc;

        struc.SerialNo_ = rec.value(0).toString();

        if(includedemo == false)
        {
            if(struc.SerialNo_ == "EDMIDEMO")
            {
                continue;
            }
        }

        struc.MeterName_ = rec.value(1).toString();
        struc.LastSync_ = rec.value(2).toInt();
        struc.Keys103_ = rec.value(3).toString();
        struc.Keys104_ = rec.value(4).toString();
        struc.Keys105_ = rec.value(5).toString();
        struc.BleIdentifier_ = rec.value(6).toString();
        struc.SystemTitle_ = rec.value(7).toString();
        struc.BlePIN_ = rec.value(8).toString();
        struc.CIUPIN_ = rec.value(9).toString();
        struc.RSSIStr_ = rec.value(10).toString();
        struc.ModelNo_ = rec.value(11).toString();
        struc.StatusStr_ = rec.value(12).toString();
        struc.LastSyncStr_ = QDateTime::fromSecsSinceEpoch(struc.LastSync_).toString("dd MMM yy hh:mm");
        struc.Status_ = atoi(struc.StatusStr_.toStdString().c_str());
        struc.RSSI_ = atoi(struc.RSSIStr_.toStdString().c_str());
        struc.IsSelected_ = false;

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
            if(struc.SerialNo_ == "EDMIDEMO")
            {
                continue;
            }
        }

        struc.MeterName_ = rec.value(1).toString();
        struc.LastSync_ = rec.value(2).toInt();
        struc.Keys103_ = rec.value(3).toString();
        struc.Keys104_ = rec.value(4).toString();
        struc.Keys105_ = rec.value(5).toString();
        struc.BleIdentifier_ = rec.value(6).toString();
        struc.SystemTitle_ = rec.value(7).toString();
        struc.BlePIN_ = rec.value(8).toString();
        struc.CIUPIN_ = rec.value(9).toString();
        struc.RSSIStr_ = rec.value(10).toString();
        struc.ModelNo_ = rec.value(11).toString();
        struc.StatusStr_ = rec.value(12).toString();
        struc.LastSyncStr_ = QDateTime::fromSecsSinceEpoch(struc.LastSync_).toString("dd MMM yy hh:mm");
        struc.Status_ = atoi(struc.StatusStr_.toStdString().c_str());
        struc.RSSI_ = atoi(struc.RSSIStr_.toStdString().c_str());
        struc.IsSelected_ = false;

        list.append(struc);
    }

    return true;
}
