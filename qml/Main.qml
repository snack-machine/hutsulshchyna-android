import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import SerialPortHandler
import CoordHandler
import FileHandler
import SerialPortENUM

ApplicationWindow {
    id: window
    width: 400
    height: 680
    visible: true
    title: "Coordinator"

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
            if (listView.currentIndex !== 0) {
                if (stackView.depth > 1) {
                    stackView.pop();
                }
                listView.currentIndex = 0;
                listView.itemText = qsTr("Port Settings");
                stackView.push("serialport-settings.qml");
            }
        }
    }

    Action {
        id: optionsMenuAction
        icon.name: "menu"
        icon.source: "qrc:/hutsulshchyna-android/icons/menu-vertical.svg"
        onTriggered: optionsMenu.open()
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
                Layout.leftMargin: 5
                Layout.alignment: Qt.AlignRight
                action: optionsMenuAction

                Menu {
                    id: optionsMenu
                    x: parent.width - width
                    transformOrigin: Menu.TopRight

                    Action {
                        text: qsTr("Settings")
                        onTriggered: settingsDialog.open()
                    }
                    Action {
                        text: qsTr("About")
                        onTriggered: aboutDialog.open()
                    }
                }
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

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: InitialItem {}
    }

    ToastManager {
        id: toast
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

    Dialog {
        id: settingsDialog
        x: Math.round((window.width - width) / 2)
        y: Math.round(window.height / 6)
        width: Math.round(Math.min(window.width, window.height) / 3 * 2)
        modal: true
        focus: true
        title: qsTr("Settings")

        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            portHandler.language = languageBox.currentIndex + 1;
            settingsDialog.close();
        }
        onRejected: {
            languageBox.currentIndex = languageBox.langIndex;
            settingsDialog.close();
        }

        contentItem: ColumnLayout {
            id: settingsColumn
            spacing: 20

            RowLayout {
                spacing: 10

                Label {
                    text: qsTr("Language:")
                }
                ComboBox {
                    id: languageBox
                    Layout.fillWidth: true
                    textRole: "text"
                    valueRole: "value"
                    property int langIndex: -1
                    model: [
                        { value: 1, text: qsTr("English") },
                        { value: 2, text: qsTr("Ukrainian") }
                    ]
                    Component.onCompleted: {
                        langIndex = indexOfValue(portHandler.language);
                        if (langIndex !== -1)
                            currentIndex = langIndex;
                    }
                }
            }
            Label {
                text: qsTr("Restart required")
                color: "#e41e25"
                opacity: languageBox.currentIndex !== languageBox.langIndex ? 1.0 : 0.0
                horizontalAlignment: Label.AlignHCenter
                verticalAlignment: Label.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }

    Dialog {
        id: aboutDialog
        modal: true
        focus: true
        title: qsTr("About")
        x: (window.width - width) / 2
        y: window.height / 6
        width: Math.min(window.width, window.height) / 3 * 2
        contentHeight: aboutColumn.height

        Column {
            id: aboutColumn
            spacing: 20

            Label {
                width: aboutDialog.availableWidth
                text: qsTr("The application allows you to calculate the coordinates of " +
                            "the current position more accurately. To do this, you need to leave " +
                            "the GPS sensor in a stationary position for a while.\n" +
                            "Point can be saved/added to KML file")
                wrapMode: Label.Wrap
                font.pixelSize: 12
                Layout.alignment: Qt.AlignJustify
            }

            Label {
                width: aboutDialog.availableWidth
                text: qsTr("Source code: https://github.com/Atr0p0s/Coordinator")
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }
        }
    }
}
