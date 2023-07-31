import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import CoordHandler
import SerialPortHandler

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
        icon.source: portHandler.portOpen ? "qrc:/hutsulshchyna-android/icons/connected.png" : "qrc:/hutsulshchyna-android/icons/disconnected.png"
        onTriggered: {
            portHandler.portOpen ? portHandler.closeSerialPort() : portHandler.openSerialPort()
        }
    }

    Action {
        id: settingsSerialPortAction
        icon.name: "settings"
        icon.source: "qrc:/hutsulshchyna-android/icons/settings.png"
        onTriggered: {
            if (stackView.depth === 1){
                listView.currentIndex = 0
                stackView.push("qrc:/hutsulshchyna-android/qml/serialport-settings.qml")
            }
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

        ListView {
            id: listView

            focus: true
            currentIndex: -1
            anchors.fill: parent
            header: Component {
                Rectangle {
                    id: drawerHeader
                    height: drawer.height * 0.2
                    width: parent.width
                    color: "green"
                }
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
//                ListElement { title: qsTr("Old Port Settings"); source: "qrc:/hutsulshchyna-android/qml/serialport-settings-old.qml" }
                ListElement { title: qsTr("Port Settings"); source: "qrc:/hutsulshchyna-android/qml/serialport-settings.qml" }
//                ListElement { title: qsTr("New Port Settings"); source: "qrc:/hutsulshchyna-android/qml/serialport-settings-new.qml" }
                ListElement { title: qsTr("Terminal"); source: "qrc:/hutsulshchyna-android/qml/terminal.qml" }
            }

            ScrollIndicator.vertical: ScrollIndicator { }
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: ScrollView {
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
                    Layout.fillWidth: true
                    font.pixelSize: 18
                    placeholderText: qsTr("Enter point description")
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
