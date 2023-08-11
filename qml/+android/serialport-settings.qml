import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import SerialPortHandler
import SerialPortENUM

ScrollView  {
    id: topItem
    contentWidth: availableWidth
    clip: true
    ColumnLayout {
        id: lay
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 5
        Text {
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 10
            text: qsTr("USB Device")
            font.pixelSize: 18
            font.bold: true
        }
        Text {
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 10
            Layout.bottomMargin: 10
            id: deviceName
            text: portHandler.productName
            font.pixelSize: 16
            Layout.columnSpan: 2
        }
        Text {
            Layout.alignment: Qt.AlignHCenter
            property string manufacturerName: portHandler.manufacturer
            text: qsTr("Manufacturer: ") + manufacturerName
            Layout.columnSpan: 2
        }
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            Text {
                property string txtID: portHandler.vendorID
                text: qsTr("Vendor ID: ") + txtID
            }
            Text {
                Layout.leftMargin: 10
                property string txtID: portHandler.productID
                text: qsTr("Product ID: ") + txtID
            }
        }
        Button {
            Layout.alignment: Qt.AlignHCenter
            Layout.bottomMargin: 20
            Layout.topMargin: 5
            text: qsTr("Refresh")
            onClicked: {
                portHandler.scanPorts();
            }
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            Layout.bottomMargin: 5
            text: qsTr("Select Parameters")
            font.pixelSize: 18
            font.bold: true
        }
        GridLayout {
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            columns: 2
            Text {
                text: qsTr("Baud rate:")
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
                text: qsTr("Data bits:")
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
                text: qsTr("Parity:")
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
                text: qsTr("Stop bits:")
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
        }

        Button {
            Layout.margins: 5
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            text: "Accept"
            onClicked: {
                let qwe = portHandler.openSerialPort();
//                if (portName.currentIndex !== -1 && !portHandler.portOpen) {
//                    portHandler.portName = portName.currentValue;
//                    portHandler.baudRate = baudRate.currentValue;
//                    portHandler.dataBits = dataBits.currentValue;
//                    portHandler.parity = parity.currentValue;
//                    portHandler.stopBits = stopBits.currentValue;
//                    portHandler.flowControl = flowControl.currentValue;
//                    let qwe = portHandler.openSerialPort();
//                    if (qwe === SerialPort.NoError) {
//                        stackView.pop()
//                        listView.currentIndex = -1
//                    }
//                }
            }
        }

//        Component.onCompleted: {
//            let temp;
//            temp = portName.indexOfValue(portHandler.portName);
//            if (temp !== -1)
//                portName.currentIndex = temp;
//            temp = baudRate.indexOfValue(portHandler.baudRate)
//            if (temp !== -1)
//                baudRate.currentIndex = temp;
//            temp = dataBits.indexOfValue(portHandler.dataBits)
//            if (temp !== -1)
//                dataBits.currentIndex = temp;
//            temp = parity.indexOfValue(portHandler.parity)
//            if (temp !== -1)
//                parity.currentIndex = temp;
//            temp = stopBits.indexOfValue(portHandler.stopBits)
//            if (temp !== -1)
//                stopBits.currentIndex = temp;
//            temp = flowControl.indexOfValue(portHandler.flowControl)
//            if (temp !== -1)
//                flowControl.currentIndex = temp;
//        }
    }
}

