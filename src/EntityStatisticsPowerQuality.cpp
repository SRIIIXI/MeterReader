#include "EntityStatisticsPowerQuality.hpp"

EntityStatisticsPowerQuality* statisticsPowerQualityEntityPtr = nullptr;

EntityStatisticsPowerQuality::EntityStatisticsPowerQuality(QSqlDatabase *db, QString tabname) : DataEntity(db, tabname)
{
    statisticsPowerQualityEntityPtr = this;
}

EntityStatisticsPowerQuality::~EntityStatisticsPowerQuality()
{
}

bool EntityStatisticsPowerQuality::createTable()
{
    QString sqlString = "CREATE TABLE statistics_power_quality( \
                        serial_no Text NOT NULL, \
                        average_voltage_a Real NOT NULL, \
                        average_voltage_b Real NOT NULL, \
                        average_voltage_c Real NOT NULL, \
                        average_current_a Real NOT NULL, \
                        average_current_b Real NOT NULL, \
                        average_current_c Real NOT NULL, \
                        average_power_factor_total Real NOT NULL, \
                        average_mcu_temperature Real NOT NULL, \
                        instantenous_net_frequency Real NOT NULL, \
                        average_thd_voltage_a Real NOT NULL, \
                        average_thd_voltage_b Real NOT NULL, \
                        average_thd_voltage_c Real NOT NULL, \
                        record_timestamp Integer NOT NULL, \
                        row_index NOT NULL)";

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

bool EntityStatisticsPowerQuality::createStatisticsPowerQuality(const StatisticsPowerQuality &rec)
{
    QSqlQuery qryresult;
    QString err;

    QString sql = "insert into statistics_power_quality (serial_no, average_voltage_a, average_voltage_b, average_voltage_c, average_current_a, average_current_b, average_current_c, average_power_factor_total, average_mcu_temperature, instantenous_net_frequency, average_thd_voltage_a, average_thd_voltage_b, average_thd_voltage_c, record_timestamp, row_index) \
                   values('serial_no__', average_voltage_a__, average_voltage_b__, average_voltage_c__, average_current_a__, average_current_b__, average_current_c__, average_power_factor_total__, average_mcu_temperature__, instantenous_net_frequency__, average_thd_voltage_a__, average_thd_voltage_b__, average_thd_voltage_c__, record_timestamp__, row_index__)";

    sql = sql.replace("serial_no__", QVariant(rec.SerialNo_).toString());
    sql = sql.replace("average_voltage_a__", getFloatString(rec.AverageVoltageA_));
    sql = sql.replace("average_voltage_b__", getFloatString(rec.AverageVoltageB_));
    sql = sql.replace("average_voltage_c__", getFloatString(rec.AverageVoltageC_));
    sql = sql.replace("average_current_a__", getFloatString(rec.AverageCurrentA_));
    sql = sql.replace("average_current_b__", getFloatString(rec.AverageCurrentB_));
    sql = sql.replace("average_current_c__", getFloatString(rec.AverageCurrentC_));
    sql = sql.replace("average_power_factor_total__", getFloatString(rec.AveragePowerFactorTotal_));
    sql = sql.replace("average_mcu_temperature__", getFloatString(rec.AverageMCUTemperature_));
    sql = sql.replace("instantenous_net_frequency__", getFloatString(rec.InstantenousNetFrequency_));
    sql = sql.replace("average_thd_voltage_a__", getFloatString(rec.AverageTHDVoltageA_));
    sql = sql.replace("average_thd_voltage_b__", getFloatString(rec.AverageTHDVoltageB_));
    sql = sql.replace("average_thd_voltage_c__", getFloatString(rec.AverageTHDVoltageC_));
    sql = sql.replace("record_timestamp__", QVariant(rec.RecordTimestamp_).toString());
    sql = sql.replace("row_index__", QVariant(rec.RowIndex_).toString());

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

bool EntityStatisticsPowerQuality::allStatisticsPowerQualitys(QList<StatisticsPowerQuality> &list)
{
    QString str;
    QList<QSqlRecord> *reclist = getAllRecords(str);

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        StatisticsPowerQuality struc;

        struc.SerialNo_ = rec.value(0).toString();
        struc.AverageVoltageA_ = rec.value(1).toReal();
        struc.AverageVoltageB_ = rec.value(2).toReal();
        struc.AverageVoltageC_ = rec.value(3).toReal();
        struc.AverageCurrentA_ = rec.value(4).toReal();
        struc.AverageCurrentB_ = rec.value(5).toReal();
        struc.AverageCurrentC_ = rec.value(6).toReal();
        struc.AveragePowerFactorTotal_ = rec.value(7).toReal();
        struc.AverageMCUTemperature_ = rec.value(8).toReal();
        struc.InstantenousNetFrequency_ = rec.value(9).toReal();
        struc.AverageTHDVoltageA_ = rec.value(10).toReal();
        struc.AverageTHDVoltageB_ = rec.value(11).toReal();
        struc.AverageTHDVoltageC_ = rec.value(12).toReal();
        struc.RecordTimestamp_ = rec.value(13).toInt();
        struc.RowIndex_ = rec.value(14).toInt();

        list.append(struc);
    }

    return true;
}

bool EntityStatisticsPowerQuality::selectedStatisticsPowerQualitys(QList<StatisticsPowerQuality> &list, QString keyname, QVariant value)
{
    QString str;
    QList<QSqlRecord> *reclist = getSelectedRecords(keyname, value, str);

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        StatisticsPowerQuality struc;

        struc.SerialNo_ = rec.value(0).toString();
        struc.AverageVoltageA_ = rec.value(1).toReal();
        struc.AverageVoltageB_ = rec.value(2).toReal();
        struc.AverageVoltageC_ = rec.value(3).toReal();
        struc.AverageCurrentA_ = rec.value(4).toReal();
        struc.AverageCurrentB_ = rec.value(5).toReal();
        struc.AverageCurrentC_ = rec.value(6).toReal();
        struc.AveragePowerFactorTotal_ = rec.value(7).toReal();
        struc.AverageMCUTemperature_ = rec.value(8).toReal();
        struc.InstantenousNetFrequency_ = rec.value(9).toReal();
        struc.AverageTHDVoltageA_ = rec.value(10).toReal();
        struc.AverageTHDVoltageB_ = rec.value(11).toReal();
        struc.AverageTHDVoltageC_ = rec.value(12).toReal();
        struc.RecordTimestamp_ = rec.value(13).toInt();
        struc.RowIndex_ = rec.value(14).toInt();

        list.append(struc);
    }

    return true;
}

bool EntityStatisticsPowerQuality::selectedStatisticsPQsByWeek(QStandardItemModel &xylist, QList<float> &datalist, QList<QString> &tslist, QString serial_no, QString keyname, QString date)
{
    xylist.clear();
    datalist.clear();
    tslist.clear();

    QDate dt = QDate::fromString(date, "dd/MM/yyyy");
    date = dt.toString("yyyy-MM-dd");
    int weekno = dt.weekNumber();

    char year[5] = {0};
    char week[5] = {0};
    memcpy(year, date.toStdString().c_str(), 4);
    sprintf(week, "%02d", weekno);

    QString str = "SELECT _key_, strftime('%w', datetime(record_timestamp, 'unixepoch')) FROM statistics_power_quality where serial_no = '_serial_no_' AND strftime('%Y', datetime(record_timestamp, 'unixepoch')) = '_year_' and strftime('%W', datetime(record_timestamp, 'unixepoch')) = '_week_';";

    str = str.replace("_key_", keyname);
    str = str.replace("_year_", year);
    str = str.replace("_serial_no_", serial_no);
    str = str.replace("_week_", week);

    qDebug() << str;

    QList<QSqlRecord> *reclist = nullptr;
    reclist= getCustomRecords(str);

    if(!reclist)
    {
        return false;
    }

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        datalist.append(rec.value(0).toReal());
        tslist.append(getDayofWeek(rec.value(1).toString()));

        QStandardItem *xItem1 = new QStandardItem;
        QStandardItem *yItem1 = new QStandardItem;
        xItem1->setData(xylist.rowCount(), Qt::DisplayRole);
        yItem1->setData(rec.value(0).toReal(), Qt::DisplayRole);
        xylist.appendRow({xItem1, yItem1});
    }

    return true;
}

bool EntityStatisticsPowerQuality::selectedStatisticsPQsByMonth(QStandardItemModel &xylist, QList<float> &datalist, QList<QString> &tslist, QString serial_no, QString keyname, QString date)
{
    xylist.clear();
    datalist.clear();
    tslist.clear();

    QDate dt = QDate::fromString(date, "dd/MM/yyyy");
    date = dt.toString("yyyy-MM-dd");
    int monthno = dt.month();

    char year[5] = {0};
    char month[5] = {0};
    memcpy(year, date.toStdString().c_str(), 4);
    sprintf(month, "%02d", monthno);

    QString str = "SELECT _key_, strftime('%d', datetime(record_timestamp, 'unixepoch')) FROM statistics_power_quality where serial_no = '_serial_no_' AND strftime('%Y', datetime(record_timestamp, 'unixepoch')) = '_year_' and strftime('%m', datetime(record_timestamp, 'unixepoch')) = '_month_';";

    str = str.replace("_key_", keyname);
    str = str.replace("_year_", year);
    str = str.replace("_serial_no_", serial_no);
    str = str.replace("_month_", month);

    qDebug() << str;

    QList<QSqlRecord> *reclist = nullptr;
    reclist= getCustomRecords(str);

    if(!reclist)
    {
        return false;
    }

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        QString mt = rec.value(1).toString();

        if(mt == "02" || mt == "03" || mt == "05" || mt == "06" || mt == "08" || mt == "09" || mt == "11" || mt == "12" || mt == "14" || mt == "15" || mt == "17" || mt == "18" || mt == "20" || mt == "21" || mt == "23" || mt == "24" || mt == "26" || mt == "27" || mt == "29" || mt == "30")
        {
            continue;
        }

        datalist.append(rec.value(0).toReal());
        tslist.append(rec.value(1).toString());

        QStandardItem *xItem1 = new QStandardItem;
        QStandardItem *yItem1 = new QStandardItem;
        xItem1->setData(xylist.rowCount(), Qt::DisplayRole);
        yItem1->setData(rec.value(0).toReal(), Qt::DisplayRole);
        xylist.appendRow({xItem1, yItem1});
    }

    return true;
}

bool EntityStatisticsPowerQuality::selectedStatisticsPQsByYear(QStandardItemModel &xylist, QList<float> &datalist, QList<QString> &tslist, QString serial_no, QString keyname, QString date)
{
    xylist.clear();
    datalist.clear();
    tslist.clear();

    normalizeDateString(date);

    char year[5] = {0};
    memcpy(year, date.toStdString().c_str(), 4);

    QString str = "SELECT _key_, strftime('%m', datetime(record_timestamp, 'unixepoch' )) FROM statistics_power_quality where serial_no = '_serial_no_' AND strftime('%Y', datetime(record_timestamp, 'unixepoch' )) = '_year_' GROUP BY strftime('%m', datetime(record_timestamp, 'unixepoch' ));";

    str = str.replace("_key_", keyname);
    str = str.replace("_year_", year);
    str = str.replace("_serial_no_", serial_no);

    qDebug() << str;

    QList<QSqlRecord> *reclist = nullptr;
    reclist= getCustomRecords(str);

    if(!reclist)
    {
        return false;
    }

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        datalist.append(rec.value(0).toReal());
        tslist.append(getMonth(rec.value(1).toString()));

        QStandardItem *xItem1 = new QStandardItem;
        QStandardItem *yItem1 = new QStandardItem;
        xItem1->setData(xylist.rowCount(), Qt::DisplayRole);
        yItem1->setData(rec.value(0).toReal(), Qt::DisplayRole);
        xylist.appendRow({xItem1, yItem1});
    }

    return true;
}

bool EntityStatisticsPowerQuality::selectedStatisticsPQsByDay(QStandardItemModel &xylist, QList<float> &datalist, QList<QString> &tslist, QString serial_no, QString keyname, QString date)
{
    normalizeDateString(date);
    QString str = "SELECT _key_, strftime('%H', datetime(record_timestamp, 'unixepoch' )) from statistics_power_quality where date(record_timestamp, 'unixepoch' ) = '_date_' and serial_no = '_serial_no_';";

    str = str.replace("_key_", keyname);
    str = str.replace("_date_", date);
    str = str.replace("_serial_no_", serial_no);

    qDebug() << str;

    QList<QSqlRecord> *reclist = nullptr;
    reclist= getCustomRecords(str);

    if(!reclist)
    {
        return false;
    }

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        datalist.append(rec.value(0).toReal());
        tslist.append(rec.value(1).toString());
    }

    return true;
}


bool EntityStatisticsPowerQuality::getRanges(QString serial_no, uint32_t &startRowIndex, uint32_t &endRowIndex, uint32_t &startTimestamp, uint32_t &endTimestamp)
{
    QList<QSqlRecord> *reclist = nullptr;
    QString sqlstart = "SELECT row_index, record_timestamp FROM statistics_energy where serial_no = '_serial_no_' ORDER BY row_index ASC LIMIT 1";
    QString sqlend = "SELECT row_index, record_timestamp FROM statistics_energy where serial_no = '_serial_no_' ORDER BY row_index DESC LIMIT 1";

    sqlstart = sqlstart.replace("_serial_no_", serial_no);
    sqlend = sqlend.replace("_serial_no_", serial_no);

    reclist= getCustomRecords(sqlstart);

    if(!reclist)
    {
        startRowIndex = 0;
        startTimestamp = 0;
    }
    else
    {
        if(reclist->count() < 1)
        {
            startRowIndex = 0;
            startTimestamp = 0;
        }
        else
        {
            startRowIndex = reclist->at(0).value(0).toUInt();
            startTimestamp = reclist->at(0).value(1).toUInt();
        }
    }

    reclist= getCustomRecords(sqlend);

    if(!reclist)
    {
        endRowIndex = 0;
        endTimestamp = 0;
    }
    else
    {
        if(reclist->count() < 1)
        {
            endRowIndex = 0;
            endTimestamp = 0;
        }
        else
        {
            endRowIndex = reclist->at(0).value(0).toUInt();
            endTimestamp = reclist->at(0).value(1).toUInt();
        }
    }

    return true;
}

bool EntityStatisticsPowerQuality::getStartIndex(QString serial_no, uint32_t &startRowIndex, uint32_t &startTimestamp, uint32_t currentTimestamp)
{
    QList<QSqlRecord> *reclist = nullptr;
    QString sql = "SELECT row_index, record_timestamp FROM statistics_energy where record_timestamp < TIME_STAMP__ and serial_no = '_serial_no_' ORDER BY record_timestamp DESC LIMIT 1";

    sql = sql.replace("TIME_STAMP__", QVariant(currentTimestamp).toString());
    sql = sql.replace("_serial_no_", serial_no);

    reclist= getCustomRecords(sql);

    if(!reclist)
    {
        startRowIndex = 0;
        startTimestamp = 0;
    }
    else
    {
        if(reclist->count() < 1)
        {
            startRowIndex = 0;
            startTimestamp = 0;
        }
        else
        {
            startRowIndex = reclist->at(0).value(0).toUInt();
            startTimestamp = reclist->at(0).value(1).toUInt();
        }
    }

    return true;
}

