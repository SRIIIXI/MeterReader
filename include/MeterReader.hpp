#ifndef EDMI_CSA
#define EDMI_CSA

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

class EdmiCsa : public QGuiApplication
{
    Q_OBJECT
public:
    explicit EdmiCsa(int argc, char *argv[]);
    virtual ~EdmiCsa();
    bool StartApplication();
private:
    void populateAlarmFilters();
    void populateDemoData();
    DataInterface db;
};

#endif
