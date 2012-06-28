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
    color: pallete.button
    border.color: "black"

    SystemPalette {
        id: pallete
        colorGroup: SystemPalette.Active
    }


    Image {
        id: img
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        width: 30
        anchors.leftMargin: 10
        fillMode: Image.PreserveAspectFit
    }

    Text {
        id: label
        text: "TEXT"
        anchors.left: img.right
        anchors.leftMargin: 10
        verticalAlignment: Text.AlignVCenter
        anchors.verticalCenter: parent.verticalCenter
        color: pallete.buttonText
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            root.clicked();
        }
    }
}
