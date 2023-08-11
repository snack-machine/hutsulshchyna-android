import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ScrollView {
    property string name: pointName.text
    property string description: pointDescription.text
    property bool userConfirmed: false
    contentWidth: stackView.width
    contentHeight: columnLayout.implicitHeight
    GridLayout {
        id: columnLayout
        anchors.fill: parent
        columns: 2
        Text {
            Layout.topMargin: 10
            Layout.alignment: Qt.AlignHCenter;
            Layout.columnSpan: 2
            text: qsTr("Current coordinates");
            font.pixelSize: 16
            font.bold: true
        }
        Text {
            Layout.leftMargin: 20
            property string lat: coordHandler.latitude
            text: qsTr("Latitude: ") + lat }
        Text {
            property string lon: coordHandler.longitude
            text: qsTr("Longitude: " + lon )
        }
        Text {
            Layout.leftMargin: 20
            property string timeValue: coordHandler.time
            text: qsTr("Time: " + timeValue)
        }
        Text {
            property string currentAlt: coordHandler.altitude
            text: qsTr("Altitude: " + currentAlt)
        }

        Text {
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignHCenter;
            text: qsTr("Average coordinates");
            font.pixelSize: 16
            font.bold: true
        }
        Text {
            Layout.leftMargin: 20
            property string averageLat: coordHandler.averageLat
            text: qsTr("Latitude: " + averageLat)
        }
        Text {
            property string averageLong: coordHandler.averageLong
            text: qsTr("Longitude: ") + averageLong
        }
        Text {
            Layout.leftMargin: 20
            Layout.bottomMargin: 10
            property string averageAlt: coordHandler.averageAlt
            text: qsTr("Altitude: ") + averageAlt
        }
        RowLayout {
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignHCenter
            Button {
                id: setCenterButton
                text: qsTr("Set Center")
                onClicked: {
                    toast.show("Sentered", 2000, "#222222");
                }
            }
//            Text { text: qsTr("Scale") }
//            SpinBox {
//                from: 1
//                to: 30
//            }
            Button {
                id: clearButton
                text: qsTr("Clear")
                onClicked: {
                    popup.open();
                }
            }
        }
        Image {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter
            Layout.columnSpan: 2
            source: "qrc:/hutsulshchyna-android/icons/coords.png"
            fillMode: Image.PreserveAspectFit
        }
        TextField {
            id: pointName
            Layout.alignment: Qt.AlignHCenter
            Layout.columnSpan: 2
            Layout.leftMargin: 20
            Layout.rightMargin: 20
            Layout.fillWidth: true
            font.pixelSize: 18
            placeholderText: qsTr("Enter point name")
        }
        TextField {
            id: pointDescription
            Layout.alignment: Qt.AlignHCenter
            Layout.columnSpan: 2
            Layout.leftMargin: 20
            Layout.rightMargin: 20
            Layout.fillWidth: true
            font.pixelSize: 18
            placeholderText: qsTr("Enter point description")
        }

        Button {
            Layout.alignment: Qt.AlignHCenter
            Layout.columnSpan: 2
            Layout.margins: 5
            text: qsTr("Save to file")
            onClicked: {
                saveDialog.open()
            }
        }

        Popup {
            id: popup
            modal: true
            focus: true
            x: (parent.width - implicitWidth) / 2
            y: (parent.height - implicitHeight) / 2
            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

            Rectangle {
                implicitWidth: column.implicitWidth + 10
                implicitHeight: column.implicitHeight + 10
                color: "white"
                Column {
                    id: column
                    spacing: 10
                    anchors.centerIn: parent
                    Text {
                        text: "Clear received coordinates?"
                        font.pixelSize: 18
                        color: "black"
                    }
                    Row {
                        spacing: 10
                        anchors.right: parent.right
                        Layout.alignment: Qt.AlignRight
                        Button {
                            Layout.alignment: Qt.AlignRight
                            text: qsTr("No")
                            onClicked: {
                                popup.close()
                            }
                        }
                        Button {
                            Layout.alignment: Qt.AlignRight
                            text: qsTr("Yes")
                            onClicked: {
                                coordHandler.clearData();
                                pointName.text = "";
                                pointDescription.text = "";
                                toast.show("Cleared", 2000, "#222222");
                                popup.close()
                            }
                        }
                    }
                }
            }
        }
    }
}
