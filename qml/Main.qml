import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

ApplicationWindow {
    id: window
    width: 400
    height: 600
    visible: true
    title: "Hutsulschyna"

    Action {
        id: navigateBackAction
        icon.name: stackView.depth > 1 ? "back" : "drawer"
        icon.source: stackView.depth > 1 ? "qrc:/hutsulshchyna-android/icons/back.png" : "qrc:/hutsulshchyna-android/icons/drawer.png"
        onTriggered: {
            if (stackView.depth > 1) {
                stackView.pop()
                listView.currentIndex = -1
            } else {
                drawer.open()
            }
        }
    }

    Action {
        id: openSerialPortAction
        icon.source: "qrc:/hutsulshchyna-android/icons/disconnect.png"
        onTriggered: {}
    }

    Action {
        id: settingsSerialPortAction
        icon.name: "settings"
        icon.source: "qrc:/hutsulshchyna-android/icons/settings.png"
        onTriggered: {
            stackView.push("qrc:/hutsulshchyna-android/qml/serialport-settings.qml")
//            drawer.close()
        }
    }

    Action {
        id: optionsMenuAction
        icon.name: "menu"
        icon.source: "qrc:/hutsulshchyna-android/icons/menu.png"
        onTriggered: {}
    }

    header: ToolBar {
        RowLayout {
            spacing: 5
            anchors.fill: parent

            ToolButton {
                action: navigateBackAction
            }

            Item {
                width: 20
            }

            Label {
                id: titleLabel
                text: listView.currentItem ? listView.currentItem.text : qsTr("Coordinates")
                font.pixelSize: 20
                Layout.fillWidth: true
            }
            ToolButton {
                action: openSerialPortAction
            }
            ToolButton {
                action: settingsSerialPortAction
            }
            Item {
                width: 20
            }

            ToolButton {
                action: optionsMenuAction
            }
        }
    }

    Drawer {
        id: drawer
        width: Math.min(window.width, window.height) / 3 * 2
        height: window.height
        interactive: stackView.depth === 1

        Rectangle {
            id: drawerHeader
            height: parent.height * 0.2
            width: parent.width
            color: "green"
        }

        ListView {
            id: listView

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

    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: Pane {
            id: pane
            ColumnLayout {
                anchors.horizontalCenter: parent.horizontalCenter
                GridLayout {
                    columns: 6
                    Layout.alignment: Qt.AlignHCenter
                    Text { text: qsTr("Current coordinates"); Layout.alignment: Qt.AlignHCenter; Layout.columnSpan: 6}
                    Text { text: qsTr("Latitude:") }
                    Text { id: currentLat; text: "0" }
                    Text { text: qsTr("Longitude:") }
                    Text { id: currentLong; text: "0" }
                    Text { text: qsTr("Altitude:") }
                    Text { id: currentAlt; text: "0" }
                    Text { text: qsTr("Time:") }
                    Text { id: timeValue; text: "0" }
                    Text { text: qsTr("RMSD:") }
                    Text { id: rmsd; text: "0" }
                    Text { text: qsTr("Satelits:") }
                    Text { id: sataleits; text: "0" }
//                    Text { id: positionModeStatus; text: "No data"; Layout.columnSpan: 4 }

                    Text { text: qsTr("Average coordinates"); Layout.alignment: Qt.AlignHCenter; Layout.columnSpan: 6}
                    Text { text: qsTr("Latitude:") }
                    Text { id: averageLat; text: "0" }
                    Text { text: qsTr("Longitude:") }
                    Text { id: averageLong; text: "0" }
                    Text { text: qsTr("Altitude:")}
                    Text { id: averageAlt; text: "0"}
                }
                Item {
                    height: 10
                }
                RowLayout {
                    Layout.alignment: Qt.AlignHCenter
                    Button {
                        id: setCenter
                        text: qsTr("Set Center")
                        onClicked: {}
                        Layout.columnSpan: 2
                    }
                    Text { text: qsTr("Scale") }
                    SpinBox {
                        from: 1
                        to: 30
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
                Rectangle {
                    Layout.fillWidth: true
                    implicitHeight: pointDescription.implicitHeight
                    TextInput {
                        id: pointDescription
                        clip: true
                        anchors.left: parent.left
                        anchors.right: parent.right
                        font.pixelSize: 18
                        text: "Enter your text here..."
                        selectByMouse: true
                    }
                }
                RoundButton {
                    Layout.margins: 5
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    text: "Save to file"
                    onClicked: {}
                }
            }
        }
    }
}
