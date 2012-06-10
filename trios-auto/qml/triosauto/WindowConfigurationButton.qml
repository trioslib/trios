// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    property int i
    property int j

    width: parent.width / parent.window_width
    height: parent.height / parent.window_height
    x: j * width
    y: i * height
    border.color: "#000000"
}
