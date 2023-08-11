#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QSettings>

#include "coord-handler.h"
#include "file-handler.h"

#ifdef PLATFORM_ANDROID
#include "serialport-handler-android.h"
#include <QJniObject>
#include <QtCore/private/qandroidextras_p.h>
#else
#include "serialport-handler.h"
#endif

//#include <QDebug>

#ifdef PLATFORM_ANDROID
namespace JavaPart {
    QJniObject javaPortHandler;
}
SerialPortHandler* globalSerialPortHandler = nullptr;

static void refreshDevice(JNIEnv* env, jobject thiz, jobjectArray stringArrays)
{
    Q_UNUSED(thiz)

    int size = env->GetArrayLength(stringArrays);
    for (int i=0; i < size; ++i)  {
        jstring string = static_cast<jstring>(env->GetObjectArrayElement(stringArrays, i));
        const char* myarray = env->GetStringUTFChars(string, nullptr);
        switch (i) {
        case 0:
            SerialPortHandler::portInfo.productName = myarray;
            break;
        case 1:
            SerialPortHandler::portInfo.manufacturerName = myarray;
            break;
        case 2:
            SerialPortHandler::portInfo.vendorID = myarray;
            break;
        case 3:
            SerialPortHandler::portInfo.productID = myarray;
            break;
        }
        env->ReleaseStringUTFChars(string, myarray);
        env->DeleteLocalRef(string);
    }
    SerialPortHandler::isDeviceFound = true;
}

static void handlePortData(JNIEnv* env, jobject thiz, jbyteArray value)
{
    Q_UNUSED(thiz)

    int byteArrayLength = env->GetArrayLength(value);
    jbyte* byteArrayElements = env->GetByteArrayElements(value, nullptr);
    QByteArray qtByteArray(reinterpret_cast<const char*>(byteArrayElements), byteArrayLength);

    if (globalSerialPortHandler) {
        emit globalSerialPortHandler->portDataRead(qtByteArray);
    }

    env->ReleaseByteArrayElements(value, byteArrayElements, JNI_ABORT);
}

static void handlePortError(JNIEnv* env, jobject thiz, jstring value)
{
    Q_UNUSED(thiz)
    const char* string = env->GetStringUTFChars(value, nullptr);

    SerialPortHandler::isPortOpen = false;

    if (globalSerialPortHandler) {
        emit globalSerialPortHandler->error(string);
        emit globalSerialPortHandler->portStateChanged();
    }

    env->ReleaseStringUTFChars(value, string);
    env->DeleteLocalRef(value);
}

bool checkPermission(const QString& perm)
{
    auto result = QtAndroidPrivate::checkPermission(perm).then([](QtAndroidPrivate::PermissionResult result) { return result; });
    result.waitForFinished();
    auto r = result.result();
    if (r == QtAndroidPrivate::Denied) {
        r = QtAndroidPrivate::requestPermission(perm).result();
        if (r == QtAndroidPrivate::Denied) {
            return false;
        }
    }
    return true;
}
#endif

int main(int argc, char *argv[])
{
    QGuiApplication::setOrganizationName("Coordinator");
    QGuiApplication::setApplicationName("Sigma");

    QGuiApplication app(argc, argv);
    QSettings settings(QGuiApplication::organizationName(), QGuiApplication::applicationName());

#ifdef PLATFORM_ANDROID
    jobject context = QNativeInterface::QAndroidApplication::context();
    JavaPart::javaPortHandler = QJniObject("com/example/portwrapper/PortWrapper", "(Landroid/content/Context;)V", context);

    const JNINativeMethod methods[] {{"refreshCpp", "([Ljava/lang/String;)V", reinterpret_cast<void *>(refreshDevice)},
                                    {"sendDataToCpp", "([B)V", reinterpret_cast<void *>(handlePortData)},
                                    {"errorToCpp", "(Ljava/lang/String;)V", reinterpret_cast<void *>(handlePortError)}};
    QJniEnvironment env;
    env.registerNativeMethods("com/example/portwrapper/PortWrapper", methods, 3);
#endif

    qmlRegisterType<SerialPortHandler>("SerialPortHandler", 1, 0, "SerialPortHandler");
    SerialPortHandler serialPortHandler(&settings);

    qmlRegisterType<SerialPortHandler>("CoordHandler", 1, 0, "CoordHandler");
    CoordHandler coordHandler;

    qmlRegisterType<SerialPortHandler>("FileHandler", 1, 0, "FileHandler");
    FileHandler fileHandler(&coordHandler);

#ifdef PLATFORM_ANDROID
    globalSerialPortHandler = &serialPortHandler;
#endif

    QObject::connect(&serialPortHandler, &SerialPortHandler::portDataRead, &coordHandler, &CoordHandler::getPortMessageSlot);
//    QObject::connect(&app, &QGuiApplication::applicationStateChanged, [&app, &serialPortHandler](Qt::ApplicationState state){
//        if (state == Qt::ApplicationSuspended) {
//            serialPortHandler.writeSettings();
//        }
//    });

    qmlRegisterUncreatableMetaObject(
        SerialPortENUM::staticMetaObject,   // meta object created by Q_NAMESPACE macro
        "SerialPortENUM",                   // import statement (can be any string)
        1, 0,                               // major and minor version of the import
        "SerialPort",                       // name in QML (does not have to match C++ name)
        "Error: only enums"                 // error in case someone tries to create a MyNamespace object
    );

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/hutsulshchyna-android/qml/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.rootContext()->setContextProperty("portHandler", &serialPortHandler);
    engine.rootContext()->setContextProperty("coordHandler", &coordHandler);
    engine.rootContext()->setContextProperty("fileHandler", &fileHandler);
    engine.load(url);

#ifdef PLATFORM_ANDROID
    const QVector<QString> permissions({
//        "android.permission.REQUEST_INSTALL_PACKAGES",
        "android.permission.READ_EXTERNAL_STORAGE",
        "android.permission.WRITE_EXTERNAL_STORAGE",
    });

    for(const QString& permission : permissions) {
        checkPermission(permission);
    }
#endif

    return app.exec();
}
