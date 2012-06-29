// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Item {
    property alias image_left: input_image.source
    property alias image_right: result_image.source

    id: root
    width: 640
    height: 480


    Item {
        id: flick_props
        property int flickX: 0
        property int flickY: 0
    }

    Flickable {
        id: input_flick
        width: parent.width/2
        anchors.left: parent.left
        anchors.leftMargin: 0
        boundsBehavior: Flickable.StopAtBounds
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
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
            cache: false
        }
    }

    Flickable {
        id: result_flick
        boundsBehavior: Flickable.StopAtBounds
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: input_flick.right
        anchors.leftMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
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
