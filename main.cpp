#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "universeview.h"
#include "lifecontroller.h"
#include "sampleview.h"
#include "samplescatalog.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<UniverseView>("titkov.universeview", 1, 0, "UniverseView");
    qmlRegisterType<LifeController>("titkov.lifecontroller", 1, 0, "LifeController");
    qmlRegisterType<SampleView>("titkov.sampleview", 1, 0, "SampleView");
    qmlRegisterType<SamplesCatalog>("titkov.samplescatalog", 1, 0, "SamplesCatalog");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    engine.load(url);

    return app.exec();
}
