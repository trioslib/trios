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
        text: "Apply Operator"
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.top: parent.top
        anchors.topMargin: 30
        font.pixelSize: 24
    }

    Text {
        id: win_label
        x: 30
        y: 66
        text: qsTr("Select an image")
        font.pixelSize: 16
    }

    Rectangle {
        id: rectangle1
        x: 30
        y: 85
        width: 326
        height: 29
        color: "#ffffff"
        radius: 5
        Text {
            id: win_path
            text: qsTr("None")
            font.pixelSize: 12
            anchors.fill: parent
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
    }

    Button {
        id: load_win
        x: 375
        y: 85
        width: 76
        height: 29
        text: "Load"
    }

    ImageButton {
        id: imagebutton1
        x: 30
        y: 405
        width: 100
        height: 39
        text: "Apply"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 16
        image: "img/play.svg"
    }

}
