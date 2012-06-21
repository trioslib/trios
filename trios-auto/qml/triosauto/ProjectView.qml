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
        id: title
        text: qsTr("Project Configurations")
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.top: parent.top
        anchors.topMargin: 30
        font.pixelSize: 24
    }

    Text {
        id: win_label
        x: 30
        y: 96
        text: qsTr("Window Configuration")
        font.pixelSize: 16
    }

    Rectangle {
        id: rectangle1
        x: 30
        y: 135
        width: 326
        height: 29
        color: "#ffffff"
        radius: 5

        Text {
            id: win_path
            text: qsTr("None")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            anchors.fill: parent
            font.pixelSize: 12
        }
    }

    Button {
        id: load_win
        x: 375
        y: 135
        width: 76
        height: 29
        text: "Load"

        onClicked: {
            var path = fileUtils.fileOpenDialog("Select the window configuration file", "", "Window configuration (*.win)");

            win_path.text = path;
        }
    }

    Button {
        id: save_win
        x: 462
        y: 135
        width: 101
        height: 29
        text: "Save Current"
    }

    Text {
        id: imgset_label
        x: 29
        y: 183
        text: qsTr("Image Set")
        font.pixelSize: 16
    }

    Rectangle {
        id: rectangle2
        x: 29
        y: 222
        width: 326
        height: 29
        color: "#ffffff"
        radius: 5
        Text {
            id: imgset_path
            text: qsTr("None")
            font.pixelSize: 12
            anchors.fill: parent
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
    }

    Button {
        id: load_imgset
        x: 374
        y: 222
        width: 76
        height: 29
        text: "Load"

        onClicked: {
            var path = fileUtils.fileOpenDialog("Select the image set", "", "Image Set (*.imgset)");

            imgset_path.text = path;
        }
    }

    Button {
        id: save_imgset
        x: 461
        y: 222
        width: 101
        height: 29
        text: "Save Current"
    }

    Text {
        id: operator_label
        x: 30
        y: 267
        text: qsTr("Operator")
        font.pixelSize: 16
    }

    Rectangle {
        id: rectangle3
        x: 30
        y: 306
        width: 326
        height: 29
        color: "#ffffff"
        radius: 5
        Text {
            id: operator_path
            text: qsTr("None")
            font.pixelSize: 12
            anchors.fill: parent
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
    }

    Button {
        id: load_operator
        x: 375
        y: 306
        width: 76
        height: 29
        text: "Load"

        onClicked: {
            var path = fileUtils.fileOpenDialog("Select the operator", "", "Operator (*.itv)");
            operator_path.text = path;
        }

    }

    Button {
        id: save_operator
        x: 462
        y: 306
        width: 101
        height: 29
        text: "Save Current"
    }
}
