#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <appcore.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    AppCore appCore;
    QQmlContext *context = engine.rootContext();

    context->setContextProperty("appCore", &appCore);

    engine.load(QUrl(QStringLiteral("qrc:/mainform.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
