#include "EntityStatisticsEnergy.hpp"

EntityStatisticsEnergy* statisticsEnergyEntityPtr = nullptr;

QMap<QString, QPair<float, float>> energyDataRowbuffer;

EntityStatisticsEnergy::EntityStatisticsEnergy(QSqlDatabase *db, QString tabname) : DataEntity(db, tabname)
{
    statisticsEnergyEntityPtr = this;
}

EntityStatisticsEnergy::~EntityStatisticsEnergy()
{
}

bool EntityStatisticsEnergy::createTable()
{
    QString sqlString = "CREATE TABLE statistics_energy( \
                        serial_no Text NOT NULL, \
                        active_import_tariff1 Real NOT NULL, \
                        active_export_tariff1 Real NOT NULL, \
                        reactive_import_tariff1 Real NOT NULL, \
                        reactive_export_tariff1 Real NOT NULL, \
                        active_import_tariff2 Real NOT NULL, \
                        active_export_tariff2 Real NOT NULL, \
                        reactive_import_tariff2 Real NOT NULL, \
                        reactive_export_tariff2 Real NOT NULL, \
                        lp_type Integer NOT NULL, \
                        record_timestamp Integer NOT NULL, \
                        row_index NOT NULL, \
                        PRIMARY KEY (serial_no, record_timestamp, lp_type) \
                        )";

    return executeQuery(sqlString);
}

bool EntityStatisticsEnergy::createStatisticsEnergy(const StatisticsEnergy &rec)
{
    QString sqlString = "insert into statistics_energy (serial_no, active_import_tariff1, active_export_tariff1, reactive_import_tariff1, reactive_export_tariff1, active_import_tariff2, active_export_tariff2, reactive_import_tariff2, reactive_export_tariff2, lp_type, record_timestamp, row_index) values('serial_no__', active_import_tariff1__, active_export_tariff1__, reactive_import_tariff1__, reactive_export_tariff1__, active_import_tariff2__, active_export_tariff2__, reactive_import_tariff2__, reactive_export_tariff2__, lp_type__, record_timestamp__, row_index__)";

    sqlString = sqlString.replace("serial_no__", QVariant(rec.SerialNo_).toString());
    sqlString = sqlString.replace("reactive_import_tariff1__", getFloatString(rec.ReactiveImportTariff1_));
    sqlString = sqlString.replace("reactive_export_tariff1__", getFloatString(rec.ReactiveExportTariff1_));
    sqlString = sqlString.replace("reactive_import_tariff2__", getFloatString(rec.ReactiveImportTariff2_));
    sqlString = sqlString.replace("reactive_export_tariff2__", getFloatString(rec.ReactiveExportTariff2_));
    sqlString = sqlString.replace("active_import_tariff1__", getFloatString(rec.ActiveImportTariff1_));
    sqlString = sqlString.replace("active_export_tariff1__", getFloatString(rec.ActiveExportTariff1_));
    sqlString = sqlString.replace("active_import_tariff2__", getFloatString(rec.ActiveImportTariff2_));
    sqlString = sqlString.replace("active_export_tariff2__", getFloatString(rec.ActiveExportTariff2_));
    sqlString = sqlString.replace("lp_type__", QVariant(rec.LoadProfileType_).toString());
    sqlString = sqlString.replace("record_timestamp__", QVariant(rec.RecordTimestamp_).toString());
    sqlString = sqlString.replace("row_index__", QVariant(rec.RowIndex_).toString());

    return executeQuery(sqlString);
}

bool EntityStatisticsEnergy::allStatisticsEnergys(QList<StatisticsEnergy> &list, QString serial_no)
{
    QString str;
    QList<QSqlRecord> *reclist = getAllRecords(str);

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        StatisticsEnergy struc;

        struc.SerialNo_ = rec.value(0).toString();
        struc.ActiveImportTariff1_ = rec.value(1).toReal();
        struc.ActiveExportTariff1_ = rec.value(2).toReal();
        struc.ReactiveImportTariff1_ = rec.value(3).toReal();
        struc.ReactiveExportTariff1_ = rec.value(4).toReal();
        struc.ActiveImportTariff2_ = rec.value(5).toReal();
        struc.ActiveExportTariff2_ = rec.value(6).toReal();
        struc.ReactiveImportTariff2_ = rec.value(7).toReal();
        struc.ReactiveExportTariff2_ = rec.value(8).toReal();
        struc.LoadProfileType_ = rec.value(9).toInt();
        struc.RecordTimestamp_ = rec.value(10).toInt();
        struc.RowIndex_ = rec.value(11).toInt();

        list.append(struc);
    }

    return true;
}

bool EntityStatisticsEnergy::selectedStatisticsEnergys(QList<StatisticsEnergy> &list, QString serial_no, QString keyname, QVariant value)
{
    QString str;
    QList<QSqlRecord> *reclist = getSelectedRecords(keyname, value, str);

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        StatisticsEnergy struc;

        struc.SerialNo_ = rec.value(0).toString();
        struc.ActiveImportTariff1_ = rec.value(1).toReal();
        struc.ActiveExportTariff1_ = rec.value(2).toReal();
        struc.ReactiveImportTariff1_ = rec.value(3).toReal();
        struc.ReactiveExportTariff1_ = rec.value(4).toReal();
        struc.ActiveImportTariff2_ = rec.value(5).toReal();
        struc.ActiveExportTariff2_ = rec.value(6).toReal();
        struc.ReactiveImportTariff2_ = rec.value(7).toReal();
        struc.ReactiveExportTariff2_ = rec.value(8).toReal();
        struc.LoadProfileType_ = rec.value(9).toInt();
        struc.RecordTimestamp_ = rec.value(10).toInt();
        struc.RowIndex_ = rec.value(11).toInt();

        list.append(struc);
    }

    return true;
}

bool EntityStatisticsEnergy::selectedStatisticsEnergysByWeek(QList<BarData> &barlist, QList<LineData> &linelist, QString serial_no, QString keyname, uint32_t datetimestart, uint32_t datetimeend, float &max, float &total, float &average)
{
    QDateTime dt;
    int weekno = 1;
    char year[5] = {0};
    char week[5] = {0};
    QString str;
    QString datestr;
    QList<QSqlRecord> *reclist = nullptr;
    int ctr = 0;
    float summary = 0;

    barlist.clear();
    linelist.clear();
    energyDataRowbuffer.clear();

    energyDataRowbuffer["0"] = QPair<float, float>(0,0);
    energyDataRowbuffer["1"] = QPair<float, float>(0,0);
    energyDataRowbuffer["2"] = QPair<float, float>(0,0);
    energyDataRowbuffer["3"] = QPair<float, float>(0,0);
    energyDataRowbuffer["4"] = QPair<float, float>(0,0);
    energyDataRowbuffer["5"] = QPair<float, float>(0,0);
    energyDataRowbuffer["6"] = QPair<float, float>(0,0);

    dt = QDateTime::fromSecsSinceEpoch(datetimestart);
    weekno = dt.date().weekNumber();
    memset(year, 0, sizeof(year));
    memset(week, 0, sizeof(week));
    normalizeDateString(datetimestart, datestr);
    memcpy(year, datestr.toStdString().c_str(), 4);
    snprintf(week, 4, "%02d", weekno);

    str = "SELECT strftime('%w', datetime(record_timestamp, 'unixepoch')), _key_ FROM statistics_energy where serial_no = '_serial_no_' AND strftime('%Y', datetime(record_timestamp, 'unixepoch')) = '_year_' and strftime('%W', datetime(record_timestamp, 'unixepoch')) = '_week_' GROUP BY strftime('%w', datetime(record_timestamp, 'unixepoch'));";

    str = str.replace("_key_", keyname);
    str = str.replace("_year_", year);
    str = str.replace("_serial_no_", serial_no);
    str = str.replace("_week_", week);

    reclist= getCustomRecords(str);

    if(!reclist)
    {
        return false;
    }

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);
        QString ts = rec.value(0).toString();
        float val = rec.value(1).toReal();

        if(energyDataRowbuffer.contains(ts))
        {
            energyDataRowbuffer[ts].first = (val + energyDataRowbuffer[ts].first)/2;
        }
    }

    dt = QDateTime::fromSecsSinceEpoch(datetimeend);
    weekno = dt.date().weekNumber();
    memset(year, 0, sizeof(year));
    memset(week, 0, sizeof(week));
    normalizeDateString(datetimeend, datestr);
    memcpy(year, datestr.toStdString().c_str(), 4);
    snprintf(week, 4, "%02d", weekno);

    str = "SELECT strftime('%w', datetime(record_timestamp, 'unixepoch')), _key_ FROM statistics_energy where serial_no = '_serial_no_' AND strftime('%Y', datetime(record_timestamp, 'unixepoch')) = '_year_' and strftime('%W', datetime(record_timestamp, 'unixepoch')) = '_week_' GROUP BY strftime('%w', datetime(record_timestamp, 'unixepoch'));";

    str = str.replace("_key_", keyname);
    str = str.replace("_year_", year);
    str = str.replace("_serial_no_", serial_no);
    str = str.replace("_week_", week);

    reclist= getCustomRecords(str);

    if(!reclist)
    {
        return false;
    }

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);
        QString ts = rec.value(0).toString();
        float val = rec.value(1).toReal();

        if(energyDataRowbuffer.contains(ts))
        {
            energyDataRowbuffer[ts].second = (val + energyDataRowbuffer[ts].second)/2;
        }
    }

    QMapIterator<QString, QPair<float, float>> i(energyDataRowbuffer);

    while (i.hasNext())
    {
        i.next();

        ctr++;
        summary = summary + i.value().first + i.value().second;

        if(i.value().first > max)
        {
            max = i.value().first;
        }

        if(i.value().second > max)
        {
            max = i.value().second;
        }

        QString dowstr = getDayofWeek(i.key());
        barlist.append(BarData(dowstr, i.value().first, i.value().second));
        linelist.append(LineData(i.key().toInt()+1, i.value().first, i.key().toInt()+1, i.value().second));
    }

    total = summary;
    average = summary/(ctr*2*7);
    return true;
}

bool EntityStatisticsEnergy::selectedStatisticsEnergysByMonth(QList<BarData> &barlist, QList<LineData> &linelist, QString serial_no, QString keyname, uint32_t datetimestart, uint32_t datetimeend, float &max, float &total, float &average)
{
    QDateTime dt;
    int weekno = 1;
    char year[5] = {0};
    char month[5] = {0};
    int monthno;
    QString str;
    QString datestr;
    QList<QSqlRecord> *reclist = nullptr; 
    int ctr = 0;
    float summary = 0;

    barlist.clear();
    linelist.clear();
    energyDataRowbuffer.clear();

    energyDataRowbuffer["01"] = QPair<float, float>(0,0);
    energyDataRowbuffer["04"] = QPair<float, float>(0,0);
    energyDataRowbuffer["07"] = QPair<float, float>(0,0);
    energyDataRowbuffer["10"] = QPair<float, float>(0,0);
    energyDataRowbuffer["13"] = QPair<float, float>(0,0);
    energyDataRowbuffer["16"] = QPair<float, float>(0,0);
    energyDataRowbuffer["19"] = QPair<float, float>(0,0);
    energyDataRowbuffer["22"] = QPair<float, float>(0,0);
    energyDataRowbuffer["09"] = QPair<float, float>(0,0);
    energyDataRowbuffer["25"] = QPair<float, float>(0,0);
    energyDataRowbuffer["28"] = QPair<float, float>(0,0);
    energyDataRowbuffer["30"] = QPair<float, float>(0,0);

    dt = QDateTime::fromSecsSinceEpoch(datetimestart);
    monthno = dt.date().month();
    memset(year, 0, sizeof(year));
    normalizeDateString(datetimestart, datestr);
    memcpy(year, datestr.toStdString().c_str(), 4);
    snprintf(month, 4, "%02d", monthno);

    str = "SELECT strftime('%d', datetime(record_timestamp, 'unixepoch')), _key_ FROM statistics_energy where serial_no = '_serial_no_' AND strftime('%Y', datetime(record_timestamp, 'unixepoch')) = '_year_' and strftime('%m', datetime(record_timestamp, 'unixepoch')) = '_month_' GROUP BY strftime('%d', datetime(record_timestamp, 'unixepoch'))";

    str = str.replace("_key_", keyname);
    str = str.replace("_year_", year);
    str = str.replace("_serial_no_", serial_no);
    str = str.replace("_month_", month);

    reclist= getCustomRecords(str);

    if(!reclist)
    {
        return false;
    }

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);
        QString mt = rec.value(0).toString();
        float val = rec.value(1).toReal();

        if(mt == "02" || mt == "03" || mt == "05" || mt == "06" || mt == "08" || mt == "11" || mt == "12" || mt == "14" || mt == "15" || mt == "17" || mt == "18" || mt == "20" || mt == "21" || mt == "23" || mt == "24" || mt == "26" || mt == "27" || mt == "29" || mt == "31")
        {
            continue;
        }

        if(energyDataRowbuffer.contains(mt))
        {
            energyDataRowbuffer[mt].first = (val + energyDataRowbuffer[mt].first)/2;
        }
    }

    dt = QDateTime::fromSecsSinceEpoch(datetimeend);
    monthno = dt.date().month();
    memset(year, 0, sizeof(year));
    normalizeDateString(datetimeend, datestr);
    memcpy(year, datestr.toStdString().c_str(), 4);
    snprintf(month, 4, "%02d", monthno);

    str = "SELECT strftime('%d', datetime(record_timestamp, 'unixepoch')), _key_ FROM statistics_energy where serial_no = '_serial_no_' AND strftime('%Y', datetime(record_timestamp, 'unixepoch')) = '_year_' and strftime('%m', datetime(record_timestamp, 'unixepoch')) = '_month_' GROUP BY strftime('%d', datetime(record_timestamp, 'unixepoch'))";

    str = str.replace("_key_", keyname);
    str = str.replace("_year_", year);
    str = str.replace("_serial_no_", serial_no);
    str = str.replace("_month_", month);

    reclist= getCustomRecords(str);

    if(!reclist)
    {
        return false;
    }

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);
        QString mt = rec.value(0).toString();
        float val = rec.value(1).toReal();

        if(mt == "02" || mt == "03" || mt == "05" || mt == "06" || mt == "08" || mt == "11" || mt == "12" || mt == "14" || mt == "15" || mt == "17" || mt == "18" || mt == "20" || mt == "21" || mt == "23" || mt == "24" || mt == "26" || mt == "27" || mt == "29" || mt == "31")
        {
            continue;
        }

        if(energyDataRowbuffer.contains(mt))
        {
            energyDataRowbuffer[mt].second = (val + energyDataRowbuffer[mt].second)/2;
        }
    }

    QMapIterator<QString, QPair<float, float>> i(energyDataRowbuffer);

    while (i.hasNext())
    {
        i.next();

        ctr++;
        summary = summary + i.value().first + i.value().second;

        if(i.value().first > max)
        {
            max = i.value().first;
        }

        if(i.value().second > max)
        {
            max = i.value().second;
        }

        barlist.append(BarData(i.key(), i.value().first, i.value().second));
        linelist.append(LineData(i.key().toInt(), i.value().first, i.key().toInt(), i.value().second));
    }

    total = summary;
    average = summary/(ctr*2*30);

    return true;
}

bool EntityStatisticsEnergy::selectedStatisticsEnergysByYear(QList<BarData> &barlist, QList<LineData> &linelist, QString serial_no, QString keyname, uint32_t datetimestart, uint32_t datetimeend, float &max, float &total, float &average)
{
    char year[5] = {0};
    QString str;
    QString datestr;
    QList<QSqlRecord> *reclist = nullptr;
    int ctr = 0;
    float summary = 0;

    barlist.clear();
    linelist.clear();
    energyDataRowbuffer.clear();

    energyDataRowbuffer["01"] = QPair<float, float>(0,0);
    energyDataRowbuffer["02"] = QPair<float, float>(0,0);
    energyDataRowbuffer["03"] = QPair<float, float>(0,0);
    energyDataRowbuffer["04"] = QPair<float, float>(0,0);
    energyDataRowbuffer["05"] = QPair<float, float>(0,0);
    energyDataRowbuffer["06"] = QPair<float, float>(0,0);
    energyDataRowbuffer["07"] = QPair<float, float>(0,0);
    energyDataRowbuffer["08"] = QPair<float, float>(0,0);
    energyDataRowbuffer["09"] = QPair<float, float>(0,0);
    energyDataRowbuffer["10"] = QPair<float, float>(0,0);
    energyDataRowbuffer["11"] = QPair<float, float>(0,0);
    energyDataRowbuffer["12"] = QPair<float, float>(0,0);

    normalizeDateString(datetimestart, datestr);
    memset(year, 0 , sizeof(year));
    memcpy(year, datestr.toStdString().c_str(), 4);

    str = "SELECT strftime('%m', datetime(record_timestamp, 'unixepoch' )), _key_ FROM statistics_energy where serial_no = '_serial_no_' AND strftime('%Y', datetime(record_timestamp, 'unixepoch' )) = '_year_' GROUP BY strftime('%m', datetime(record_timestamp, 'unixepoch' ));";

    str = str.replace("_key_", keyname);
    str = str.replace("_year_", year);
    str = str.replace("_serial_no_", serial_no);

    reclist= getCustomRecords(str);

    if(!reclist)
    {
        return false;
    }

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);
        QString ts = rec.value(0).toString();
        float val = rec.value(1).toReal();

        if(energyDataRowbuffer.contains(ts))
        {
            energyDataRowbuffer[ts].first = (val + energyDataRowbuffer[ts].first)/2;
        }
    }

    normalizeDateString(datetimeend, datestr);
    memset(year, 0 , sizeof(year));
    memcpy(year, datestr.toStdString().c_str(), 4);

    str = "SELECT strftime('%m', datetime(record_timestamp, 'unixepoch' )), _key_ FROM statistics_energy where serial_no = '_serial_no_' AND strftime('%Y', datetime(record_timestamp, 'unixepoch' )) = '_year_' GROUP BY strftime('%m', datetime(record_timestamp, 'unixepoch' ));";

    str = str.replace("_key_", keyname);
    str = str.replace("_year_", year);
    str = str.replace("_serial_no_", serial_no);

    reclist= getCustomRecords(str);

    if(!reclist)
    {
        return false;
    }

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);
        QString ts = rec.value(0).toString();
        float val = rec.value(1).toReal();

        if(energyDataRowbuffer.contains(ts))
        {
            energyDataRowbuffer[ts].second = (val + energyDataRowbuffer[ts].second)/2;
        }
    }

    QMapIterator<QString, QPair<float, float>> i(energyDataRowbuffer);

    while (i.hasNext())
    {
        i.next();

        ctr++;
        summary = summary + i.value().first + i.value().second;

        if(i.value().first > max)
        {
            max = i.value().first;
        }

        if(i.value().second > max)
        {
            max = i.value().second;
        }

        QString monthstr = getMonth(i.key());
        barlist.append(BarData(monthstr, i.value().first, i.value().second));
        linelist.append(LineData(i.key().toInt(), i.value().first, i.key().toInt(), i.value().second));
    }

    total = summary;
    average = summary/(ctr*2*365);

    return true;
}

bool EntityStatisticsEnergy::selectedStatisticsEnergysByDay(QList<BarData> &barlist, QList<LineData> &linelist, QString serial_no, QString keyname, uint32_t datetimestart, uint32_t datetimeend, float &max, float &total, float &average)
{
    QString str;
    QString datestr;
    QList<QSqlRecord> *reclist = nullptr;
    int ctr = 0;
    float summary = 0;

    barlist.clear();
    linelist.clear();
    energyDataRowbuffer.clear();

    energyDataRowbuffer["01"] = QPair<float, float>(0,0);
    energyDataRowbuffer["03"] = QPair<float, float>(0,0);
    energyDataRowbuffer["05"] = QPair<float, float>(0,0);
    energyDataRowbuffer["07"] = QPair<float, float>(0,0);
    energyDataRowbuffer["09"] = QPair<float, float>(0,0);
    energyDataRowbuffer["11"] = QPair<float, float>(0,0);
    energyDataRowbuffer["13"] = QPair<float, float>(0,0);
    energyDataRowbuffer["15"] = QPair<float, float>(0,0);
    energyDataRowbuffer["17"] = QPair<float, float>(0,0);
    energyDataRowbuffer["19"] = QPair<float, float>(0,0);
    energyDataRowbuffer["21"] = QPair<float, float>(0,0);
    energyDataRowbuffer["23"] = QPair<float, float>(0,0);

    normalizeDateString(datetimestart, datestr);
    str = "SELECT strftime('%H', datetime(record_timestamp, 'unixepoch' )), _key_ from statistics_energy where date(record_timestamp, 'unixepoch' ) = '_date_' and serial_no = '_serial_no_';";

    str = str.replace("_key_", keyname);
    str = str.replace("_date_", datestr);
    str = str.replace("_serial_no_", serial_no);

    reclist= getCustomRecords(str);

    if(!reclist)
    {
        return false;
    }

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);
        QString ts = rec.value(0).toString();
        float val = rec.value(1).toReal();

        if(energyDataRowbuffer.contains(ts))
        {
            energyDataRowbuffer[ts].first = (val + energyDataRowbuffer[ts].first)/2;
        }
    }

    normalizeDateString(datetimeend, datestr);
    str = "SELECT strftime('%H', datetime(record_timestamp, 'unixepoch' )), _key_ from statistics_energy where date(record_timestamp, 'unixepoch' ) = '_date_' and serial_no = '_serial_no_';";

    str = str.replace("_key_", keyname);
    str = str.replace("_date_", datestr);
    str = str.replace("_serial_no_", serial_no);

    reclist= getCustomRecords(str);

    if(!reclist)
    {
        return false;
    }

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);
        QString ts = rec.value(0).toString();
        float val = rec.value(1).toReal();

        if(energyDataRowbuffer.contains(ts))
        {
            energyDataRowbuffer[ts].second = (val + energyDataRowbuffer[ts].second)/2;
        }
    }

    QMapIterator<QString, QPair<float, float>> i(energyDataRowbuffer);

    while (i.hasNext())
    {
        i.next();

        ctr++;
        summary = summary + i.value().first + i.value().second;

        if(i.value().first > max)
        {
            max = i.value().first;
        }

        if(i.value().second > max)
        {
            max = i.value().second;
        }

        if(i.key().toUInt() % 2 == 1)
        {
            barlist.append(BarData(i.key(), i.value().first, i.value().second));
            linelist.append(LineData(i.key().toInt(), i.value().first, i.key().toInt(), i.value().second));
        }
        else
        {
            continue;
        }
    }

    total = summary;
    average = summary/(ctr*2);

    return true;
}

bool EntityStatisticsEnergy::getIndexRanges(QString serial_no,uint32_t &startRowIndex, uint32_t &endRowIndex, uint32_t lptype)
{
    QList<QSqlRecord> *reclist = nullptr;
    QString sqlstart = "SELECT row_index FROM statistics_energy where lp_type = LP_TYPE__ and serial_no = '_serial_no_' ORDER BY row_index ASC LIMIT 1";
    QString sqlend = "SELECT row_index FROM statistics_energy where lp_type = LP_TYPE__ and serial_no = '_serial_no_' ORDER BY row_index DESC LIMIT 1";

    sqlstart = sqlstart.replace("LP_TYPE__", QVariant(lptype).toString());
    sqlend = sqlend.replace("LP_TYPE__", QVariant(lptype).toString());
    sqlstart = sqlstart.replace("_serial_no_", serial_no);
    sqlend = sqlend.replace("_serial_no_", serial_no);

    reclist= getCustomRecords(sqlstart);

    if(!reclist)
    {
        startRowIndex = 0;
    }
    else
    {
        if(reclist->count() < 1)
        {
            startRowIndex = 0;
        }
        else
        {
            startRowIndex = reclist->at(0).value(0).toUInt();
        }
    }

    reclist= getCustomRecords(sqlend);

    if(!reclist)
    {
        endRowIndex = 0;
    }
    else
    {
        if(reclist->count() < 1)
        {
            endRowIndex = 0;
        }
        else
        {
            endRowIndex = reclist->at(0).value(0).toUInt();
        }
    }

    return true;
}

bool EntityStatisticsEnergy::getTimestampRanges(QString serial_no, uint32_t &startTimestamp, uint32_t &endTimestamp, uint32_t lptype)
{
    QList<QSqlRecord> *reclist = nullptr;
    QString sqlstart = "SELECT record_timestamp FROM statistics_energy where lp_type = LP_TYPE__ and serial_no = '_serial_no_' ORDER BY record_timestamp ASC LIMIT 1";
    QString sqlend = "SELECT record_timestamp FROM statistics_energy where lp_type = LP_TYPE__ and serial_no = '_serial_no_' ORDER BY record_timestamp DESC LIMIT 1";

    sqlstart = sqlstart.replace("LP_TYPE__", QVariant(lptype).toString());
    sqlend = sqlend.replace("LP_TYPE__", QVariant(lptype).toString());
    sqlstart = sqlstart.replace("_serial_no_", serial_no);
    sqlend = sqlend.replace("_serial_no_", serial_no);

    reclist= getCustomRecords(sqlstart);

    if(!reclist)
    {
        startTimestamp = 0;
    }
    else
    {
        if(reclist->count() < 1)
        {
            startTimestamp = 0;
        }
        else
        {
            startTimestamp = reclist->at(0).value(0).toUInt();
        }
    }

    reclist= getCustomRecords(sqlend);

    if(!reclist)
    {
        endTimestamp = 0;
    }
    else
    {
        if(reclist->count() < 1)
        {
            endTimestamp = 0;
        }
        else
        {
            endTimestamp = reclist->at(0).value(0).toUInt();
        }
    }

    return true;
}

bool EntityStatisticsEnergy::getStartIndex(QString serial_no, uint32_t &startRowIndex, uint32_t &startTimestamp, uint32_t currentTimestamp, uint32_t lptype)
{
    QList<QSqlRecord> *reclist = nullptr;
    QString sql = "SELECT row_index, record_timestamp FROM statistics_energy where record_timestamp < TIME_STAMP__ and lp_type = LP_TYPE__ and serial_no = '_serial_no_' ORDER BY record_timestamp DESC LIMIT 1";

    sql = sql.replace("TIME_STAMP__", QVariant(currentTimestamp).toString());
    sql = sql.replace("LP_TYPE__", QVariant(lptype).toString());
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

