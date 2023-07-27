import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SerialPortHandler
import SerialPortENUM

ScrollView  {
    id: page

    ListModel {
        id: settingsModel
        ListElement { settingName: "Baud Rate"; settingValue: 9600 }
        ListElement { settingName: "Data Bits"; settingValue: 8 }
        ListElement { settingName: "Parity"; settingValue: "None" }
        // Add more settings as needed
    }

    Popup {
        id: settingsPopup
        width: 200
        height: 200
        modal: true
        contentItem: Column {
            spacing: 10
            anchors.centerIn: parent

            Repeater {
                model: settingName === "Baud Rate" ? [9600, 115200] : (settingName === "Data Bits" ? [8, 7] : ["None", "Even", "Odd"])

                Button {
                    text: settingName === "Baud Rate" ? modelData : (settingName === "Data Bits" ? modelData : modelData)
                    onClicked: {
                        settingValue = modelData
                        settingsPopup.close()
                    }
                }
            }
        }
    }

    ListView {
        id: settingsListView

        focus: true
        currentIndex: -1
        anchors {
            top: drawerHeader.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        delegate: ItemDelegate {
            width: listView.width
            text: model.title
            highlighted: ListView.isCurrentItem
            onClicked: {
                listView.currentIndex = index
                stackView.push(model.source)
                drawer.close()
            }
        }

        model: ListModel {
            ListElement { title: qsTr("Port Settings"); source: "qrc:/hutsulshchyna-android/qml/serialport-settings.qml" }
            ListElement { title: qsTr("New Port Settings"); source: "qrc:/hutsulshchyna-android/qml/serialport-settings-new.qml" }
        }

        ScrollIndicator.vertical: ScrollIndicator { }
    }
}
