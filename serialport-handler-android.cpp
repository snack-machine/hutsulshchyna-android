#include "serialport-handler-android.h"
#include <QDebug> // <--

SerialPortHandler::PortDescription SerialPortHandler::portInfo;
bool SerialPortHandler::isDeviceFound = false;
bool SerialPortHandler::isPortOpen = false;
const QList<QString> SerialPortHandler::errorDescription {"NoError", tr("Device not found"),
                                                         tr("Permission denied"),
                                                         tr("Port open error"),
                                                         tr("Settings setup error")};

SerialPortHandler::SerialPortHandler(QSettings* s, QObject* parent)
    : QObject(parent), settings{s}
{
    scanPorts();
    readSettings();
}

SerialPortHandler::~SerialPortHandler()
{
    writeSettings();
}

void SerialPortHandler::scanPorts()
{
    SerialPortHandler::isDeviceFound = false;
    JavaPart::javaPortHandler.callMethod<void>("refresh");
    if (!SerialPortHandler::isDeviceFound) {
        portInfo.productName = tr("No Devices");
        portInfo.manufacturerName.clear();
        portInfo.vendorID.clear();
        portInfo.productID.clear();
    }
    emit portInfoChanged();
}

SerialPortHandler::PortErrors SerialPortHandler::openSerialPort()
{
    scanPorts();
    if (!isDeviceFound) {
        emit error(tr("No USB Device"));
        return PortErrors::DeviceError;
    }
    jint result = JavaPart::javaPortHandler.callMethod<jint>("connect");
    if (result == PortErrors::NoError) {
        SerialPortHandler::isPortOpen = true;
        emit portStateChanged();
        emit success(tr("USB Device Connected"));
    }
    return PortErrors::OpenError;
}

//void SerialPortHandler::handlePortError()
//{
//    closeSerialPort();
//    emit error(tr("Critical Resource Error"));
//}

void SerialPortHandler::closeSerialPort()
{
    JavaPart::javaPortHandler.callMethod<void>("disconnect");
    SerialPortHandler::isPortOpen = false;
    emit portStateChanged();
    emit success(tr("Device Disconnected"));
}

void SerialPortHandler::readSettings()
{
//    settings->beginGroup("/serialPort");
//    portSettings.name =         settings->value("/portName", "").toString();
//    portSettings.baudRate =     static_cast<QSerialPort::BaudRate>(settings->value("/baudRate", QSerialPort::Baud9600).toInt());
//    portSettings.dataBits =     static_cast<QSerialPort::DataBits>(settings->value("/dataBits", QSerialPort::Data8).toInt());
//    portSettings.parity =       static_cast<QSerialPort::Parity>(settings->value("/parity", QSerialPort::NoParity).toInt());
//    portSettings.stopBits =     static_cast<QSerialPort::StopBits>(settings->value("/stopBits", QSerialPort::OneStop).toInt());
//    portSettings.flowControl =  static_cast<QSerialPort::FlowControl>(settings->value("/flowControl", QSerialPort::NoFlowControl).toInt());
//    settings->endGroup();
}

void SerialPortHandler::writeSettings()
{
//    settings->beginGroup("/serialPort");
//    settings->setValue("/portName",     portSettings.name);
//    settings->setValue("/baudRate",     portSettings.baudRate);
//    settings->setValue("/dataBits",     portSettings.dataBits);
//    settings->setValue("/parity",       portSettings.parity);
//    settings->setValue("/stopBits",     portSettings.stopBits);
//    settings->setValue("/flowControl",  portSettings.flowControl);
//    settings->endGroup();
}

//void SerialPortHandler::setPortName(const QString& name)
//{
//    if (portSettings.name == name)
//        return;
//    portSettings.name = name;
//    emit portNameChanged();
//}

//void SerialPortHandler::setBaudRate(QSerialPort::BaudRate baudRate)
//{
//    if (portSettings.baudRate == baudRate)
//        return;
//    portSettings.baudRate = baudRate;
//    emit baudRateChanged();
//}

//void SerialPortHandler::setDataBits(QSerialPort::DataBits dataBits)
//{
//    if (portSettings.dataBits == dataBits)
//        return;
//    portSettings.dataBits = dataBits;
//    emit dataBitsChanged();
//}

//void SerialPortHandler::setParity(QSerialPort::Parity parity)
//{
//    if (portSettings.parity == parity)
//        return;
//    portSettings.parity = parity;
//    emit parityChanged();
//}

//void SerialPortHandler::setStopBits(QSerialPort::StopBits stopBits)
//{
//    if (portSettings.stopBits == stopBits)
//        return;
//    portSettings.stopBits = stopBits;
//    emit stopBitsChanged();
//}

//void SerialPortHandler::setFlowControl(QSerialPort::FlowControl flowControl)
//{
//    if (portSettings.flowControl == flowControl)
//        return;
//    portSettings.flowControl = flowControl;
//    emit flowControlChanged();
//}

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
