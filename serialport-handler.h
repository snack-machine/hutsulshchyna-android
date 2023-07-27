#ifndef SERIALPORTHANDLER_H
#define SERIALPORTHANDLER_H

#include <QObject>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QPointer>
#include <qqml.h>

namespace SerialPortENUM
{
Q_NAMESPACE
    Q_ENUM_NS(QSerialPort::DataBits)
    Q_ENUM_NS(QSerialPort::BaudRate)
    Q_ENUM_NS(QSerialPort::FlowControl)
    Q_ENUM_NS(QSerialPort::Parity)
    Q_ENUM_NS(QSerialPort::StopBits)
    Q_ENUM_NS(QSerialPort::SerialPortError)
}

class SerialPortHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList ports READ getPortList NOTIFY portListChanged)
    QML_ELEMENT

    struct PortSettings {
        QString name;
        qint32 baudRate;
        QSerialPort::DataBits dataBits;
        QSerialPort::Parity parity;
        QSerialPort::StopBits stopBits;
        QSerialPort::FlowControl flowControl;
        bool localEchoEnabled;
    };

public:
    explicit SerialPortHandler(QObject* parent = nullptr);

    Q_INVOKABLE void scanPorts();
    Q_INVOKABLE QSerialPort::SerialPortError openSerialPort();
    Q_INVOKABLE void closeSerialPort();

    const QStringList getPortList() const;
    Q_INVOKABLE const QString description(int index) const;
    Q_INVOKABLE const QString manufacturer(int index) const;
    Q_INVOKABLE const QString location(int index) const;
    Q_INVOKABLE const QString vendor(int index) const;
    Q_INVOKABLE const QString product(int index) const;

    Q_INVOKABLE void setPortName(const QString& name);
    Q_INVOKABLE void setBaudRate(qint32 baudRate);
    Q_INVOKABLE void setDataBits(QSerialPort::DataBits dataBits);
    Q_INVOKABLE void setParity(QSerialPort::Parity parity);
    Q_INVOKABLE void setStopBits(QSerialPort::StopBits stopBits);
    Q_INVOKABLE void setFlowControl(QSerialPort::FlowControl flowControl);

Q_SIGNALS:
    void error(const QString &message);
    void portListChanged();

private:
    QList<QSerialPortInfo> m_portList;
    QStringList m_portNames;
    QPointer<QSerialPort> m_port;
    PortSettings portSettings;
};

#endif // SERIALPORTHANDLER_H
