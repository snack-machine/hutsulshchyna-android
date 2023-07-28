#ifndef SERIALPORTHANDLER_H
#define SERIALPORTHANDLER_H

#include <QObject>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QPointer>
#include <qqml.h>
#include <QSettings.h>

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
    Q_PROPERTY(QString name READ getPortName WRITE setPortName NOTIFY portNameChanged)
    Q_PROPERTY(QSerialPort::BaudRate baudRate READ getBaudRate WRITE setBaudRate NOTIFY baudRateChanged)
    Q_PROPERTY(QSerialPort::DataBits dataBits READ getDataBits WRITE setDataBits NOTIFY dataBitsChanged)
    Q_PROPERTY(QSerialPort::Parity parity READ getParity WRITE setParity NOTIFY parityChanged)
    Q_PROPERTY(QSerialPort::StopBits stopBits READ getStopBits WRITE setStopBits NOTIFY stopBitsChanged)
    Q_PROPERTY(QSerialPort::FlowControl flowControl READ getFlowControl WRITE setFlowControl NOTIFY flowControlChanged)
    QML_ELEMENT

    struct PortSettings {
        QString name;
        QSerialPort::BaudRate baudRate;
        QSerialPort::DataBits dataBits;
        QSerialPort::Parity parity;
        QSerialPort::StopBits stopBits;
        QSerialPort::FlowControl flowControl;
        bool localEchoEnabled;
    };

public:
    explicit SerialPortHandler(QSettings*, QObject* parent = nullptr);
    ~SerialPortHandler();

    Q_INVOKABLE void scanPorts();
    Q_INVOKABLE QSerialPort::SerialPortError openSerialPort();
    Q_INVOKABLE void closeSerialPort() const /*{ m_port->close(); }*/;

    void handlePortError();

    const QStringList getPortList() const { return m_portNames; };
    Q_INVOKABLE const QString description(int index) const;
    Q_INVOKABLE const QString manufacturer(int index) const;
    Q_INVOKABLE const QString location(int index) const;
    Q_INVOKABLE const QString vendor(int index) const;
    Q_INVOKABLE const QString product(int index) const;

    void setPortName(const QString& name);
    void setBaudRate(QSerialPort::BaudRate baudRate);
    void setDataBits(QSerialPort::DataBits dataBits);
    void setParity(QSerialPort::Parity parity);
    void setStopBits(QSerialPort::StopBits stopBits);
    void setFlowControl(QSerialPort::FlowControl flowControl);

    const QString& getPortName() const { return portSettings.name; };
    QSerialPort::BaudRate getBaudRate() const { return portSettings.baudRate; };
    QSerialPort::DataBits getDataBits() const { return portSettings.dataBits; };
    QSerialPort::Parity getParity() const { return portSettings.parity; };
    QSerialPort::StopBits getStopBits() const { return portSettings.stopBits; };
    QSerialPort::FlowControl getFlowControl() const { return portSettings.flowControl; };

    void readSettings();
    void writeSettings();

Q_SIGNALS:
    void portDataRead(const QByteArray&);
    void error(const QString &message);
    void portListChanged();
    void portNameChanged();
    void baudRateChanged();
    void dataBitsChanged();
    void parityChanged();
    void stopBitsChanged();
    void flowControlChanged();

private:
    QList<QSerialPortInfo> m_portList;
    QStringList m_portNames;
    QSerialPort* m_port;
    PortSettings portSettings;
    QSettings* settings;
    QByteArray data;
};

#endif // SERIALPORTHANDLER_H
