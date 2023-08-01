#include "serialport-handler.h"
#include <QDebug> // <--

SerialPortHandler::SerialPortHandler(QSettings* s, QObject* parent)
    : QObject(parent), m_port{new QSerialPort(this)}, settings{s}
{
    connect(m_port, &QSerialPort::errorOccurred, this, [this](){
        if (m_port->error() == QSerialPort::ResourceError)
            handlePortError();
    });
    connect(m_port, &QSerialPort::readyRead, this, [this]() {
        QByteArray data;
        data = m_port->readAll();
        qDebug() << data;
//        while (m_port->canReadLine()) {
//            data = m_port->readLine();
//            QString line = QString::fromUtf8(data).trimmed();
//            qDebug() << "Received line:" << line;
//        }
//        QByteArray data = m_port->readAll();
        emit portDataRead(data);
    });
    scanPorts();
    readSettings();
}

SerialPortHandler::~SerialPortHandler()
{
    writeSettings();
}

void SerialPortHandler::scanPorts()
{
    m_portList.clear();
    m_portNames.clear();
    m_portList = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo& portInfo : qAsConst(m_portList)) {
        m_portNames.append(portInfo.portName());
    }
    emit portListChanged();
}

QSerialPort::SerialPortError SerialPortHandler::openSerialPort()
{
    m_port->setPortName(portSettings.name);
    if (!(m_port->setBaudRate(portSettings.baudRate)
          && m_port->setDataBits(portSettings.dataBits)
          && m_port->setParity(portSettings.parity)
          && m_port->setStopBits(portSettings.stopBits)
          && m_port->setFlowControl(portSettings.flowControl)))
    {
        emit error("Can't set serial port settings");
        return QSerialPort::NotOpenError;
    }
    if (!m_port->open(QIODevice::ReadOnly)) {
        emit error("Can't open serial port");
        return QSerialPort::OpenError;
    }
    qDebug() << portSettings.name;  // <--
    emit portStateChanged();
    return QSerialPort::NoError;
}

void SerialPortHandler::handlePortError()
{
    closeSerialPort();
    emit error(tr("Critical Resource Error"));
}

void SerialPortHandler::closeSerialPort()
{
    m_port->close();
    emit portStateChanged();
}

bool SerialPortHandler::getPortStatus() const
{
    return m_port->isOpen();
}

void SerialPortHandler::readSettings()
{
    settings->beginGroup("/serialPort");
    portSettings.name =         settings->value("/portName", "").toString();
    portSettings.baudRate =     static_cast<QSerialPort::BaudRate>(settings->value("/baudRate", QSerialPort::Baud9600).toInt());
    portSettings.dataBits =     static_cast<QSerialPort::DataBits>(settings->value("/dataBits", QSerialPort::Data8).toInt());
    portSettings.parity =       static_cast<QSerialPort::Parity>(settings->value("/parity", QSerialPort::NoParity).toInt());
    portSettings.stopBits =     static_cast<QSerialPort::StopBits>(settings->value("/stopBits", QSerialPort::OneStop).toInt());
    portSettings.flowControl =  static_cast<QSerialPort::FlowControl>(settings->value("/flowControl", QSerialPort::NoFlowControl).toInt());
    settings->endGroup();
}

void SerialPortHandler::writeSettings()
{
    settings->beginGroup("/serialPort");
    settings->setValue("/portName",     portSettings.name);
    settings->setValue("/baudRate",     portSettings.baudRate);
    settings->setValue("/dataBits",     portSettings.dataBits);
    settings->setValue("/parity",       portSettings.parity);
    settings->setValue("/stopBits",     portSettings.stopBits);
    settings->setValue("/flowControl",  portSettings.flowControl);
    settings->endGroup();
}

void SerialPortHandler::setPortName(const QString& name)
{
    if (portSettings.name == name)
        return;
    portSettings.name = name;
    emit portNameChanged();
}

void SerialPortHandler::setBaudRate(QSerialPort::BaudRate baudRate)
{
    if (portSettings.baudRate == baudRate)
        return;
    portSettings.baudRate = baudRate;
    emit baudRateChanged();
}

void SerialPortHandler::setDataBits(QSerialPort::DataBits dataBits)
{
    if (portSettings.dataBits == dataBits)
        return;
    portSettings.dataBits = dataBits;
    emit dataBitsChanged();
}

void SerialPortHandler::setParity(QSerialPort::Parity parity)
{
    if (portSettings.parity == parity)
        return;
    portSettings.parity = parity;
    emit parityChanged();
}

void SerialPortHandler::setStopBits(QSerialPort::StopBits stopBits)
{
    if (portSettings.stopBits == stopBits)
        return;
    portSettings.stopBits = stopBits;
    emit stopBitsChanged();
}

void SerialPortHandler::setFlowControl(QSerialPort::FlowControl flowControl)
{
    if (portSettings.flowControl == flowControl)
        return;
    portSettings.flowControl = flowControl;
    emit flowControlChanged();
}

//const QString SerialPortHandler::description(int index) const
//{
//    if (!(index >= 0 && index < m_portList.size())) {
//        return "";
//    }
//    return m_portList.at(index).description();
//}

//const QString SerialPortHandler::manufacturer(int index) const
//{
//    if (!(index >= 0 && index < m_portList.size())) {
//        return "";
//    }
//    return m_portList.at(index).manufacturer();
//}

//const QString SerialPortHandler::location(int index) const
//{
//    if (!(index >= 0 && index < m_portList.size())) {
//        return "";
//    }
//    return m_portList.at(index).systemLocation();
//}

//const QString SerialPortHandler::vendor(int index) const
//{
//    if (!(index >= 0 && index < m_portList.size()) || !m_portList.at(index).hasVendorIdentifier()) {
//        return "";
//    }
//    return QString(m_portList.at(index).vendorIdentifier());
//}

//const QString SerialPortHandler::product(int index) const
//{
//    if (!(index >= 0 && index < m_portList.size()) || !m_portList.at(index).hasProductIdentifier()) {
//        return "";
//    }
//    return QString(m_portList.at(index).productIdentifier());
//}
