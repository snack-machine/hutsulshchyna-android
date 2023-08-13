#ifndef SERIALPORTHANDLER_H
#define SERIALPORTHANDLER_H

#include <QObject>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QSettings.h>

#ifdef Q_OS_ANDROID
#include <QJniObject>

namespace JavaPart {
    extern QJniObject javaPortHandler;
}
#endif

namespace SerialPortENUM
{
    Q_NAMESPACE
    Q_ENUM_NS(QSerialPort::BaudRate)
    Q_ENUM_NS(QSerialPort::DataBits)
    Q_ENUM_NS(QSerialPort::FlowControl)
    Q_ENUM_NS(QSerialPort::Parity)
    Q_ENUM_NS(QSerialPort::StopBits)
    Q_ENUM_NS(QSerialPort::SerialPortError)
}

class SerialPortHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList ports READ getPortList NOTIFY portListChanged)
    Q_PROPERTY(QString portName READ getPortName WRITE setPortName NOTIFY portNameChanged)
    Q_PROPERTY(QSerialPort::BaudRate baudRate READ getBaudRate WRITE setBaudRate NOTIFY baudRateChanged)
    Q_PROPERTY(QSerialPort::DataBits dataBits READ getDataBits WRITE setDataBits NOTIFY dataBitsChanged)
    Q_PROPERTY(QSerialPort::Parity parity READ getParity WRITE setParity NOTIFY parityChanged)
    Q_PROPERTY(QSerialPort::StopBits stopBits READ getStopBits WRITE setStopBits NOTIFY stopBitsChanged)
    Q_PROPERTY(QSerialPort::FlowControl flowControl READ getFlowControl WRITE setFlowControl NOTIFY flowControlChanged)
    Q_PROPERTY(bool portOpen READ getPortStatus NOTIFY portStateChanged)
    Q_PROPERTY(Language language READ getLanguage WRITE setLanguage NOTIFY languageChanged)

    struct PortSettings {
        QString name;
        QSerialPort::BaudRate baudRate;
        QSerialPort::DataBits dataBits;
        QSerialPort::Parity parity;
        QSerialPort::StopBits stopBits;
        QSerialPort::FlowControl flowControl;
    };

public:
    enum Language {
        EN = 1,
        UA
    };

    explicit SerialPortHandler(QSettings*, QObject* parent = nullptr);
    ~SerialPortHandler();

    Q_INVOKABLE void scanPorts();
    Q_INVOKABLE QSerialPort::SerialPortError openSerialPort();
    Q_INVOKABLE void closeSerialPort();
//    Q_INVOKABLE const QString description(int index) const;
//    Q_INVOKABLE const QString manufacturer(int index) const;
//    Q_INVOKABLE const QString location(int index) const;
//    Q_INVOKABLE const QString vendor(int index) const;
//    Q_INVOKABLE const QString product(int index) const;

    void handlePortError();

    void setPortName(const QString& name);
    void setBaudRate(QSerialPort::BaudRate baudRate);
    void setDataBits(QSerialPort::DataBits dataBits);
    void setParity(QSerialPort::Parity parity);
    void setStopBits(QSerialPort::StopBits stopBits);
    void setFlowControl(QSerialPort::FlowControl flowControl);

    const QStringList getPortList() const { return m_portNames; };
    const QString& getPortName() const { return portSettings.name; };
    QSerialPort::BaudRate getBaudRate() const { return portSettings.baudRate; };
    QSerialPort::DataBits getDataBits() const { return portSettings.dataBits; };
    QSerialPort::Parity getParity() const { return portSettings.parity; };
    QSerialPort::StopBits getStopBits() const { return portSettings.stopBits; };
    QSerialPort::FlowControl getFlowControl() const { return portSettings.flowControl; };
    QSerialPort* getPort() { return m_port; };
    bool getPortStatus() const;

    void setLanguage(Language);
    Language getLanguage() const { return language; };

    void readSettings();
    void writeSettings();

Q_SIGNALS:
    void portDataRead(const QByteArray&);
    void error(const QString &message);
    void success(const QString& message);
    void portListChanged();
    void portNameChanged();
    void baudRateChanged();
    void dataBitsChanged();
    void parityChanged();
    void stopBitsChanged();
    void flowControlChanged();
    void portStateChanged();
    void languageChanged();

private:
    QList<QSerialPortInfo> m_portList;
    QStringList m_portNames;
    QSerialPort* m_port;
    PortSettings portSettings;
    QSettings* settings;
    Language language;
};

#endif // SERIALPORTHANDLER_H
