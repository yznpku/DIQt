#include <QApplication>
#include <QQmlApplicationEngine>

#include "countservice.h"
#include <DIQt>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    DIQtModule module;
    module.provideAtRoot<CountService>();

    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, [&module](QObject* object, const QUrl& url) {
        module.bootstrap(object);
    });

    engine.load(":/main.qml");
    return app.exec();
}
