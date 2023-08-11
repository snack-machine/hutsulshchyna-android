import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import SerialPortHandler
import CoordHandler
import FileHandler

ApplicationWindow {
    id: window
    width: 400
    height: 640
    visible: true
    title: "Hutsulschyna"

    Connections {
        target: fileHandler
        function onError(msg) {
            toast.show(msg, 3000, "#FF0000");
        }
        function onSuccess(msg) {
            toast.show(msg, 3000, "#50C878");
        }
    }
    Connections {
        target: portHandler
        function onError(msg) {
            toast.show(msg, 3000, "#FF0000");
        }
        function onSuccess(msg) {
            toast.show(msg, 3000, "#50C878");
        }
    }

    Action {
        id: navigateBackAction
        icon.name: stackView.depth > 1 ? "back" : "drawer"
        icon.source: stackView.depth > 1 ? "qrc:/hutsulshchyna-android/icons/arrow-back.svg" : "qrc:/hutsulshchyna-android/icons/menu-burger.svg"
        onTriggered: {
            if (stackView.depth > 1) {
                stackView.pop();
                listView.currentIndex = -1;
            } else {
                drawer.open();
            }
        }
    }

    Action {
        id: openSerialPortAction
        icon.source: portHandler.portOpen ? "qrc:/hutsulshchyna-android/icons/connected.svg" : "qrc:/hutsulshchyna-android/icons/disconnected.svg"
        onTriggered: {
            portHandler.portOpen ? portHandler.closeSerialPort() : portHandler.openSerialPort();
        }
    }

    Action {
        id: settingsSerialPortAction
        icon.name: "settings"
        icon.source: "qrc:/hutsulshchyna-android/icons/settings.svg"
        onTriggered: {
//            if (stackView.depth === 1){
//                listView.currentIndex = 0
//                stackView.push("qrc:/hutsulshchyna-android/qml/serialport-settings.qml")
//            }
            if (stackView.depth > 1) {
                stackView.pop();
            }
            listView.currentIndex = 0;
            listView.itemText = qsTr("Port Settings");
            stackView.push("serialport-settings.qml");
        }
    }

    Action {
        id: optionsMenuAction
        icon.name: "menu"
        icon.source: "qrc:/hutsulshchyna-android/icons/menu-vertical.svg"
        onTriggered: {}
    }

    header: ToolBar {
        RowLayout {
            spacing: 5
            anchors.fill: parent
            ToolButton {
                action: navigateBackAction
            }
            Label {
                id: titleLabel
                Layout.leftMargin: 20
                text: listView.currentItem ? listView.itemText : qsTr("Coordinates")
                font.pixelSize: 20
                Layout.fillWidth: true
            }
            ToolButton {
                Layout.alignment: Qt.AlignRight
                action: openSerialPortAction
            }
            ToolButton {
                Layout.alignment: Qt.AlignRight
                action: settingsSerialPortAction
            }
            ToolButton {
                Layout.leftMargin: 10
                Layout.alignment: Qt.AlignRight
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
            property string itemText: ""

            header: Component {
                Image {
                    height: drawer.height * 0.2
                    width: parent.width
                    source: "qrc:/hutsulshchyna-android/icons/header.jpg"
                    fillMode: Image.Stretch
                    Rectangle {
                        width: parent.width
                        height: 1
                        color: "#47652F"
                        anchors.bottom: parent.bottom
                    }
                }
            }
            delegate: ItemDelegate {
                id: control
                width: listView.width
                contentItem: Text {
                    text: model.title
                    color: "#47652F"
                    font.pixelSize: 18
                    font.bold: true
                }
                highlighted: ListView.isCurrentItem
                onClicked: {
                    listView.currentIndex = index;
                    stackView.push(model.source);
                    drawer.close();
                    listView.itemText = model.title;
                }
//                background: Rectangle {
//                    width: listView.width
//                    implicitHeight: 40
//                    opacity: enabled ? 1 : 0.3
//                    color: control.down ? "#dddedf" : "#eeeeee"

//                    Rectangle {
//                        width: parent.width
//                        height: 1
//                        color: control.down ? "#17a81a" : "#21be2b"
//                        anchors.bottom: parent.bottom
//                    }
//                }
            }
            model: ListModel {
                property var _data: [
                    { title: qsTr("Port Settings"), source: "serialport-settings.qml" },
                    { title: qsTr("Terminal"), source: "terminal.qml" }
                ]
                Component.onCompleted: { for (let obj of _data) append(obj) }
            }
            ScrollIndicator.vertical: ScrollIndicator { }
        }
    }

    ToastManager {
        id: toast
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: InitialItem {}
    }

    FileDialog {
        id: saveDialog
        fileMode: FileDialog.SaveFile
        nameFilters: ["KML files (*.kml)", "All files (*)"]
        title: qsTr("Save File")

        Component.onCompleted: {
            var currentDate = new Date();
            var formattedDateTime = Qt.formatDateTime(currentDate, "yyyy-MM-dd_hh-mm-ss");
            currentFile = formattedDateTime;
        }

        onAccepted: {
            fileHandler.processFile(selectedFile, stackView.initialItem.name, stackView.initialItem.description);
        }
    }
}
