#ifndef DATA_ENTITY_H
#define DATA_ENTITY_H

#include <QObject>
#include <QtSql>
#include <stdint.h>
#include <stdbool.h>


class DataEntity
{
public:
    DataEntity(QSqlDatabase* db, QString tabname);
    virtual ~DataEntity();
    void clear();
    void truncate();
    QList<QSqlRecord>* getAllRecords(QString str);
    QList<QSqlRecord>* getSelectedRecords(QString keyname, QVariant value, QString str);
    QList<QSqlRecord>* getCustomRecords(QString str);
    bool remove(QString keyname, QVariant value);
    bool update(QString uniquekeyname, QVariant uniquevalue, QString keyname, QVariant value);
    virtual bool createTable()=0;
    QVariant maximumValue(QString keyname);
    QVariant maximumValue(QString keyname, QString filter, QVariant filterval);
protected:
    void normalizeDateString(QString &str);
    void normalizeDateString(uint32_t val, QString &str);
    void substitueValue(QVariant &var, QString &str);
    QString getDayofWeek(QString str);
    QString getMonth(QString str);
    QString getFloatString(const float val);
    QSqlDatabase*       _Database;
    QString             _TableName;
    QList<QSqlRecord>   _RecordList;
};

#endif
