// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: root
    property alias image: btn_image.source;
    property alias text: label.text;
    property alias image_height: btn_image.height
    property bool enabled: true
    property string root_state
    signal clicked;

    color: "#00000000"
    width: 100
    height: 80

    Image {
        id: btn_image
        anchors.top: parent.top
        anchors.topMargin: 5
        fillMode: Image.PreserveAspectFit
        height: parent.height - label.height - 10
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Text {
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: btn_image.bottom
        anchors.topMargin: 5
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (root.enabled) {
                root.clicked();
                app_root.state = root_state;
            }
        }
    }

    states: [
        State {
            name: "selected"

            PropertyChanges {
                target: root
                color: "#6d6d6d"
            }
        }
    ]
}

