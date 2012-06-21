// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "components"

Rectangle {
    id: root
    width: 640
    height: 460
    anchors.fill: parent
    color: "#d5d5d5"

    Text {
        id: text1
        text: "Build Operator"
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.top: parent.top
        anchors.topMargin: 30
        font.pixelSize: 24
    }

    Text {
        id: text4
        x: 30
        y: 100
        text: qsTr("Building an operator takes a really long time. Push the button below and go pick a cofee.")
        font.pixelSize: 12
    }

    ImageButton {
        id: imagebutton1
        x: 30
        y: 145
        width: 94
        height: 46
        image: "img/build.svg"
        text: "Build"
    }


}
