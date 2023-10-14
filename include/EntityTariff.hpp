#ifndef ENTITY_TARIFF
#define ENTITY_TARIFF

#include <QObject>
#include <QtSql>
#include "DataEntity.hpp"


class Tariff
{
public:
    Q_GADGET

public:
    bool IsApplied_;
public:
    QString SerialNo_;
public:
    uint32_t EffectiveDate_;
public:
    QString EffectiveDateStr_;
public:
    QString TariffId_;
public:
    float Import1_;
public:
    float Export1_;
public:
    float Import2_;
public:
    float Export2_;
public:
    Q_PROPERTY(bool IsApplied MEMBER IsApplied_)
    Q_PROPERTY(QString SerialNo MEMBER SerialNo_)
    Q_PROPERTY(uint32_t EffectiveDate MEMBER EffectiveDate_)
    Q_PROPERTY(QString EffectiveDateStr MEMBER EffectiveDateStr_)
    Q_PROPERTY(QString TariffId MEMBER TariffId_)
    Q_PROPERTY(float Import1 MEMBER Import1_)
    Q_PROPERTY(float Export1 MEMBER Export1_)
    Q_PROPERTY(float Import2 MEMBER Import2_)
    Q_PROPERTY(float Export2 MEMBER Export2_)

public:
    ~Tariff()
    {

    }

    Tariff()
    {

    }

    Tariff(bool iIsApplied, QString iSerialNo, uint32_t iEffectiveDate, QString iTariffId, float iImport1, float iExport1, float iImport2, float iExport2)
    {
        SerialNo_ = iSerialNo;
        IsApplied_ = iIsApplied;
        EffectiveDate_ = iEffectiveDate;
        TariffId_ = iTariffId;
        Import1_ = iImport1;
        Export1_ = iExport1;
        Import2_ = iImport2;
        Export2_ = iExport2;
        EffectiveDateStr_ = QDateTime::fromSecsSinceEpoch(EffectiveDate_).toString("dd MMM yy");
    }

    Tariff(const Tariff& other)
    {
        SerialNo_ = other.SerialNo_;
        IsApplied_ = other.IsApplied_;
        EffectiveDate_ = other.EffectiveDate_;
        TariffId_ = other.TariffId_;
        Import1_ = other.Import1_;
        Export1_ = other.Export1_;
        Import2_ = other.Import2_;
        Export2_ = other.Export2_;
        EffectiveDateStr_ = QDateTime::fromSecsSinceEpoch(other.EffectiveDate_).toString("dd MMM yy");
    }

    Tariff& operator = (const Tariff& other)
    {
        SerialNo_ = other.SerialNo_;
        IsApplied_ = other.IsApplied_;
        EffectiveDate_ = other.EffectiveDate_;
        TariffId_ = other.TariffId_;
        Import1_ = other.Import1_;
        Export1_ = other.Export1_;
        Import2_ = other.Import2_;
        Export2_ = other.Export2_;
        EffectiveDateStr_ = QDateTime::fromSecsSinceEpoch(other.EffectiveDate_).toString("dd MMM yy");
        return *this;
    }
};

class EntityTariff : public DataEntity
{
public:
    EntityTariff(QSqlDatabase* db, QString tabname);
    virtual ~EntityTariff();
    bool createTable();
    bool createTariff(const Tariff &rec);
    bool allTariffs(QList<Tariff> &list);
    bool selectedTariffs(QList<Tariff> &list, QString keyname, QVariant value);
};

extern EntityTariff* tariffEntityPtr;

#endif
