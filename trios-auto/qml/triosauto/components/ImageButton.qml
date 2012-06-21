// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: root
    property alias text: label.text
    property alias image: img.source
    property alias imageWidth: img.width
    signal clicked


    width: 100
    height: 62
    radius: 9
    border.color: "#000000"

    Image {
        id: img
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 3
        anchors.top: parent.top
        anchors.topMargin: 3
        width: 30
    }

    Text {
        id: label
        text: "TEXT"
        anchors.left: img.right
        anchors.leftMargin: 10
        verticalAlignment: Text.AlignVCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            root.clicked();
        }
    }
}
