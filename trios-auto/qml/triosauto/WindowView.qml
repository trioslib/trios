// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: root
    width: 640
    height: 480
    anchors.fill: parent
    color: "#d5d5d5"

    Text {
        id: text1
        text: qsTr("Window Configuration")
        anchors.top: parent.top
        anchors.topMargin: 0
        horizontalAlignment: Text.AlignHCenter
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        font.pixelSize: 20
    }

    Row {
        id: row1
        y: 44
        width: 640
        height: 59
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0

        Text {
            id: text2
            x: 140
            y: 22
            text: qsTr("text")
            font.pixelSize: 12
        }

        TextEdit {
            id: text_edit1
            x: 112
            y: 17
            width: 80
            height: 20
            text: qsTr("text edit")
            font.pixelSize: 12
        }

        Text {
            id: text3
            x: 229
            y: 17
            text: qsTr("text")
            font.pixelSize: 12
        }

        TextEdit {
            id: text_edit2
            x: 233
            y: 17
            width: 80
            height: 20
            text: qsTr("text edit")
            font.pixelSize: 12
        }
    }
}
