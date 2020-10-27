#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtCore>
#include <QtAV/QtAV>
#include <QtAVWidgets/QtAVWidgets>
#include <QtNetwork>
#include <csignal>
#include "API/NetworkAPI.h"
#include "Networking/NetworkDevice.h"
#include "Networking/NetworkDeviceScanner.h"
#include "Networking/NetworkSinkHandler.h"


QApplication *app;

void signalHandler(int signum) {
    Q_UNUSED(signum)
    QMetaObject::invokeMethod(app, "quit", Qt::QueuedConnection);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, &signalHandler);
    signal(SIGTERM, &signalHandler);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    app = new QApplication(argc, argv);

    NetworkAPI api;
    api.init();
    QtAV::Widgets::registerRenderers();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("deviceDirectory", api.getDeviceDirectory());
    engine.rootContext()->setContextProperty("backendAPI", &api);
    engine.rootContext()->setContextProperty("sinkHandler", api.getSinkHandler());
    const QUrl url(QStringLiteral("qrc:/gui/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    });
    engine.load(url);
    api.start();
//    QTimer timer;
//    timer.setSingleShot(true);
//    QObject::connect(&timer, &QTimer::timeout, &sinkHandler, &NetworkSinkHandler::stopDiscoverable);
//    timer.start(10000);

    QObject::connect(app, &QGuiApplication::aboutToQuit, &api, &NetworkAPI::stop);

    return app->exec();
}
