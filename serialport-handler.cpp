#include "serialport-handler.h"

//#include <QFile>
//#include <QQmlFile>

//#include <QDebug>

SerialPortHandler::SerialPortHandler(QObject* parent)
    : QObject(parent), m_port{nullptr}
{
    scanPorts();
//    m_port = new QSerialPort(this);
//    qDebug() << m_portList.size();
}

void SerialPortHandler::scanPorts()
{
    m_portList.clear();
    m_portNames.clear();
    m_portList = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo& portInfo : m_portList) {
        m_portNames.append(portInfo.portName());
    }
    emit portListChanged();
}

const QStringList SerialPortHandler::getPortList() const
{
    return m_portNames;
}

const QString SerialPortHandler::description(int index) const
{
    if (!(index >= 0 && index < m_portList.size())) {
        return "";
    }
    return m_portList.at(index).description();
}

const QString SerialPortHandler::manufacturer(int index) const
{
    if (!(index >= 0 && index < m_portList.size())) {
        return "";
    }
    return m_portList.at(index).manufacturer();
}

const QString SerialPortHandler::location(int index) const
{
    if (!(index >= 0 && index < m_portList.size())) {
        return "";
    }
    return m_portList.at(index).systemLocation();
}

const QString SerialPortHandler::vendor(int index) const
{
    if (!(index >= 0 && index < m_portList.size()) || !m_portList.at(index).hasVendorIdentifier()) {
        return "";
    }
    return QString(m_portList.at(index).vendorIdentifier());
}

const QString SerialPortHandler::product(int index) const
{
    if (!(index >= 0 && index < m_portList.size()) || !m_portList.at(index).hasProductIdentifier()) {
        return "";
    }
    return QString(m_portList.at(index).productIdentifier());
}

void SerialPortHandler::setDataBits(QSerialPort::DataBits dataBits)
{
    portSettings.dataBits = dataBits;
}

void SerialPortHandler::setParity(QSerialPort::Parity parity)
{
    portSettings.parity = parity;
}

void SerialPortHandler::setStopBits(QSerialPort::StopBits stopBits)
{
    portSettings.stopBits = stopBits;
}

void SerialPortHandler::setFlowControl(QSerialPort::FlowControl flowControl)
{
    portSettings.flowControl = flowControl;
}

void SerialPortHandler::setBaudRate(qint32 baudRate)
{
    portSettings.baudRate = baudRate;
}

void SerialPortHandler::setPortName(const QString& name)
{
    portSettings.name = name;
}

QSerialPort::SerialPortError SerialPortHandler::openSerialPort()
{
    if (!m_port) {
        m_port = new QSerialPort(this);
        if (!m_port) {
            emit error("Can't allocate memory.");
            return QSerialPort::DeviceNotFoundError;
        }
    }
    m_port->setPortName(portSettings.name);
    if (m_port->open(QIODevice::ReadOnly)) {
        if (!(m_port->setBaudRate(portSettings.baudRate)
              && m_port->setDataBits(portSettings.dataBits)
              && m_port->setParity(portSettings.parity)
              && m_port->setStopBits(portSettings.stopBits)
              && m_port->setFlowControl(portSettings.flowControl)))
        {
            m_port->close();
            emit error("Can't set serial port settings");
            return QSerialPort::NotOpenError;
        }
    } else {
        emit error("Can't open serial port");
        return QSerialPort::OpenError;
    }
    return QSerialPort::NoError;
}

void SerialPortHandler::closeSerialPort()
{
    if (m_port) {
        m_port->close();
    }
}
