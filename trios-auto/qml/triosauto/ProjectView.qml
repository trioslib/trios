// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "components"

View {
    property alias operator_path: operator_path_txt.text

    id: root

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
        y: 96
        text: qsTr("Window Configuration")
        anchors.left: parent.left
        anchors.leftMargin: 30
        font.pixelSize: 16
    }



    TextInput {
        id: win_path
        y: 135
        height: 29
        text: "None"
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.right: load_win.left
        anchors.rightMargin: 15
    }

    Button {
        id: load_win
        x: 375
        y: 135
        width: 76
        height: 29
        text: "Load"
        anchors.right: save_win.left
        anchors.rightMargin: 15

        onClicked: {
            var path = fileUtils.fileOpenDialog("Select the window configuration file", "", "Window configuration (*.win)");
            if (path != null && path != "") {
                var window = trios2qml.read_window(path);
                if (window['error'] == false) {
                    window_view.loadWindow(window['width'], window['height'], window['window_data']);
                    win_path.text = path;
                } else {
                    root.state = "error_load";
                }
            }
        }
    }

    Button {
        id: save_win
        x: 462
        y: 135
        width: 101
        height: 29
        text: "Save Current"
        anchors.right: parent.right
        anchors.rightMargin: 10

        onClicked: {
            var obj = {};
            obj["width"] = window_view.win_width;
            obj["height"] = window_view.win_height;
            obj["window_data"] = window_view.getWindowData();

            var save_path = fileUtils.fileSaveDialog("Save window configuration", "", "Window configuration (*.win)");
            if (trios2qml.write_window(obj, save_path)) {
                console.log("WRITE OK!");
                win_path.text = save_path;
            } else {
                console.log("WRITE ERROR");
            }
        }
    }

    Text {
        id: imgset_label
        x: 29
        y: 183
        text: qsTr("Image Set")
        font.pixelSize: 16
    }

    TextInput {
        id: imgset_path
        y: 222
        height: 29
        text: "None"
        anchors.right: load_imgset.left
        anchors.rightMargin: 15
        anchors.left: parent.left
        anchors.leftMargin: 30
    }

    Button {
        id: load_imgset
        x: 374
        y: 222
        width: 76
        height: 29
        text: "Load"
        anchors.right: save_imgset.left
        anchors.rightMargin: 15

        onClicked: {
            var path = fileUtils.fileOpenDialog("Select the image set", "", "Image Set (*.imgset)");
            if (path != null && path != "") {
                var obj = trios2qml.read_imgset(path);
                if (obj['error'] == false) {
                    samples_view.loadSamples(obj);
                    imgset_path.text = path;
                } else {
                    root.state = "error_load";
                }
            }
        }
    }

    Button {
        id: save_imgset
        x: 461
        y: 222
        width: 101
        height: 29
        text: "Save Current"
        anchors.right: parent.right
        anchors.rightMargin: 10

        onClicked: {
            var s = samples_view.getSamples();
            var path = fileUtils.fileSaveDialog("Save image set", "", "Image set (*.imgset)");
            var r = trios2qml.write_imgset(s, path);
            if (r) {
                imgset_path.text = path;
            } else {
                console.log("IMGSERT WRITE FAIL");
            }
        }
    }

    Text {
        id: operator_label
        x: 30
        y: 267
        text: qsTr("Operator")
        font.pixelSize: 16
    }

    TextInput {
        id: operator_path_txt
        y: 306
        height: 29
        text: "None"
        anchors.right: load_operator.left
        anchors.rightMargin: 15
        anchors.left: parent.left
        anchors.leftMargin: 30
    }

    Button {
        id: load_operator
        x: 375
        y: 306
        width: 76
        height: 29
        text: "Load"
        anchors.right: parent.right
        anchors.rightMargin: 126

        onClicked: {
            var path = fileUtils.fileOpenDialog("Select the operator", "", "Operator (*.itv)");
            operator_path_txt.text = path;
        }

    }

    InfoDialog {
        id: error_load
        anchors.fill: parent
        dialog_width: 250
        dialog_height: 130
        title: "Error loading the file."
        content: "There was an error loading the file."
        visible: false

        onDismissed: {
            root.state = "";
        }
    }

    InfoDialog {
        id: error_save
        anchors.fill: parent
        dialog_width: 250
        dialog_height: 130
        title: "Error saving the file."
        content: "There was an error saving the file."
        visible: false

        onDismissed: {
            root.state = "";
        }
    }

    states: [
        State {
            name: "error_load"

            PropertyChanges {
                target: error_load
                visible: true
            }
        },

        State {
            name: "error_save"

            PropertyChanges {
                target: error_save
                visible: true
            }
        }
    ]

}
