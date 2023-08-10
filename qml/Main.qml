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

    readonly property string settingsFile: (Qt.platform.os === "android")?"qrc:/hutsulshchyna-android/qml/serialport-settings-android.qml":"qrc:/hutsulshchyna-android/qml/serialport-settings.qml"

    function getSerialPortSettingsQMLPath() {
        if (Qt.platform.os === "android") {
            return "qrc:/hutsulshchyna-android/qml/serialport-settings-android.qml";
        } else {
            return "qrc:/hutsulshchyna-android/qml/serialport-settings.qml";
        }
    }

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
//            if (stackView.depth === 1){
//                listView.currentIndex = 0
//                stackView.push("qrc:/hutsulshchyna-android/qml/serialport-settings.qml")
//            }
            if (stackView.depth > 1) {
                stackView.pop();
            }
            listView.currentIndex = 0;
            stackView.push(window.settingsFile);
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
            Label {
                id: titleLabel
                Layout.leftMargin: 20
                text: listView.currentItem ? listView.currentItem.text : qsTr("Coordinates")
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
                font.pixelSize: 18
                font.bold: true
                highlighted: ListView.isCurrentItem
                onClicked: {
                    listView.currentIndex = index
                    stackView.push(model.source)
                    drawer.close()
                }
            }

            model: ListModel {
                // delete "-android" for desktop version
                ListElement { title: qsTr("Port Settings"); source: "qrc:/hutsulshchyna-android/qml/serialport-settings-android.qml" }
//                ListElement { title: qsTr("Port Settings"); source: window.settingsFile }
                ListElement { title: qsTr("Terminal"); source: "qrc:/hutsulshchyna-android/qml/terminal.qml" }
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

    Rectangle {
        id: infoWindow
        width: 200
        height: 60
        color: "lightgreen"
        border.color: "darkgreen"
        radius: 10
        visible: false

        Text {
            anchors.centerIn: parent
            text: "Operation successful!"
            font.pixelSize: 16
            color: "black"
        }

        Timer {
            id: timer
            interval: 2000
            repeat: false
            onTriggered: infoWindow.visible = false
        }
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

//    FileDialog {
//        id: openDialog
//        fileMode: FileDialog.OpenFile
//        nameFilters: ["KML files (*.kml)", "All files (*)"]
//        title: qsTr("Open File")

//        onAccepted: {
//            fileHandler.processFile(selectedFile, stackView.initialItem.name, stackView.initialItem.description);
//        }
//    }
}
