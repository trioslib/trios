import QtQuick 1.1

Rectangle {
    property alias text: text_edit1.text

    color: "white"
    radius: 5
    width:  326
    height: 29
    clip: true

    TextEdit {
        id: text_edit1
        text: qsTr("text edit")
        cursorVisible: false
        font.pixelSize: 12
        anchors.fill: parent
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }
}
