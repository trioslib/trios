// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "components"

View {
    id: root

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

        onClicked: {
            root.state = "building";

            var obj = {};
            obj["width"] = window_view.win_width;
            obj["height"] = window_view.win_height;
            obj["window_data"] = window_view.getWindowData();
            trios2qml.write_window(obj, "window.win");

            var s = samples_view.getSamples();
            trios2qml.write_imgset(s, "images.imgset");

            var temp_operator_path = trios.build("window.win", "images.imgset");
            root.state = "build_finished";
        }
    }

    Rectangle {
        id: building_message
        color: "#ffffff"
        visible: false
        anchors.fill: parent

        Image {
            id: wait_building
            anchors.centerIn: parent
            source: "img/wait.gif"

            RotationAnimation {
                target: wait_building
                direction: RotationAnimation.Clockwise
                duration: 3000
                from: 0
                to: 359
                running: true
                loops: Animation.Infinite
            }
        }
    }

    InfoDialog {
        id: infodialog1
        x: 38
        y: -105
        visible: false

        onDismissed: {
            root.state = "";
        }
    }
    states: [
        State {
            name: "building"

            PropertyChanges {
                target: building_message
                visible: true
            }

            PropertyChanges {
                target: infodialog1
                visible: false
            }
        },
        State {
            name: "build_finished"

            PropertyChanges {
                target: infodialog1
                dialog_height: 150
                title: "Build finished"
                content: "The build has finished. Click Ok to save the operator."
                visible: true
                opacity: 1
            }
        }
    ]


}
