import QtQuick 1.1
import "components"

View {

    id: root

    Text {
        id: text1
        text: "Apply Operator"
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.top: parent.top
        anchors.topMargin: 30
        font.pixelSize: 24
    }

    Text {
        id: win_label
        x: 30
        y: 66
        text: qsTr("Select an image")
        font.pixelSize: 16
    }

    Rectangle {
        id: rectangle1
        x: 30
        y: 85
        width: 326
        height: 29
        color: "#ffffff"
        radius: 5
        Text {
            id: input_apply_path
            text: qsTr("None")
            font.pixelSize: 12
            anchors.fill: parent
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
    }

    Button {
        id: load_apply_input
        x: 375
        y: 85
        width: 76
        height: 29
        text: "Load"

        onClicked: {
            var path = fileUtils.fileOpenDialog("Choose an image to apply the operator");
            if (path !=  null && path != "") {
                input_apply_path.text = path;
                apply_panel.image_left = path;
            }
        }
    }

    ImageButton {
        id: apply_button
        x: 30
        y: 405
        width: 100
        height: 39
        text: "Apply"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 16
        image: "img/play.svg"

        onClicked: {
            var res_path = trios.apply(input_apply_path.text, project_view.operator_path);
            apply_panel.image_right = "";
            apply_panel.image_right = res_path;
            root.state = "apply_finished";
        }
    }

    DualImagePanel {
        id: apply_panel
        anchors.bottom: apply_button.top
        anchors.bottomMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: rectangle1.bottom
        anchors.topMargin: 10
    }

    InfoDialog {
        id: apply_finished_dialog
        x: 30
        y: 85
        visible: false

        title: "Apply finished"
        content: "The operator was applied with success."
        dialog_width: 300
        dialog_height: 130

        onDismissed: {
            root.state = "";
        }
    }
    states: [
        State {
            name: "apply_finished"

            PropertyChanges {
                target: apply_finished_dialog
                visible: true
            }
        }
    ]



}
