#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtCore>
#include <QtNetwork>
#include <csignal>
#include "Networking/NetworkDevice.h"
#include "Networking/NetworkDeviceScanner.h"
#include "Networking/NetworkSinkHandler.h"


QGuiApplication *app;

void signalHandler(int signum) {
    Q_UNUSED(signum)
    QMetaObject::invokeMethod(app, "quit", Qt::QueuedConnection);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, &signalHandler);
    signal(SIGTERM, &signalHandler);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    app = new QGuiApplication(argc, argv);

    NetworkDevice *nd = new NetworkDevice(QHostAddress::AnyIPv4, "C++ Device");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    engine.rootContext()->setContextProperty("someDevice", nd);

    NetworkSinkHandler sinkHandler;
    NetworkDeviceDirectory directory;
    NetworkDeviceScanner scanner(&directory);
    scanner.start();

    QObject::connect(app, &QGuiApplication::aboutToQuit, &scanner, &NetworkDeviceScanner::stop);

    return app->exec();
}
