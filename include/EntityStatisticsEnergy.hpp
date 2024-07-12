#ifndef ENTITY_STATISTICS_ENERGY
#define ENTITY_STATISTICS_ENERGY

#include <QObject>
#include <QtSql>
#include "DataEntity.hpp"
#include "GraphData.hpp"

class StatisticsEnergy
{
public:
    Q_GADGET

public:
    QString SerialNo_;
public:
    float ActiveImportTariff1_;
public:
    float ActiveExportTariff1_;
public:
    float ReactiveImportTariff1_;
public:
    float ReactiveExportTariff1_;
public:
    float ActiveImportTariff2_;
public:
    float ActiveExportTariff2_;
public:
    float ReactiveImportTariff2_;
public:
    float ReactiveExportTariff2_;
public:
    uint32_t LoadProfileType_;
public:
    uint32_t RecordTimestamp_;
public:
    uint32_t RowIndex_;

public:
    Q_PROPERTY(QString  SerialNo MEMBER SerialNo_)
    Q_PROPERTY(float ActiveImportTariff1 MEMBER ActiveImportTariff1_)
    Q_PROPERTY(float ActiveExportTariff1 MEMBER ActiveExportTariff1_)
    Q_PROPERTY(float ReactiveImportTariff1 MEMBER ReactiveImportTariff1_)
    Q_PROPERTY(float ReactiveExportTariff1 MEMBER ReactiveExportTariff1_)
    Q_PROPERTY(float ActiveImportTariff2 MEMBER ActiveImportTariff2_)
    Q_PROPERTY(float ActiveExportTariff2 MEMBER ActiveExportTariff2_)
    Q_PROPERTY(float ReactiveImportTariff2 MEMBER ReactiveImportTariff2_)
    Q_PROPERTY(float ReactiveExportTariff2 MEMBER ReactiveExportTariff2_)
    Q_PROPERTY(uint32_t  LoadProfileType MEMBER LoadProfileType_)
    Q_PROPERTY(uint32_t  RecordTimestamp MEMBER RecordTimestamp_)
    Q_PROPERTY(uint32_t  RowIndex MEMBER RowIndex_)

public:
    ~StatisticsEnergy()
    {

    }

    StatisticsEnergy()
    {
        SerialNo_ = "";
        ActiveImportTariff1_ = 0;
        ActiveExportTariff1_ = 0;
        ReactiveImportTariff1_ = 0;
        ReactiveExportTariff1_ = 0;
        ActiveImportTariff2_ = 0;
        ActiveExportTariff2_ = 0;
        ReactiveImportTariff2_ = 0;
        ReactiveExportTariff2_ = 0;
        RecordTimestamp_ = 0;
        LoadProfileType_ = 0;
        RowIndex_ = 0;
    }

    StatisticsEnergy(QString  iSerialNo, float
                     iActiveImportTariff1, float iActiveExportTariff1, float iReactiveImportTariff1, float iReactiveExportTariff1,
                     float iActiveImportTariff2, float iActiveExportTariff2, float iReactiveImportTariff2, float iReactiveExportTariff2,
                     uint32_t iLoadProfileType, uint32_t iRecordTimestamp, uint32_t iRowIndex)
    {
        SerialNo_ = iSerialNo;
        ActiveImportTariff1_ = iActiveImportTariff1;
        ActiveExportTariff1_ = iActiveExportTariff1;
        ReactiveImportTariff1_ = iReactiveImportTariff1;
        ReactiveExportTariff1_ = iReactiveExportTariff1;
        ActiveImportTariff2_ = iActiveImportTariff2;
        ActiveExportTariff2_ = iActiveExportTariff2;
        ReactiveImportTariff2_ = iReactiveImportTariff2;
        ReactiveExportTariff2_ = iReactiveExportTariff2;
        LoadProfileType_ = iLoadProfileType;
        RecordTimestamp_ = iRecordTimestamp;
        RowIndex_ = iRowIndex;
    }

    StatisticsEnergy(const StatisticsEnergy& other)
    {
        SerialNo_ = other.SerialNo_;
        ActiveImportTariff1_ = other.ActiveImportTariff1_;
        ActiveExportTariff1_ = other.ActiveExportTariff1_;
        ReactiveImportTariff1_ = other.ReactiveImportTariff1_;
        ReactiveExportTariff1_ = other.ReactiveExportTariff1_;
        ActiveImportTariff2_ = other.ActiveImportTariff2_;
        ActiveExportTariff2_ = other.ActiveExportTariff2_;
        ReactiveImportTariff2_ = other.ReactiveImportTariff2_;
        ReactiveExportTariff2_ = other.ReactiveExportTariff2_;
        LoadProfileType_ = other.LoadProfileType_;
        RecordTimestamp_ = other.RecordTimestamp_;
        RowIndex_ = other.RowIndex_;
    }

    StatisticsEnergy& operator = (const StatisticsEnergy& other)
    {
        SerialNo_ = other.SerialNo_;
        ActiveImportTariff1_ = other.ActiveImportTariff1_;
        ActiveExportTariff1_ = other.ActiveExportTariff1_;
        ReactiveImportTariff1_ = other.ReactiveImportTariff1_;
        ReactiveExportTariff1_ = other.ReactiveExportTariff1_;
        ActiveImportTariff2_ = other.ActiveImportTariff2_;
        ActiveExportTariff2_ = other.ActiveExportTariff2_;
        ReactiveImportTariff2_ = other.ReactiveImportTariff2_;
        ReactiveExportTariff2_ = other.ReactiveExportTariff2_;
        LoadProfileType_ = other.LoadProfileType_;
        RecordTimestamp_ = other.RecordTimestamp_;
        RowIndex_ = other.RowIndex_;

        return *this;
    }
};

class EntityStatisticsEnergy : public DataEntity
{
public:
    EntityStatisticsEnergy(QSqlDatabase* db, QString tabname);
    virtual ~EntityStatisticsEnergy();
    bool createTable();
    bool createStatisticsEnergy(const StatisticsEnergy &rec);
    bool allStatisticsEnergys(QList<StatisticsEnergy> &list, QString serial_no);
    bool selectedStatisticsEnergys(QList<StatisticsEnergy> &list, QString serial_no, QString keyname, QVariant value);
    bool getIndexRanges(QString serial_no,uint32_t &startRowIndex, uint32_t &endRowIndex, uint32_t lptype);
    bool getTimestampRanges(QString serial_no, uint32_t &startTimestamp, uint32_t &endTimestamp, uint32_t lptype);
    bool getStartIndex(QString serial_no,uint32_t &startRowIndex, uint32_t &startTimestamp, uint32_t currentTimestamp, uint32_t lptype);
    bool selectedStatisticsEnergysByWeek(QList<BarData> &barlist, QList<LineData> &linelist, QString serial_no, QString keyname, uint32_t datetimestart, uint32_t datetimeend, float &max, float &total, float &average);
    bool selectedStatisticsEnergysByMonth(QList<BarData> &barlist, QList<LineData> &linelist, QString serial_no, QString keyname, uint32_t datetimestart, uint32_t datetimeend, float &max, float &total, float &average);
    bool selectedStatisticsEnergysByYear(QList<BarData> &barlist, QList<LineData> &linelist, QString serial_no, QString keyname, uint32_t datetimestart, uint32_t datetimeend, float &max, float &total, float &average);
    bool selectedStatisticsEnergysByDay(QList<BarData> &barlist, QList<LineData> &linelist, QString serial_no, QString keyname, uint32_t datetimestart, uint32_t datetimeend, float &max, float &total, float &average);
};

extern EntityStatisticsEnergy* statisticsEnergyEntityPtr;

#endif
