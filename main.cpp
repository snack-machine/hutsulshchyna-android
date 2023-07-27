
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "serialport-handler.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<SerialPortHandler>("SerialPortHandler", 1, 0, "SerialPortHandler");
    qmlRegisterUncreatableMetaObject(
        SerialPortENUM::staticMetaObject, // meta object created by Q_NAMESPACE macro
        "SerialPortENUM",                // import statement (can be any string)
        1, 0,                          // major and minor version of the import
        "SerialPort",                 // name in QML (does not have to match C++ name)
        "Error: only enums"            // error in case someone tries to create a MyNamespace object
        );

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/hutsulshchyna-android/qml/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

//#include <QtWidgets/QApplication>
//#include <QtQuick/QQuickView>
//#include <QtQml/QQmlEngine>
//#include <QtQml/QQmlContext>
//#include <QTimer>


//int main(int argc, char *argv[])
//{
//    QApplication app(argc, argv);

//    QQuickView viewer;
//    QObject::connect(viewer.engine(), &QQmlEngine::quit, &viewer, &QWindow::close);
//    viewer.setTitle(QStringLiteral("hutsulshchyna-android"));
//    viewer.setSource(QUrl("qrc:/hutsulshchyna-android/Main.qml"));
//    viewer.setResizeMode(QQuickView::SizeRootObjectToView);
//    viewer.show();

//    return app.exec();
//}