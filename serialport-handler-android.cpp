#include "serialport-handler-android.h"
//#include <QDebug>

SerialPortHandler::PortDescription SerialPortHandler::portInfo;
bool SerialPortHandler::isDeviceFound = false;
bool SerialPortHandler::isPortOpen = false;

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
    jint result = JavaPart::javaPortHandler.callMethod<jint>("connect", "(IIII)I",
                                                             portSettings.baudRate, portSettings.dataBits,
                                                             portSettings.stopBits, portSettings.parity);
    if (result == PortErrors::NoError) {
        SerialPortHandler::isPortOpen = true;
        emit portStateChanged();
        emit success(tr("USB Device Connected"));
    }
    return PortErrors::OpenError;
}

void SerialPortHandler::closeSerialPort()
{
    JavaPart::javaPortHandler.callMethod<void>("disconnect");
    SerialPortHandler::isPortOpen = false;
    emit portStateChanged();
    emit success(tr("Device Disconnected"));
}

void SerialPortHandler::readSettings()
{
    settings->beginGroup("/serialPort");
    portSettings.baudRate = (settings->value("/baudRate", 9600).toInt());
    portSettings.dataBits = (settings->value("/dataBits", 8).toInt());
    portSettings.parity =   (settings->value("/parity", 0).toInt());
    portSettings.stopBits = (settings->value("/stopBits", 1).toInt());
    settings->endGroup();
}

void SerialPortHandler::writeSettings()
{
    settings->beginGroup("/serialPort");
    settings->setValue("/baudRate", portSettings.baudRate);
    settings->setValue("/dataBits", portSettings.dataBits);
    settings->setValue("/parity",   portSettings.parity);
    settings->setValue("/stopBits", portSettings.stopBits);
    settings->endGroup();
    settings->setValue("language", language);
}

void SerialPortHandler::setBaudRate(int baudRate)
{
    if (portSettings.baudRate == baudRate)
        return;
    portSettings.baudRate = baudRate;
    emit baudRateChanged();
}

void SerialPortHandler::setDataBits(int dataBits)
{
    if (portSettings.dataBits == dataBits)
        return;
    portSettings.dataBits = dataBits;
    emit dataBitsChanged();
}

void SerialPortHandler::setParity(int parity)
{
    if (portSettings.parity == parity)
        return;
    portSettings.parity = parity;
    emit parityChanged();
}

void SerialPortHandler::setStopBits(int stopBits)
{
    if (portSettings.stopBits == stopBits)
        return;
    portSettings.stopBits = stopBits;
    emit stopBitsChanged();
}

void SerialPortHandler::setLanguage(SerialPortHandler::Language lang)
{
    if (language == lang)
        return;
    language = lang;
    emit languageChanged();
}
