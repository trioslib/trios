// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Item {
    id: app_root
    width: 640
    height: 480
    state: "welcome"

    Row {
        id: row1
        anchors.fill: parent

        Rectangle {
            id: side_menu
            width: 100
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.top: parent.top

            Image {
                id: bg
                anchors.fill: parent

                source: "img/menu_bg.svg"
            }

            Column {
                id: items
                anchors.topMargin: 10
                spacing: 10
                anchors.fill: parent

                MenuItem {
                    id: menuitem1
                    image: "img/welcome.svg"
                    text: "Welcome"
                    onClicked: {
                        app_root.state = "welcome";
                    }
                }

                MenuItem {
                    image: "img/project.svg"
                    text: "Project"
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
                    image: "img/build.svg"
                    text: "Build"
                }

                MenuItem {
                    image: "img/play.svg"
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
                color: "#b9b9c0"

            }
        }
    }
    states: [
        State {
            name: "welcome"

            PropertyChanges {
                target: menuitem1
                state: "selected"
            }
        }
    ]
}
