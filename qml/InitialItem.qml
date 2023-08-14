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
            text: qsTr("Latitude: ") + lat
        }
        Text {
            property string lon: coordHandler.longitude
            text: qsTr("Longitude: ") + lon
        }
        Text {
            Layout.leftMargin: 20
            property string timeValue: coordHandler.time
            text: qsTr("Time: ") + timeValue
        }
        Text {
            property string currentAlt: coordHandler.altitude
            text: qsTr("Altitude: ") + currentAlt
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
        Button {
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignHCenter;
            id: clearButton
            text: qsTr("Clear")
            onClicked: {
                clearDialog.open();
            }
        }
//        RowLayout {
//            Layout.columnSpan: 2
//            Layout.alignment: Qt.AlignHCenter
//            Button {
//                id: setCenterButton
//                text: qsTr("Set Center")
//                onClicked: {
//                    toast.show(qsTr("Sentered"), 2000, "#222222");
//                }
//            }
////            Text { text: qsTr("Scale") }
////            SpinBox {
////                from: 1
////                to: 30
////            }
//            Button {
//                id: clearButton
//                text: qsTr("Clear")
//                onClicked: {
//                    clearDialog.open();
//                }
//            }
//        }
//        Image {
//            Layout.fillWidth: true
//            Layout.alignment: Qt.AlignHCenter
//            Layout.columnSpan: 2
//            source: "qrc:/hutsulshchyna-android/icons/coords.png"
//            fillMode: Image.PreserveAspectFit
//        }
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

        Dialog {
            id: clearDialog
            x: Math.round((window.width - width) / 2)
            y: Math.round(window.height / 6)
            width: Math.round(Math.min(window.width, window.height) / 3 * 2)
            modal: true
            focus: true
            title: qsTr("Clear")


            standardButtons: Dialog.Ok | Dialog.Cancel
            onAccepted: {
                coordHandler.clearData();
                pointName.text = "";
                pointDescription.text = "";
                toast.show(qsTr("Cleared"), 2000, "#222222");
                clearDialog.close()
            }
            onRejected: {
                clearDialog.close();
            }

            contentItem: ColumnLayout {
                id: settingsColumn
                spacing: 20
                Label {
                    text: qsTr("Clear received coordinates?")
                    font.pixelSize: 18
                }
            }
        }
    }
}
