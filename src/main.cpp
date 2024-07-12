#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QUrl>
#include "MeterReader.hpp"
#include "Controller.hpp"
#include "BluetoothInterface.hpp"
#include "CameraInterface.hpp"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(MeterReader);

    MeterReader  app(argc, argv);
    BluetoothInterface binterface;
    CameraInterface cinterface;

    Controller controller(&app);

    if(!binterface.Initialize())
    {
        return -1;
    }

    if(!cinterface.Initialize())
    {
        return -1;
    }
    
    const QUrl urlMain("qrc:///qml/MainWindow.qml");

    QQmlApplicationEngine engine;

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app, []()
        { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("applicationData", &controller);
    engine.load(urlMain);
    
    if(!app.InitializeDatabase())
    {
        return -1;
    }

    controller.Initialize();

    controller.SignalInitializationComplete();

    return app.exec();
}
