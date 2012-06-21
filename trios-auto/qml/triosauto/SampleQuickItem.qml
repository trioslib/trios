// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Item {
    x: 5
    width: 70
    height: parent.height


    Column {
        id: row1
        spacing: 10
        Image {
            width: 50
            height: 50
            source: input
        }

        Image {
            width: 50
            height: 50
            source: ideal
        }

        Text {
            text: index
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            input_image.source = input;
            ideal_image.source = ideal;
        }
    }
    Image {
        x: 47
        y: 0
        width: 23
        height: 22
        anchors.right: parent.right
        source: "img/delete.svg"
        MouseArea {
            anchors.fill: parent
            onClicked: {
                images.remove(index);
                if (samples_quick_view.currentIndex == index) {
                    input_image.source = "";
                    ideal_image.source = "";
                }
            }
        }
    }
}
