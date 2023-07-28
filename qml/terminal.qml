import QtQuick
import QtQuick.Controls
//import QtQuick.Dialogs
import QtQuick.Layouts
import SerialPortHandler
import SerialPortENUM

ScrollView  {
    contentWidth: availableWidth
    TextArea {
        id: textArea
//        anchors.fill: parent
        wrapMode: TextArea.Wrap
        font.pixelSize: 14

    //    Component.onCompleted: {
    //        // Connect the readyRead signal of serialPort to a slot to read data
    //        serialPort.readyRead.connect(readData);
    //    }

    //    function readData() {
    //        var newData = serialPort.readAll();
    //        textArea.text += newData;
    //    }
    }
}
