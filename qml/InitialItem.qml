import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ScrollView {
    property string name: pointName.text
    property string description: pointDescription.text
    contentWidth: stackView.width
    contentHeight: columnLayout.implicitHeight
    ColumnLayout {
        id: columnLayout
        anchors.horizontalCenter: parent.horizontalCenter
        Item {
            height: 10
        }
        GridLayout {
            columns: 6
            Layout.alignment: Qt.AlignHCenter
            Text { text: qsTr("Current coordinates"); Layout.alignment: Qt.AlignHCenter; Layout.columnSpan: 6}
            Text { text: qsTr("Latitude:") }
            Text { id: currentLat; text: coordHandler.latitude }
            Text { text: qsTr("Longitude:") }
            Text { id: currentLong; text: coordHandler.longitude }
            Text { text: qsTr("Altitude:") }
            Text { id: currentAlt; text: coordHandler.altitude }
            Text { text: qsTr("Time:") }
            Text { id: timeValue; text: coordHandler.time }
            Text { text: qsTr("Satelits:") }
            Text { id: sataleits; text: coordHandler.satelits }
            Text { text: qsTr("RMSD:") }
            Text { id: rmsd; text: "0" }
    //                    Text { id: positionModeStatus; text: "No data"; Layout.columnSpan: 4 }

            Text { text: qsTr("Average coordinates"); Layout.alignment: Qt.AlignHCenter; Layout.columnSpan: 6}
            Text { text: qsTr("Latitude:") }
            Text { id: averageLat; text: coordHandler.averageLat }
            Text { text: qsTr("Longitude:") }
            Text { id: averageLong; text: coordHandler.averageLong }
            Text { text: qsTr("Altitude:")}
            Text { id: averageAlt; text: coordHandler.averageAlt}
        }
        Item {
            height: 10
        }
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            Button {
                id: setCenterButton
                text: qsTr("Set Center")
                onClicked: {}
            }
            Text { text: qsTr("Scale") }
            SpinBox {
                from: 1
                to: 30
            }
            Button {
                id: clearButton
                text: qsTr("Clear")
                onClicked: {}
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

        RoundButton {
            Layout.margins: 5
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            text: "Save to file"
            onClicked: {
                saveDialog.open()
            }
        }
    }
}
