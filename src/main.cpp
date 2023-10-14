#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "MeterReader.hpp"
#include "UICommandHandler.hpp"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(MeterReader);

    MeterReader  app(argc, argv);

    if(!app.StartApplication())
    {
        return -1;
    }

    UICommandHandler appData(&app);
    appData.Initialize();

    const QUrl urlMain("qrc:///qml/MainWindow.qml");

    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app, []()
        { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("applicationData", &appData);
    engine.load(urlMain);

    return app.exec();
}
