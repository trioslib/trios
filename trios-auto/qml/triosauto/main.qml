// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Item {
    id: app_root
    width: 640
    height: 480

    Row {
        id: row1
        anchors.fill: parent

        Rectangle {
            id: side_menu
            width: 100
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            Column {
                id: items
                anchors.fill: parent

                MenuItem {
                    text: "Welcome"
                }

                MenuItem {
                    text: "Window"
                }

                MenuItem {
                    text: "Samples"
                }

                MenuItem {
                    text: "Decision"
                }

                MenuItem {
                    text: "Apply"
                    enabled: false
                }
            }
        }

        Rectangle {
            id: config_container
            width: app_root.width - side_menu.width
            height: app_root.height

            WelcomeView {

            }
        }
    }
}
