#ifndef SERIALPORTHANDLER_H
#define SERIALPORTHANDLER_H

#include <QObject>
#include <QSettings.h>
#include <QJniObject>

namespace JavaPart {
    extern QJniObject javaPortHandler;
}

class SerialPortHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int baudRate READ getBaudRate WRITE setBaudRate NOTIFY baudRateChanged)
    Q_PROPERTY(int dataBits READ getDataBits WRITE setDataBits NOTIFY dataBitsChanged)
    Q_PROPERTY(int parity READ getParity WRITE setParity NOTIFY parityChanged)
    Q_PROPERTY(int stopBits READ getStopBits WRITE setStopBits NOTIFY stopBitsChanged)
    Q_PROPERTY(QString productName READ getProductName NOTIFY portInfoChanged)
    Q_PROPERTY(QString manufacturer READ getManufacturerName NOTIFY portInfoChanged)
    Q_PROPERTY(QString vendorID READ getVendorID NOTIFY portInfoChanged)
    Q_PROPERTY(QString productID READ getProductID NOTIFY portInfoChanged)
    Q_PROPERTY(bool portOpen READ getPortStatus NOTIFY portStateChanged)
    Q_PROPERTY(Language language READ getLanguage WRITE setLanguage NOTIFY languageChanged)

    struct PortDescription {
        QString productName;
        QString manufacturerName;
        QString vendorID;
        QString productID;
    };

    struct PortSettings {
        int baudRate;
        int dataBits;
        int parity;
        int stopBits;
    };

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

    enum Language {
        EN = 1,
        UA
    };

    static PortDescription portInfo;
    static bool isDeviceFound;
    static bool isPortOpen;

    Q_INVOKABLE void scanPorts();
    Q_INVOKABLE PortErrors openSerialPort();
    Q_INVOKABLE void closeSerialPort();

    void setPortName(const QString& name);
    void setBaudRate(int baudRate);
    void setDataBits(int dataBits);
    void setParity(int parity);
    void setStopBits(int stopBits);

    int getBaudRate() const { return portSettings.baudRate; };
    int getDataBits() const { return portSettings.dataBits; };
    int getParity() const { return portSettings.parity; };
    int getStopBits() const { return portSettings.stopBits; };

    const QString& getProductName() const{ return SerialPortHandler::portInfo.productName; };
    const QString& getManufacturerName() const{ return SerialPortHandler::portInfo.manufacturerName; };
    const QString& getVendorID() const{ return SerialPortHandler::portInfo.vendorID; };
    const QString& getProductID() const{ return SerialPortHandler::portInfo.productID; };

    bool getPortStatus() const { return SerialPortHandler::isPortOpen; };

    void setLanguage(Language);
    Language getLanguage() const { return language; };

    void writeSettings();

Q_SIGNALS:
    void portDataRead(const QByteArray&);
    void error(const QString& message);
    void success(const QString& message);

    void baudRateChanged();
    void dataBitsChanged();
    void parityChanged();
    void stopBitsChanged();

    void portInfoChanged();
    void portStateChanged();

    void languageChanged();

private:
    void readSettings();

    PortSettings portSettings;
    QSettings* settings;
    Language language;
};

namespace SerialPortENUM
{
Q_NAMESPACE
Q_ENUM_NS(SerialPortHandler::PortErrors)
Q_ENUM_NS(SerialPortHandler::Language)
}

#endif // SERIALPORTHANDLER_H
