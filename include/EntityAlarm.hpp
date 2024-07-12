#ifndef ENTITY_ALARM
#define ENTITY_ALARM

#include <QObject>
#include <QtSql>
#include "DataEntity.hpp"

class Alarm
{
public:
    Q_GADGET

public:
    QString AlarmName_;
public:
    QString SerialNo_;
public:
    uint32_t  AlarmId_;
public:
    uint32_t  AlarmTimestamp_;
public:
    Q_PROPERTY(QString AlarmName MEMBER AlarmName_)
    Q_PROPERTY(QString SerialNo MEMBER SerialNo_)
    Q_PROPERTY(uint32_t  AlarmId MEMBER AlarmId_)
    Q_PROPERTY(uint32_t  AlarmTimestamp MEMBER AlarmTimestamp_)

public:
    ~Alarm()
    {

    }

    Alarm()
    {

    }

    Alarm(QString iAlarmName, QString iSerialNo, uint32_t  iAlarmId, uint32_t  iAlarmTimestamp)
    {
        SerialNo_ = iSerialNo;
        AlarmName_ = iAlarmName;
        AlarmTimestamp_ = iAlarmId;
        AlarmId_ = iAlarmTimestamp;
    }

    Alarm(const Alarm& other)
    {
        SerialNo_ = other.SerialNo_;
        AlarmName_ = other.AlarmName_;
        AlarmTimestamp_ = other.AlarmTimestamp_;
        AlarmId_ = other.AlarmId_;
    }

    Alarm& operator = (const Alarm& other)
    {
        SerialNo_ = other.SerialNo_;
        AlarmName_ = other.AlarmName_;
        AlarmTimestamp_ = other.AlarmTimestamp_;
        AlarmId_ = other.AlarmId_;
        return *this;
    }
};

class EntityAlarm : public DataEntity
{
public:
    EntityAlarm(QSqlDatabase* db, QString tabname);
    virtual ~EntityAlarm();
    bool createTable();
    bool createAlarm(const Alarm &rec);
    bool selectedAlarms(QList<Alarm> &list, QString keyname, QVariant value);
};

extern EntityAlarm* alarmEntityPtr;

#endif
