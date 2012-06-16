// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "components"

Rectangle {
    id: root
    width: 640
    height: 480
    anchors.fill: parent
    color: "#d5d5d5"

    Rectangle {
        id: rectangle1
        width: 640
        height: 62
        color: "#ffffff"
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0

        Button {
            id: button1
            x: 13
            y: 6
            width: 100
            height: 50
            text: "Load Sample"

            onClicked: {
                var input = fileUtils.fileDialog("Choose the input image");
                var ideal = fileUtils.fileDialog("Choose the ideal image");
                var mask = fileUtils.fileDialog("Choose the mask image (optional)");
                if (input != "" && ideal != "") {
                    console.log("Sample!");
                }
            }
        }
    }

    Column {
        id: column1
        y: 281
        height: 400
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
    }

}
