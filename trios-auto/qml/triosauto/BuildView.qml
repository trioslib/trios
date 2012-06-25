// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "components"

View {
    id: root

    function build_finished(path) {
        root.state = "build_finished";
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

            trios.build_finished.connect(build_finished);
            trios.build("window.win", "images.imgset");
        }
    }

    View {
        id: building_message
        visible: false

        Image {
            id: wait_building
            anchors.centerIn: parent
            source: "img/wait.gif"
            opacity: 0.5

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

        Text {
            id: text4
            x: 63
            text: qsTr("Building an operator takes a really long time. Go pick a(t least one) cofee.")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: wait_building.bottom
            anchors.topMargin: 10
            font.pixelSize: 12
        }
    }

    Text {
        id: text1
        text: "Build Operator"
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.top: parent.top
        anchors.topMargin: 30
        font.pixelSize: 24
    }

    InfoDialog {
        id: infodialog1
        x: 38
        y: -105
        visible: false

        onDismissed: {
            var path = fileUtils.fileSaveDialog("Save operator", "", "Operator (*.itv)");
            if (path != null && path != "") {
                fileUtils.copy("itv_final.itv", path);
                project_view.operator_path = path;
            }
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
