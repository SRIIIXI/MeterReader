#ifndef ENTITY_METER_SETTINGS
#define ENTITY_METER_SETTINGS

#include <QObject>
#include <QtSql>
#include "DataEntity.hpp"

extern int ACCOUNT_TYPE;
extern int ACCOUNT_CURRENCY;
extern int ACCOUNT_CURRENCY_SCALAR;
extern int ACTIVE_ENERGY_SCALAR;
extern int INSTANTENOUS_POWER_SCALAR;
extern int ACTIVE_EXPORT_SCALAR;
extern int ACTIVE_IMPORT_SCALAR;
extern int REACTIVE_EXPORT_SCALAR;
extern int REACTIVE_IMPORT_SCALAR;


class MeterSettings
{
public:
    Q_GADGET

public:
    QString SerialNo_;
public:
    int Key_;
public:
    QString Value_;

public:
    Q_PROPERTY(QString SerialNo MEMBER SerialNo_)
    Q_PROPERTY(int Key MEMBER Key_)
    Q_PROPERTY(QString Value MEMBER Value_)

public:
    ~MeterSettings()
    {

    }

    MeterSettings()
    {
    }

    MeterSettings(QString iSerialNo, int iKey, QString iValue)
    {
        SerialNo_ = iSerialNo;
        Key_ = iKey;
        Value_ = iValue;
    }

    MeterSettings(const MeterSettings& other)
    {
        SerialNo_ = other.SerialNo_;
        Key_ = other.Key_;
        Value_ = other.Value_;
    }

    MeterSettings& operator = (const MeterSettings& other)
    {
        SerialNo_ = other.SerialNo_;
        Key_ = other.Key_;
        Value_ = other.Value_;
        return *this;
    }

    void Clear()
    {
        SerialNo_ = "";
        Key_ = -1;
        Value_ = "";
    }
};

class EntityMeterSettings : public DataEntity
{
public:
    EntityMeterSettings(QSqlDatabase* db, QString tabname);
    virtual ~EntityMeterSettings();
    bool createTable();
    bool createMeterSettings(const MeterSettings &rec);
    bool allMetersSettings(QList<MeterSettings> &list);
    bool selectedMetersSettings(QList<MeterSettings> &list, QString keyname, QVariant value);
};

extern EntityMeterSettings* meterSettingsEntityPtr;

#endif
