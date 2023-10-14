#ifndef ENTITY_ALARM_FILTER
#define ENTITY_ALARM_FILTER

#include <QObject>
#include <QtSql>
#include "DataEntity.hpp"


class AlarmFilter
{
public:
    Q_GADGET

public:
    bool IsSet_;
public:
    QString AlarmName_;
public:
    uint32_t  AlarmId_;
public:
    Q_PROPERTY(bool IsSet MEMBER IsSet_)
    Q_PROPERTY(QString AlarmName MEMBER AlarmName_)
    Q_PROPERTY(uint32_t  AlarmId MEMBER AlarmId_)

public:
    ~AlarmFilter()
    {

    }

    AlarmFilter()
    {

    }

    AlarmFilter(bool iIsSet, QString iAlarmName, uint32_t  iAlarmId)
    {
        IsSet_ = iIsSet;
        AlarmName_ = iAlarmName;
        AlarmId_ = iAlarmId;
    }

    AlarmFilter(const AlarmFilter& other)
    {
        IsSet_ = other.IsSet_;
        AlarmName_ = other.AlarmName_;
        AlarmId_ = other.AlarmId_;
    }

    AlarmFilter& operator = (const AlarmFilter& other)
    {
        IsSet_ = other.IsSet_;
        AlarmName_ = other.AlarmName_;
        AlarmId_ = other.AlarmId_;
        return *this;
    }
};

class EntityAlarmFilter : public DataEntity
{
public:
    EntityAlarmFilter(QSqlDatabase* db, QString tabname);
    virtual ~EntityAlarmFilter();
    bool createTable();
    bool createAlarmFilter(const AlarmFilter &rec);
    bool allAlarmFilters(QList<AlarmFilter> &list);
    bool selectedAlarmFilters(QList<AlarmFilter> &list, QString keyname, QVariant value);
};

extern EntityAlarmFilter* alarmFilterEntityPtr;

#endif

