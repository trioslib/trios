// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "window_configuration.js" as Script

Rectangle {
    id: root
    property int window_width: 3
    property int window_height: 3

    width: 300
    height: 300
    border.color: "#000000"

    function set(i, j, value) {
        var k;
        for (k = 0; k < root.children.length; k++) {
            var w = root.children[k];
            if (w.i == i && w.j == j) {
                w.selected = value;
            }
        }
    }

    Component.onCompleted: {
        // create window configuration
        Script.update();
    }

    onWindow_heightChanged: {
        // update window configuration
        Script.update();
    }

    onWindow_widthChanged: {
        // update window configuration
        Script.update();
    }

}
