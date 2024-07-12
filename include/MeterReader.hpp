#ifndef METER_READER
#define METER_READER

#include <QtQuickWidgets/QQuickWidget>
#include <QGuiApplication>
#include <QPalette>
#include <QMap>
#include <QObject>
#include <QString>
#include <QQuickItem>
#include <QSettings>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickView>
#include <QFileInfo>
#include <QStandardPaths>
#include <QDir>
#include <QFile>

#include "DataInterface.hpp"

class MeterReader : public QGuiApplication
{
    Q_OBJECT
public:
    explicit MeterReader(int argc, char *argv[]);
    virtual ~MeterReader();
    bool InitializeDatabase();
    void ResetDemoStatistics();
private:
    void populateAlarmFilters();
    void populateDemoMasterData();
    void populateMeterObjects();
    void populateEnergyProfileData(QString serialNo);
    DataInterface db;
};

extern MeterReader* csa;

#endif
