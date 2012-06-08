// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Column {
    id: root
    property alias image: btn_image.source;
    property alias text: label.text;
    property bool enabled: true
    signal clicked;

    width: 100
    height: 60

    Image {
        id: btn_image
        width: parent.width
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Text {
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
    }

    MouseArea {
        onClicked: {
            if (root.enabled) {
                root.clicked();
            }
        }
    }

}
