#ifndef SERIALPORTHANDLER_H
#define SERIALPORTHANDLER_H

#include <QObject>
//#include <QtSerialPort/QSerialPortInfo>
//#include <QtSerialPort/QSerialPort>
#include <QSettings.h>
#include <QJniObject>

namespace JavaPart {
    extern QJniObject javaPortHandler;
}

class SerialPortHandler : public QObject
{
    Q_OBJECT
//    Q_PROPERTY(QStringList ports READ getPortList NOTIFY portListChanged)
//    Q_PROPERTY(QString portName READ getPortName WRITE setPortName NOTIFY portNameChanged)
//    Q_PROPERTY(QSerialPort::BaudRate baudRate READ getBaudRate WRITE setBaudRate NOTIFY baudRateChanged)
//    Q_PROPERTY(QSerialPort::DataBits dataBits READ getDataBits WRITE setDataBits NOTIFY dataBitsChanged)
//    Q_PROPERTY(QSerialPort::Parity parity READ getParity WRITE setParity NOTIFY parityChanged)
//    Q_PROPERTY(QSerialPort::StopBits stopBits READ getStopBits WRITE setStopBits NOTIFY stopBitsChanged)
//    Q_PROPERTY(QSerialPort::FlowControl flowControl READ getFlowControl WRITE setFlowControl NOTIFY flowControlChanged)
    Q_PROPERTY(bool portOpen READ getPortStatus NOTIFY portStateChanged)
//    Q_PROPERTY(bool deviceFound READ getDeviceStatus NOTIFY deviceStateChanged)
    Q_PROPERTY(QString productName READ getProductName NOTIFY portInfoChanged)
    Q_PROPERTY(QString manufacturer READ getManufacturerName NOTIFY portInfoChanged)
    Q_PROPERTY(QString vendorID READ getVendorID NOTIFY portInfoChanged)
    Q_PROPERTY(QString productID READ getProductID NOTIFY portInfoChanged)

//    Q_PROPERTY(int portNumber READ getPortNumber WRITE setPortNumber NOTIFY portNumberChanged)
//    Q_PROPERTY(QString info READ getInfo WRITE setInfo NOTIFY infoChanged)

    struct PortDescription {
        QString productName;
        QString manufacturerName;
        QString vendorID;
        QString productID;
    };

    static const QList<QString> errorDescription;

//    struct PortSettings {
//        QString name;
//        QSerialPort::BaudRate baudRate;
//        QSerialPort::DataBits dataBits;
//        QSerialPort::Parity parity;
//        QSerialPort::StopBits stopBits;
//        QSerialPort::FlowControl flowControl;
//    };

public:
    explicit SerialPortHandler(QSettings*, QObject* parent = nullptr);
    ~SerialPortHandler();

    enum PortErrors {
        NoError = 0,
        DeviceError,
        PermissionError,
        OpenError,
        SettingsError
    };

    static PortDescription portInfo;
    static bool isDeviceFound;
    static bool isPortOpen;

    Q_INVOKABLE void scanPorts();
    Q_INVOKABLE PortErrors openSerialPort();
    Q_INVOKABLE void closeSerialPort();
//    Q_INVOKABLE const QString description(int index) const;
//    Q_INVOKABLE const QString manufacturer(int index) const;
//    Q_INVOKABLE const QString location(int index) const;
//    Q_INVOKABLE const QString vendor(int index) const;
//    Q_INVOKABLE const QString product(int index) const;

//    void handlePortError();

//    void setPortName(const QString& name);
//    void setBaudRate(QSerialPort::BaudRate baudRate);
//    void setDataBits(QSerialPort::DataBits dataBits);
//    void setParity(QSerialPort::Parity parity);
//    void setStopBits(QSerialPort::StopBits stopBits);
//    void setFlowControl(QSerialPort::FlowControl flowControl);

//    const QStringList getPortList() const { return m_portNames; };
//    const QString& getPortName() const { return portSettings.name; };
//    QSerialPort::BaudRate getBaudRate() const { return portSettings.baudRate; };
//    QSerialPort::DataBits getDataBits() const { return portSettings.dataBits; };
//    QSerialPort::Parity getParity() const { return portSettings.parity; };
//    QSerialPort::StopBits getStopBits() const { return portSettings.stopBits; };
//    QSerialPort::FlowControl getFlowControl() const { return portSettings.flowControl; };
//    QSerialPort* getPort() { return m_port; };
    bool getPortStatus() const { return SerialPortHandler::isPortOpen; };

    const QString& getProductName() const{ return SerialPortHandler::portInfo.productName; };
    const QString& getManufacturerName() const{ return SerialPortHandler::portInfo.manufacturerName; };
    const QString& getVendorID() const{ return SerialPortHandler::portInfo.vendorID; };
    const QString& getProductID() const{ return SerialPortHandler::portInfo.productID; };

    void readSettings();
    void writeSettings();

//    void refreshDevice(JNIEnv* env, jobject thiz, jobjectArray stringArrays);

//    void setPortNumber(int n) { m_portNumber = n; emit portNumberChanged(); };
//    const int getPortNumber() const { return m_portNumber; };
//    void setInfo(QString s) { info += s; info += "\n"; emit infoChanged(); };
//    const QString getInfo() const { return info; };

Q_SIGNALS:
    void portDataRead(const QByteArray&);
    void error(const QString& message);
    void success(const QString& message);
    void portListChanged();
    void portNameChanged();
    void baudRateChanged();
    void dataBitsChanged();
    void parityChanged();
    void stopBitsChanged();
    void flowControlChanged();
    void portStateChanged();
    void portInfoChanged();

private:
//    QList<QSerialPortInfo> m_portList;
//    QStringList m_portNames;
//    QSerialPort* m_port;
//    PortSettings portSettings;
    QSettings* settings;


//    PortDescription portInfo;

//    static QString info;
//    int m_portNumber;
//    QByteArray data;
};

namespace SerialPortENUM
{
Q_NAMESPACE
Q_ENUM_NS(SerialPortHandler::PortErrors)
//    Q_ENUM_NS(QSerialPort::BaudRate)
//    Q_ENUM_NS(QSerialPort::DataBits)
//    Q_ENUM_NS(QSerialPort::FlowControl)
//    Q_ENUM_NS(QSerialPort::Parity)
//    Q_ENUM_NS(QSerialPort::StopBits)
//    Q_ENUM_NS(QSerialPort::SerialPortError)
}

#endif // SERIALPORTHANDLER_H
