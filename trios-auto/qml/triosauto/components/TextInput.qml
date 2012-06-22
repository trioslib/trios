import QtQuick 1.1

Rectangle {
    property alias text: text_edit1.text

    color: "white"
    radius: 5

    TextEdit {
        id: text_edit1
        anchors.fill: parent
        text: qsTr("text edit")
        font.pixelSize: 12
    }
}
