import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ScrollView {
//    anchors.fill: parent
    property string name: pointName.text
    property string description: pointDescription.text
    contentWidth: stackView.width
    contentHeight: columnLayout.implicitHeight

    ColumnLayout {
        id: columnLayout
        anchors.horizontalCenter: parent.horizontalCenter
        GridLayout {
            columns: 6
            Layout.alignment: Qt.AlignHCenter
            Text {
                Layout.topMargin: 10
                Layout.alignment: Qt.AlignHCenter;
                Layout.columnSpan: 6
                text: qsTr("Current coordinates");
                font.pixelSize: 16
                font.bold: true
            }
            Text {
                Layout.columnSpan: 3
                property string lat: coordHandler.latitude
                text: qsTr("Latitude: ") + lat }
            Text {
                Layout.columnSpan: 3
                property string lon: coordHandler.longitude
                text: qsTr("Longitude: " + lon )
            }
            Text {
                Layout.columnSpan: 3
                property string timeValue: coordHandler.time
                text: qsTr("Time: " + timeValue)
            }
            Text {
                Layout.columnSpan: 3
                property string currentAlt: coordHandler.altitude
                text: qsTr("Altitude: " + currentAlt)
            }

            Text {
                Layout.alignment: Qt.AlignHCenter;
                Layout.columnSpan: 6
                text: qsTr("Average coordinates");
                font.pixelSize: 16
                font.bold: true
            }
            Text {
                Layout.columnSpan: 3
                property string averageLat: coordHandler.averageLat
                text: qsTr("Latitude: " + averageLat)
            }
            Text {
                Layout.columnSpan: 3
                property string averageLong: coordHandler.averageLong
                text: qsTr("Longitude: ") + averageLong
            }
            Text {
                Layout.columnSpan: 3
                Layout.bottomMargin: 10
                property string averageAlt: coordHandler.averageAlt
                text: qsTr("Altitude: ") + averageAlt
            }
        }
        RowLayout {
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
                    coordHandler.clearData();
                    toast.show("Cleared", 2000, "#222222");
                }
            }
        }
        Rectangle {
            id: map
            anchors.topMargin: 20
            Layout.fillWidth: true
            Layout.fillHeight: true
            implicitWidth: 300
            implicitHeight: 300
            color: "blue"
            Image {
                anchors.fill: parent // Fill the entire rectangle
                source: "qrc:/hutsulshchyna-android/icons/coords.png" // Replace with the actual path to your image
                fillMode: Image.PreserveAspectFit // Adjust the fill mode as needed
            }
        }
        TextField {
            id: pointName
            Layout.fillWidth: true
            font.pixelSize: 18
            placeholderText: qsTr("Enter point name")
        }
        TextField {
            id: pointDescription
            Layout.fillWidth: true
            font.pixelSize: 18
            placeholderText: qsTr("Enter point description")
        }

        Button {
            Layout.margins: 5
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            text: "Save to file"
            onClicked: {
                saveDialog.open()
            }
        }
    }
}
