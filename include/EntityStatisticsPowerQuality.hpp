#ifndef ENTITY_STATISTICS_POWER_QUALITY
#define ENTITY_STATISTICS_POWER_QUALITY

#include <QObject>
#include <QtSql>
#include "DataEntity.hpp"
#include "qstandarditemmodel.h"


class StatisticsPowerQuality
{
public:
    Q_GADGET

public:
    QString  SerialNo_;
public:
    float AverageVoltageA_;
public:
    float AverageVoltageB_;
public:
    float AverageVoltageC_;
public:
    float AverageCurrentA_;
public:
    float AverageCurrentB_;
public:
    float AverageCurrentC_;
public:
    float AveragePowerFactorTotal_;
public:
    float AverageMCUTemperature_;
public:
    float InstantenousNetFrequency_;
public:
    float AverageTHDVoltageA_;
public:
    float AverageTHDVoltageB_;
public:
    float AverageTHDVoltageC_;
public:
    uint32_t  RecordTimestamp_;
public:
    uint32_t RowIndex_;

public:
    Q_PROPERTY(QString  SerialNo MEMBER SerialNo_)
    Q_PROPERTY(float AverageVoltageA MEMBER AverageVoltageA_)
    Q_PROPERTY(float AverageVoltageB MEMBER AverageVoltageB_)
    Q_PROPERTY(float AverageVoltageC MEMBER AverageVoltageC_)
    Q_PROPERTY(float AverageCurrentA MEMBER AverageCurrentA_)
    Q_PROPERTY(float AverageCurrentB MEMBER AverageCurrentB_)
    Q_PROPERTY(float AverageCurrentC MEMBER AverageCurrentC_)
    Q_PROPERTY(float AveragePowerFactorTotal MEMBER AveragePowerFactorTotal_)
    Q_PROPERTY(float AverageMCUTemperature MEMBER AverageMCUTemperature_)
    Q_PROPERTY(float IntantenousNetFrequency MEMBER InstantenousNetFrequency_)
    Q_PROPERTY(float AverageTHDVoltageA MEMBER AverageTHDVoltageA_)
    Q_PROPERTY(float AverageTHDVoltageB MEMBER AverageTHDVoltageB_)
    Q_PROPERTY(float AverageTHDVoltageC MEMBER AverageTHDVoltageC_)
    Q_PROPERTY(uint32_t  RecordTimestamp MEMBER RecordTimestamp_)
    Q_PROPERTY(uint32_t  RowIndex MEMBER RowIndex_)

public:
    ~StatisticsPowerQuality()
    {

    }

    StatisticsPowerQuality()
    {
        SerialNo_ = "";
        RecordTimestamp_ = 0;
        AverageVoltageA_ = 0;
        AverageVoltageB_ = 0;
        AverageVoltageC_ = 0;
        AverageCurrentA_ = 0;
        AverageCurrentB_ = 0;
        AverageCurrentC_ = 0;
        AveragePowerFactorTotal_ = 0;
        AverageMCUTemperature_ = 0;
        InstantenousNetFrequency_ = 0;
        AverageTHDVoltageA_ = 0;
        AverageTHDVoltageB_ = 0;
        AverageTHDVoltageC_ = 0;
        RowIndex_ = 0;
    }

    StatisticsPowerQuality(QString  iSerialNo, uint32_t  iRecordTimestamp,
                           float iAverageVoltageA, float iAverageVoltageB, float iAverageVoltageC,
                           float iAverageCurrentA, float iAverageCurrentB, float iAverageCurrentC,
                           float iAveragePowerFactorTotal, float iAverageMCUTemperature, float iInstantenousNetFrequency,
                           float iAverageTHDVoltageA, float iAverageTHDVoltageB, float iAverageTHDVoltageC,
                            uint32_t iRowIndex)
    {
        SerialNo_ = iSerialNo;
        RecordTimestamp_ = iRecordTimestamp;
        AverageVoltageA_ = iAverageVoltageA;
        AverageVoltageB_ = iAverageVoltageB;
        AverageVoltageC_ = iAverageVoltageC;
        AverageCurrentA_ = iAverageCurrentA;
        AverageCurrentB_ = iAverageCurrentB;
        AverageCurrentC_ = iAverageCurrentC;
        AveragePowerFactorTotal_ = iAveragePowerFactorTotal;
        AverageMCUTemperature_ = iAverageMCUTemperature;
        InstantenousNetFrequency_ = iInstantenousNetFrequency;
        AverageTHDVoltageA_ = iAverageTHDVoltageA;
        AverageTHDVoltageB_ = iAverageTHDVoltageB;
        AverageTHDVoltageC_ = iAverageTHDVoltageC;
        RowIndex_ = iRowIndex;
    }

    StatisticsPowerQuality(const StatisticsPowerQuality& other)
    {
        SerialNo_ = other.SerialNo_;
        RecordTimestamp_ = other.RecordTimestamp_;
        AverageVoltageA_ = other.AverageVoltageA_;
        AverageVoltageB_ = other.AverageVoltageB_;
        AverageVoltageC_ = other.AverageVoltageC_;
        AverageCurrentA_ = other.AverageCurrentA_;
        AverageCurrentB_ = other.AverageCurrentB_;
        AverageCurrentC_ = other.AverageCurrentC_;
        AveragePowerFactorTotal_ = other.AveragePowerFactorTotal_;
        AverageMCUTemperature_ = other.AverageMCUTemperature_;
        InstantenousNetFrequency_ = other.InstantenousNetFrequency_;
        AverageTHDVoltageA_ = other.AverageTHDVoltageA_;
        AverageTHDVoltageB_ = other.AverageTHDVoltageB_;
        AverageTHDVoltageC_ = other.AverageTHDVoltageC_;
        RowIndex_ = other.RowIndex_;
    }

    StatisticsPowerQuality& operator = (const StatisticsPowerQuality& other)
    {
        SerialNo_ = other.SerialNo_;
        RecordTimestamp_ = other.RecordTimestamp_;
        AverageVoltageA_ = other.AverageVoltageA_;
        AverageVoltageB_ = other.AverageVoltageB_;
        AverageVoltageC_ = other.AverageVoltageC_;
        AverageCurrentA_ = other.AverageCurrentA_;
        AverageCurrentB_ = other.AverageCurrentB_;
        AverageCurrentC_ = other.AverageCurrentC_;
        AveragePowerFactorTotal_ = other.AveragePowerFactorTotal_;
        AverageMCUTemperature_ = other.AverageMCUTemperature_;
        InstantenousNetFrequency_ = other.InstantenousNetFrequency_;
        AverageTHDVoltageA_ = other.AverageTHDVoltageA_;
        AverageTHDVoltageB_ = other.AverageTHDVoltageB_;
        AverageTHDVoltageC_ = other.AverageTHDVoltageC_;
        RowIndex_ = other.RowIndex_;

        return *this;
    }
};

class EntityStatisticsPowerQuality : public DataEntity
{
public:
    EntityStatisticsPowerQuality(QSqlDatabase* db, QString tabname);
    virtual ~EntityStatisticsPowerQuality();
    bool createTable();
    bool createStatisticsPowerQuality(const StatisticsPowerQuality &rec);
    bool allStatisticsPowerQualitys(QList<StatisticsPowerQuality> &list);
    bool selectedStatisticsPowerQualitys(QList<StatisticsPowerQuality> &list, QString keyname, QVariant value);
    bool getRanges(QString serial_no,uint32_t &startRowIndex, uint32_t &endRowIndex, uint32_t &startTimestamp, uint32_t &endTimestamp);
    bool getStartIndex(QString serial_no,uint32_t &startRowIndex, uint32_t &startTimestamp, uint32_t currentTimestamp);
    bool selectedStatisticsPQsByWeek(QStandardItemModel &xylist, QList<float> &datalist, QList<QString> &tslist, QString serial_no, QString keyname, QString date);
    bool selectedStatisticsPQsByMonth(QStandardItemModel &xylist, QList<float> &datalist, QList<QString> &tslist, QString serial_no, QString keyname, QString date);
    bool selectedStatisticsPQsByYear(QStandardItemModel &xylist, QList<float> &datalist, QList<QString> &tslist, QString serial_no, QString keyname, QString date);
    bool selectedStatisticsPQsByDay(QStandardItemModel &xylist, QList<float> &datalist, QList<QString> &tslist, QString serial_no, QString keyname, QString date);
};

extern EntityStatisticsPowerQuality* statisticsPowerQualityEntityPtr;


#endif
