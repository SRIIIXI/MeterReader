#ifndef ENTITY_METER
#define ENTITY_METER

#include <QObject>
#include <QtSql>
#include "DataEntity.hpp"

class Meter
{
public:
    Q_GADGET

public:
    QString SerialNo_;
public:
    QString MeterName_;
public:
    QString ModelNo_;
public:
    QString SystemTitle_;
public:
    QString CustomerPIN_;
public:
    QString PairingPIN_;
public:
    QString Keys103_;
public:
    QString Keys104_;
public:
    QString Keys105_;
public:
    uint32_t LastSync_;
public:
    QString LastSyncStr_;
public:
    QString RSSIStr_;
public:
    QString StatusStr_;
public:
    uint32_t IsSelected_;
public:
    Q_PROPERTY(QString SerialNo MEMBER SerialNo_)
    Q_PROPERTY(QString MeterName MEMBER MeterName_)
    Q_PROPERTY(QString ModelNo MEMBER ModelNo_)
    Q_PROPERTY(QString SystemTitle MEMBER SystemTitle_)
    Q_PROPERTY(QString CustomerPIN MEMBER CustomerPIN_)
    Q_PROPERTY(QString PairingPIN MEMBER PairingPIN_)
    Q_PROPERTY(QString Keys103 MEMBER Keys103_)
    Q_PROPERTY(QString Keys104 MEMBER Keys104_)
    Q_PROPERTY(QString Keys105 MEMBER Keys105_)
    Q_PROPERTY(uint32_t LastSync MEMBER LastSync_)
    Q_PROPERTY(QString LastSyncStr MEMBER LastSyncStr_)
    Q_PROPERTY(QString RSSIStr MEMBER RSSIStr_)
    Q_PROPERTY(QString StatusStr MEMBER StatusStr_)
    Q_PROPERTY(uint32_t IsSelected MEMBER IsSelected_)

public:
    ~Meter()
    {

    }

    Meter()
    {
        IsSelected_ = false;
    }

    Meter(QString iStatusStr, QString iSerialNo, QString iMeterName, uint32_t iLastSync, QString iRSSI, QString iModelNo)
    {
        SerialNo_ = iSerialNo;
        MeterName_ = iMeterName;
        ModelNo_ = iModelNo;
        LastSync_ = iLastSync;
        LastSyncStr_ = QDateTime::fromSecsSinceEpoch(LastSync_).toString("dd MMM yy hh:mm");
        RSSIStr_ = iRSSI;
        StatusStr_ = iStatusStr;
    }

    Meter(const Meter& other)
    {
        SerialNo_ = other.SerialNo_;
        MeterName_ = other.MeterName_;
        ModelNo_ = other.ModelNo_;
        SystemTitle_ = other.SystemTitle_;
        CustomerPIN_ = other.CustomerPIN_;
        PairingPIN_ = other.PairingPIN_;
        Keys103_ = other.Keys103_;
        Keys104_ = other.Keys104_;
        Keys105_ = other.Keys105_;
        LastSync_ = other.LastSync_;
        LastSyncStr_ = QDateTime::fromSecsSinceEpoch(LastSync_).toString("dd MMM yy hh:mm");
        RSSIStr_ = other.RSSIStr_;
        StatusStr_ = other.StatusStr_;
        IsSelected_ = other.IsSelected_;
    }

    Meter& operator = (const Meter& other)
    {
        SerialNo_ = other.SerialNo_;
        MeterName_ = other.MeterName_;
        ModelNo_ = other.ModelNo_;
        SystemTitle_ = other.SystemTitle_;
        CustomerPIN_ = other.CustomerPIN_;
        PairingPIN_ = other.PairingPIN_;
        Keys103_ = other.Keys103_;
        Keys104_ = other.Keys104_;
        Keys105_ = other.Keys105_;
        LastSync_ = other.LastSync_;
        LastSyncStr_ = QDateTime::fromSecsSinceEpoch(LastSync_).toString("dd MMM yy hh:mm");
        RSSIStr_ = other.RSSIStr_;
        StatusStr_ = other.StatusStr_;
        IsSelected_ = other.IsSelected_;
        return *this;
    }

    void Clear()
    {
        SerialNo_ = "";
        MeterName_ = "";
        ModelNo_ = "";
        SystemTitle_ = "";
        CustomerPIN_ = "";
        PairingPIN_ = "";
        Keys103_ = "";
        Keys104_ = "";
        Keys105_ = "";
        LastSync_ = QDateTime::currentDateTime().toMSecsSinceEpoch();
        LastSyncStr_ = QDateTime::fromSecsSinceEpoch(LastSync_).toString("dd MMM yy hh:mm");
        RSSIStr_ = "";
        StatusStr_ = "";
        IsSelected_ = 0;
    }
};

class EntityMeter : public DataEntity
{
public:
    EntityMeter(QSqlDatabase* db, QString tabname);
    virtual ~EntityMeter();
    bool createTable();
    bool createMeter(const Meter &rec);
    bool allMeters(QList<Meter> &list, bool includedemo);
    bool selectedMeters(QList<Meter> &list, QString keyname, QVariant value, bool includedemo);
};

extern EntityMeter* meterEntityPtr;

#endif
