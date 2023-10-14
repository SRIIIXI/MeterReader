#ifndef ENTITY_METER
#define ENTITY_METER

#include <QObject>
#include <QtSql>
#include "DataEntity.hpp"

typedef enum MeterConnectionState
{
    NotConnected = 0,
    Connected = 1
}MeterConnectionState;

class Meter
{
public:
    Q_GADGET

public:
    uint32_t Status_;
public:
    QString SerialNo_;
public:
    QString MeterName_;
public:
    uint32_t LastSync_;
public:
    QString LastSyncStr_;
public:
    uint32_t RSSI_;
public:
    QString ModelNo_;
public:
    QString BlePIN_;
public:
    QString CIUPIN_;
public:
    QString SystemTitle_;
public:
    QString Keys103_;
public:
    QString Keys104_;
public:
    QString Keys105_;
public:
    QString BleIdentifier_;
public:
    QString StatusStr_;
public:
    QString RSSIStr_;
public:
    bool IsSelected_;
public:
    Q_PROPERTY(uint32_t Status MEMBER Status_)
    Q_PROPERTY(QString StatusStr MEMBER StatusStr_)
    Q_PROPERTY(QString SerialNo MEMBER SerialNo_)
    Q_PROPERTY(QString MeterName MEMBER MeterName_)
    Q_PROPERTY(uint32_t LastSync MEMBER LastSync_)
    Q_PROPERTY(QString LastSyncStr MEMBER LastSyncStr_)
    Q_PROPERTY(uint32_t RSSI MEMBER RSSI_)
    Q_PROPERTY(QString RSSIStr MEMBER RSSIStr_)
    Q_PROPERTY(QString ModelNo MEMBER ModelNo_)
    Q_PROPERTY(QString BlePIN MEMBER BlePIN_)
    Q_PROPERTY(QString CIUPIN MEMBER CIUPIN_)
    Q_PROPERTY(QString SystemTitle MEMBER SystemTitle_)
    Q_PROPERTY(QString Keys103 MEMBER Keys103_)
    Q_PROPERTY(QString Keys104 MEMBER Keys104_)
    Q_PROPERTY(QString Keys105 MEMBER Keys105_)
    Q_PROPERTY(QString BleIdentifier MEMBER BleIdentifier_)
    Q_PROPERTY(bool IsSelected MEMBER IsSelected_)

public:
    ~Meter()
    {

    }

    Meter()
    {
        IsSelected_ = false;
    }

    Meter(uint32_t iStatus, QString iSerialNo, QString iMeterName, uint32_t iLastSync, uint32_t iRSSI, QString iModelNo)
    {
        Status_ = iStatus;
        SerialNo_ = iSerialNo;
        MeterName_ = iMeterName;
        LastSync_ = iLastSync;
        RSSI_ = iRSSI;
        ModelNo_ = iModelNo;
        LastSyncStr_ = QDateTime::fromSecsSinceEpoch(LastSync_).toString("dd MMM yy hh:mm");
        StatusStr_ = QVariant(Status_).toString();
        RSSIStr_ = QVariant(RSSI_).toString();
    }

    Meter(const Meter& other)
    {
        Status_ = other.Status_;
        SerialNo_ = other.SerialNo_;
        MeterName_ = other.MeterName_;
        LastSync_ = other.LastSync_;
        RSSI_ = other.RSSI_;
        ModelNo_ = other.ModelNo_;
        BlePIN_ = other.BlePIN_;
        CIUPIN_ = other.CIUPIN_;
        SystemTitle_ = other.SystemTitle_;
        Keys103_ = other.Keys103_;
        Keys104_ = other.Keys104_;
        Keys105_ = other.Keys105_;
        BleIdentifier_ = other.BleIdentifier_;
        RSSIStr_ = other.RSSIStr_;
        StatusStr_ = other.StatusStr_;
        LastSyncStr_ = QDateTime::fromSecsSinceEpoch(LastSync_).toString("dd MMM yy hh:mm");
        IsSelected_ = other.IsSelected_;
    }

    Meter& operator = (const Meter& other)
    {
        Status_ = other.Status_;
        SerialNo_ = other.SerialNo_;
        MeterName_ = other.MeterName_;
        LastSync_ = other.LastSync_;
        RSSI_ = other.RSSI_;
        ModelNo_ = other.ModelNo_;
        BlePIN_ = other.BlePIN_;
        CIUPIN_ = other.CIUPIN_;
        SystemTitle_ = other.SystemTitle_;
        Keys103_ = other.Keys103_;
        Keys104_ = other.Keys104_;
        Keys105_ = other.Keys105_;
        BleIdentifier_ = other.BleIdentifier_;
        LastSyncStr_ = QDateTime::fromSecsSinceEpoch(LastSync_).toString("dd MMM yy hh:mm");
        RSSIStr_ = other.RSSIStr_;
        StatusStr_ = other.StatusStr_;
        IsSelected_ = other.IsSelected_;
        return *this;
    }
};

class EntityMeter : public DataEntity
{
public:
    EntityMeter(QSqlDatabase* db, QString tabname);
    virtual ~EntityMeter();
    bool createTable();
    bool createMeter(const Meter &rec);
    bool allMeters(QList<Meter> &list, bool includedemo = false);
    bool selectedMeters(QList<Meter> &list, QString keyname, QVariant value, bool includedemo = false);
};

extern EntityMeter* meterEntityPtr;

#endif
