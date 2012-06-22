// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "components"

Rectangle {
    id: root
    width: 640
    height: 480
    anchors.fill: parent
    color: "#d5d5d5"

    function loadSamples(samples) {
        for (var k in samples) {
            var sample = samples[k];
            images.add_sample(sample[0], sample[1]);
        }
    }

    ListModel {
        id: images

        function add_sample(input, ideal) {
            images.append({"input": input, "ideal": ideal});
        }
    }

    Rectangle {
        id: rectangle1
        width: 640
        height: 62
        color: "#ffffff"
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0

        Button {
            id: button1
            x: 13
            y: 6
            width: 100
            height: 50
            text: "Load Sample"

            onClicked: {
                var input = fileUtils.fileOpenDialog("Choose the input image");
                var ideal = fileUtils.fileOpenDialog("Choose the ideal image");
                var mask = fileUtils.fileOpenDialog("Choose the mask image (optional)");
                if (input != "" && ideal != "") {
                    images.add_sample(input, ideal);
                }
            }
        }
    }

    ListView {
        clip: true
        id: samples_quick_view
        y: 320
        height: 160
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        orientation: ListView.Horizontal
        model: images
        delegate: SampleQuickItem {
        }
    }

    Item {
        id: flick_props
        property int flickX: 0
        property int flickY: 0
    }

    Flickable {
        id: input_view_flick
        x: 0
        width: parent.width/2
        boundsBehavior: Flickable.StopAtBounds
        anchors.bottom: samples_quick_view.top
        anchors.bottomMargin: 10
        anchors.top: input_label.bottom
        anchors.topMargin: 0
        clip: true
        contentWidth: input_image.width
        contentHeight: input_image.height
        contentX: flick_props.flickX
        contentY: flick_props.flickY

        onContentXChanged: {
            flick_props.flickX = contentX;
            input_view_flick.returnToBounds();
        }

        onContentYChanged: {
            flick_props.flickY = contentY;
            input_view_flick.returnToBounds();
        }

        Image {
            id: input_image
        }
    }

    Flickable {
        id: ideal_view_flick
        boundsBehavior: Flickable.StopAtBounds
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: input_view_flick.right
        anchors.leftMargin: 0
        anchors.top: ideal_label.bottom
        anchors.topMargin: 0
        anchors.bottom: samples_quick_view.top
        anchors.bottomMargin: 10
        clip: true
        contentWidth: ideal_image.width
        contentHeight: ideal_image.height
        contentX: flick_props.flickX
        contentY: flick_props.flickY

        Image {
            id: ideal_image
        }

        onContentXChanged: {
            flick_props.flickX = contentX;
            ideal_view_flick.returnToBounds();
        }

        onContentYChanged: {
            flick_props.flickY = contentY;
            ideal_view_flick.returnToBounds();
        }
    }

    Text {
        id: input_label
        x: 20
        y: 62
        text: qsTr("Input Image")
        font.pixelSize: 12
    }

    Text {
        id: ideal_label
        x: 381
        text: qsTr("Ideal Image")
        anchors.top: rectangle1.bottom
        anchors.topMargin: 0
        font.pixelSize: 12
    }



}
