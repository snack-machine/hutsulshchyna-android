import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SerialPortHandler

ScrollView  {
    contentWidth: availableWidth
    TextArea {
        id: textArea
        Layout.fillHeight: true
        Layout.preferredWidth: parent.width
        clip: true
        wrapMode: TextArea.Wrap
        font.pixelSize: 14
    }

    Connections {
        target: portHandler
        function onPortDataRead(data) {
            textArea.text += data;
            textArea.cursorPosition = textArea.length - 1
        }
    }
}
