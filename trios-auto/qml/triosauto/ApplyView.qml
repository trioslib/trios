import QtQuick 1.1
import "components"

Rectangle {
    property string operator_path: ""

    id: root
    width: 640
    height: 460
    anchors.fill: parent
    color: "#d5d5d5"

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
                input_image.source = path;
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
            console.log("Apply: " + root.operator_path + " " + input_apply_path.text)
            var res_path = trios.apply(input_apply_path.text, root.operator_path);
            console.log("FINISH " + res_path);
            result_image.source = "";
            result_image.source = res_path;
        }
    }

    Item {
        id: flick_props
        property int flickX: 0
        property int flickY: 0
    }

    Flickable {
        id: input_flick
        x: 0
        width: parent.width/2
        boundsBehavior: Flickable.StopAtBounds
        anchors.bottom: apply_button.top
        anchors.bottomMargin: 10
        anchors.top: load_apply_input.bottom
        anchors.topMargin: 10
        clip: true
        contentWidth: input_image.width
        contentHeight: input_image.height
        contentX: flick_props.flickX
        contentY: flick_props.flickY

        onContentXChanged: {
            flick_props.flickX = contentX;
            result_flick.returnToBounds();
        }

        onContentYChanged: {
            flick_props.flickY = contentY;
            result_flick.returnToBounds();
        }

        Image {
            id: input_image
        }
    }

    Flickable {
        id: result_flick
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: input_flick.right
        anchors.leftMargin: 0
        anchors.top: load_apply_input.bottom
        anchors.topMargin: 10
        anchors.bottom: apply_button.top
        anchors.bottomMargin: 10
        clip: true
        contentWidth: result_image.width
        contentHeight: result_image.height
        contentX: flick_props.flickX
        contentY: flick_props.flickY

        Image {
            id: result_image
            cache: false
        }

        onContentXChanged: {
            flick_props.flickX = contentX;
            result_flick.returnToBounds();
        }

        onContentYChanged: {
            flick_props.flickY = contentY;
            result_flick.returnToBounds();
        }
    }

}
