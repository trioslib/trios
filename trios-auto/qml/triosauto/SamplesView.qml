// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "components"
import QtQuick 1.0

Rectangle {
    id: root
    width: 640
    height: 480
    anchors.fill: parent
    color: "#d5d5d5"

    function loadSamples(samples) {
        images.clear();
        for (var k in samples) {
            var sample = samples[k];
            if (k != 'error') {
                images.add_sample(sample[0], sample[1]);
            }
        }
    }

    function getSamples() {
        var samples = {};
        var i;
        for (i = 0; i < images.count; i++) {
            var sample = images.get(i);
            samples[i] = [sample["input"], sample["ideal"]];
        }
        return samples;
    }

    ListModel {
        id: images

        function add_sample(input, ideal) {
            images.append({"input": input, "ideal": ideal});
        }
    }

    Button {
        id: button1
        x: 13
        y: 6
        width: 140
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



    Text {
        id: input_label
        x: 20
        text: qsTr("Input Image")
        anchors.top: button1.bottom
        anchors.topMargin: 5
        font.pixelSize: 12
    }

    Text {
        id: ideal_label
        text: qsTr("Ideal Image")
        anchors.leftMargin: parent.width/2
        anchors.left: parent.left
        anchors.top: button1.bottom
        anchors.topMargin: 5
        font.pixelSize: 12
    }

    DualImagePanel {
        id: sample_panel
        anchors.bottom: samples_quick_view.top
        anchors.bottomMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: input_label.bottom
        anchors.topMargin: 10
    }



}
