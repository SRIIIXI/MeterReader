#include "EntityTariff.hpp"

EntityTariff* tariffEntityPtr = nullptr;

EntityTariff::EntityTariff(QSqlDatabase *db, QString tabname) : DataEntity(db, tabname)
{
    tariffEntityPtr = this;
}

EntityTariff::~EntityTariff()
{
}

bool EntityTariff::createTable()
{
    QString sqlString = "CREATE TABLE tariff( \
                        tariff_id Text NOT NULL PRIMARY KEY, \
                        serial_no Text NOT NULL, \
                        effective_date Integer NOT NULL, \
                        applied Integer NOT NULL, \
                        import1_value Real NOT NULL, \
                        export1_value Real NOT NULL, \
                        import2_value Real NOT NULL, \
                        export2_value Real NOT NULL, \
                        CONSTRAINT unique_tariff_id UNIQUE ( tariff_id ) )";

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

bool EntityTariff::createTariff(const Tariff &rec)
{
    QSqlQuery qryresult;
    QString err;

    QString sql;

    sql = "insert into tariff (tariff_id, serial_no, effective_date, applied, import1_value, export1_value, import2_value, export2_value) values('tariff_id__', 'serial_no__', effective_date__, applied__, import1_value__, export1_value__, import2_value__, export2_value__)";

    sql = sql.replace("tariff_id__", rec.TariffId_);
    sql = sql.replace("effective_date__", QVariant(rec.EffectiveDate_).toString());
    sql = sql.replace("applied__", QVariant(rec.IsApplied_).toString());
    sql = sql.replace("import1_value__", getFloatString(rec.Import1_));
    sql = sql.replace("export1_value__", getFloatString(rec.Export1_));
    sql = sql.replace("import2_value__", getFloatString(rec.Import2_));
    sql = sql.replace("export2_value__", getFloatString(rec.Export2_));
    sql = sql.replace("serial_no__", rec.SerialNo_);


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

bool EntityTariff::allTariffs(QList<Tariff> &list)
{
    QString str = "select tariff_id, effective_date, applied, import1_value, export1_value, import2_value, export2_value from tariff order by effective_date asc";

    QList<QSqlRecord> *reclist = getCustomRecords(str);

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        Tariff struc;

        struc.TariffId_ = rec.value(0).toString();
        struc.EffectiveDate_ = rec.value(1).toInt();
        struc.IsApplied_ = rec.value(2).toInt();
        struc.Import1_ = rec.value(3).toReal();
        struc.Export1_ = rec.value(4).toReal();
        struc.Import2_ = rec.value(5).toReal();
        struc.Export2_ = rec.value(6).toReal();
        struc.EffectiveDateStr_ = QDateTime::fromSecsSinceEpoch(struc.EffectiveDate_).toString("dd MMM yy");

        list.append(struc);
    }

    return true;
}

bool EntityTariff::selectedTariffs(QList<Tariff> &list, QString keyname, QVariant value)
{
    QList<QSqlRecord> *reclist = nullptr;

    QString str = "select tariff_id, effective_date, applied, import1_value, export1_value, import2_value, export2_value from tariff where _KEY_NAME_ = '_VALUE_'  order by effective_date asc";

    str = str.replace("_KEY_NAME_", keyname);
    str = str.replace("_VALUE_", value.toString());
    reclist= getCustomRecords(str);

    if(reclist)
    {
        for(int ctr = 0; ctr < reclist->count(); ctr++)
        {
            QSqlRecord rec = reclist->at(ctr);

            Tariff struc;

            struc.TariffId_ = rec.value(0).toString();
            struc.EffectiveDate_ = rec.value(1).toInt();
            struc.IsApplied_ = rec.value(2).toInt();
            struc.Import1_ = rec.value(3).toReal();
            struc.Export1_ = rec.value(4).toReal();
            struc.Import2_ = rec.value(5).toReal();
            struc.Export2_ = rec.value(6).toReal();
            struc.EffectiveDateStr_ = QDateTime::fromSecsSinceEpoch(struc.EffectiveDate_).toString("dd MMM yy");

            list.append(struc);
        }
    }

    return true;
}
