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
                model: [9600, 19200, 38400, 57600, 115200]
            }
            Text {
                text: qsTr("Data bits:")
            }
            ComboBox {
                id: dataBits
                currentIndex: 3
                model: [5, 6, 7, 8]
            }
            Text {
                text: qsTr("Parity:")
            }
            ComboBox {
                id: parity
                textRole: "text"
                valueRole: "value"
                model: [
                    { value: 0, text: "None" },
                    { value: 2, text: "Even" },
                    { value: 1, text: "Odd" },
                    { value: 4, text: "Space" },
                    { value: 3, text: "Mark" }
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
                    { value: 1, text: "1" },
                    { value: 3, text: "1.5" },
                    { value: 2, text: "2" }
                ]
            }
        }

        Button {
            Layout.margins: 5
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            text: "Accept"
            onClicked: {
                portHandler.baudRate = baudRate.currentValue;
                portHandler.dataBits = dataBits.currentValue;
                portHandler.parity = parity.currentValue;
                portHandler.stopBits = stopBits.currentValue;
                if (!portHandler.portOpen) {
                    let qwe = portHandler.openSerialPort();
                    if (qwe === SerialPort.NoError) {
                        stackView.pop();
                        listView.currentIndex = -1;
                    }
                }
            }
        }

        Component.onCompleted: {
            let temp;
            temp = baudRate.indexOfValue(portHandler.baudRate)
            if (temp !== -1)
                baudRate.currentIndex = temp;
            temp = dataBits.indexOfValue(portHandler.dataBits)
            if (temp !== -1)
                dataBits.currentIndex = temp;
            temp = parity.indexOfValue(portHandler.parity)
            if (temp !== -1)
                parity.currentIndex = temp;
            temp = stopBits.indexOfValue(portHandler.stopBits)
            if (temp !== -1)
                stopBits.currentIndex = temp;
        }
    }
}

