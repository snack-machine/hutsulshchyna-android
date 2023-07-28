import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import SerialPortHandler
import SerialPortENUM

ScrollView  {
    id: topItem
//    clip: true
//    contentWidth: availableWidth
//    signal portOpened(string result)

//    Connections {
//        target: window

//        function onClosePort() {
//            portHandler.closeSerialPort();
//        }
//    }
    ColumnLayout {
        id: lay
        spacing: 5
        anchors.centerIn: parent
        SerialPortHandler {
            id: portHandler
            onError: (msg)=> {
                console.log(msg);
            }
        }
        RowLayout {
            Layout.margins: 10
            Layout.alignment: Qt.AlignHCenter
    //        Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                Layout.alignment: Qt.AlignTop
                Layout.rightMargin: 20
                Text {
                    Layout.bottomMargin: 5
                    text: "Select Serial Port:"
                }
                RowLayout {
                    ComboBox {
                        id: portName
                        model: portHandler.ports
                    }
                    Button {
                        text: "Refresh"
                        onClicked: {
                            portHandler.scanPorts();
                        }
                    }
                }
                Text {
                    id: description
                    text: "Description: " + portHandler.description((portName.currentIndex))
                }
                Text {
                    id: manufacturer
                    text: "Manufacturer: " + portHandler.manufacturer((portName.currentIndex))
                }
                Text {
                    id: location
                    text: "Location: " + portHandler.location((portName.currentIndex))
                }
                Text {
                    id: vendor
                    text: "Vendor ID: " + portHandler.vendor((portName.currentIndex))
                }
                Text {
                    id: product
                    text: "Product ID: " + portHandler.product((portName.currentIndex))
                }
            }
            ColumnLayout {
                Layout.alignment: Qt.AlignTop
                Text {
                    Layout.bottomMargin: 5
                    text: "Select Parameters:"
                }
                GridLayout {
                    Layout.fillWidth: true
                    columns: 2
                    Text {
                        text: "Baud rate:"
                    }
                    ComboBox {
                        id: baudRate
                        textRole: "text"
                        valueRole: "value"
                        model: [
                            { value: SerialPort.Baud9600, text: "9600" },
                            { value: SerialPort.Baud19200, text: "19200" },
                            { value: SerialPort.Baud38400, text: "38400" },
                            { value: SerialPort.Baud57600, text: "57600" },
                            { value: SerialPort.Baud115200, text: "115200" }
                        ]
                    }
                    Text {
                        text: "Data bits:"
                    }
                    ComboBox {
                        id: dataBits
                        textRole: "text"
                        valueRole: "value"
                        currentIndex: 3
                        model: [
                            { value: SerialPort.Data5, text: "5" },
                            { value: SerialPort.Data6, text: "6" },
                            { value: SerialPort.Data7, text: "7" },
                            { value: SerialPort.Data8, text: "8" }
                        ]
                    }
                    Text {
                        text: "Parity:"
                    }
                    ComboBox {
                        id: parity
                        textRole: "text"
                        valueRole: "value"
                        model: [
                            { value: SerialPort.NoParity, text: "None" },
                            { value: SerialPort.EvenParity, text: "Even" },
                            { value: SerialPort.OddParity, text: "Odd" },
                            { value: SerialPort.SpaceParity, text: "Space" },
                            { value: SerialPort.MarkParity, text: "Mark" }
                        ]
                    }
                    Text {
                        text: "Stop bits:"
                    }
                    ComboBox {
                        id: stopBits
                        textRole: "text"
                        valueRole: "value"
                        model: [
                            { value: SerialPort.OneStop, text: "1" },
                            { value: SerialPort.OneAndHalfStop, text: "1.5" },
                            { value: SerialPort.TwoStop, text: "2" }
                        ]
                    }
                    Text {
                        text: "Flow control:"
                    }
                    ComboBox {
                        id: flowControl
                        textRole: "text"
                        valueRole: "value"
                        model: [
                            { value: SerialPort.NoFlowControl, text: "None" },
                            { value: SerialPort.HardwareControl, text: "RTS/CTS" },
                            { value: SerialPort.SoftwareControl, text: "XON/XOFF" }
                        ]
                    }
                }
            }
        }
    //    CheckBox {
    //        id: echoBox
    //        Layout.margins: 10
    //        checked: true
    //        text: "Local echo"
    //    }

        RoundButton {
            Layout.margins: 5
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            text: "Accept"
            onClicked: {
                if (portName.currentIndex !== -1) {
                    portHandler.setPortName(portName.currentValue);
                    portHandler.setBaudRate(baudRate.currentValue);
                    portHandler.setDataBits(dataBits.currentValue);
                    portHandler.setParity(parity.currentValue);
                    portHandler.setStopBits(stopBits.currentValue);
                    portHandler.setFlowControl(flowControl.currentValue);
                    var qwe = portHandler.openSerialPort();
                    if (qwe === SerialPort.NoError) {
                        stackView.pop();
                        topItem.portOpened(portName.currentValue);
                    }
                }
            }
        }
        Item {
            Layout.fillHeight: true
        }
    }
}
