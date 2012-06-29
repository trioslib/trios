// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    property alias dialog_width: dialog.width
    property alias dialog_height: dialog.height
    property alias title: title.text
    property alias content: content.text

    signal dismissed

    id: root
    width: 640
    height: 480
    anchors.fill: parent
    color: "#7fa8a8a8"

    Rectangle {
        id: dialog
        x: 120
        y: 146
        width: 200
        height: 200
        color: "#ffffff"
        radius: 10
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        Button {
            id: okButton
            x: 50
            y: 171
            width: 56
            height: 29
            text: "Ok"
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter

            onClicked: {
                root.dismissed();
            }
        }

        Text {
            id: title
            x: 88
            text: qsTr("Title")
            font.bold: true
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 14
        }

        Text {
            id: content
            text: qsTr("text")
            wrapMode: Text.WordWrap
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottom: okButton.top
            anchors.bottomMargin: 10
            anchors.top: title.bottom
            anchors.topMargin: 10
            font.pixelSize: 12
        }
    }

}
