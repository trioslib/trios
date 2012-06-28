// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: root
    property alias text: label.text
    signal clicked

    width: 100
    height: 62
    radius: 9
    border.color: "#000000"
    color: pallete.button

    SystemPalette {
        id: pallete
        colorGroup: SystemPalette.Active
    }

    Text {
        id: label
        text: "TEXT"
        verticalAlignment: Text.AlignVCenter
        anchors.horizontalCenter: parent.horizontalCenter
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
