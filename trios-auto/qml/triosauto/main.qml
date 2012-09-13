// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Item {
    id: app_root
    width: 640
    height: 580
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
                    id: menu_welcome
                    image: "img/welcome.svg"
                    text: "Welcome"
                    root_state: "welcome"
                }

                MenuItem {
                    id: menu_project
                    image: "img/project.svg"
                    text: "Project"
                    root_state: "project"
                }

                MenuItem {
                    id: menu_window
                    text: "Window"
                    image: "img/window.svg"
                    root_state: "window"
                }

                MenuItem {
                    id: menu_samples
                    text: "Samples"
                    image: "img/samples.svg"
                    root_state: "samples"
                }

                MenuItem {
                    id: menu_build
                    image: "img/build.svg"
                    text: "Build"
                    root_state: "build"
                }

                MenuItem {
                    id: menu_apply
                    image: "img/play.svg"
                    text: "Apply"
                    root_state: "apply"
                }
            }
        }

        Rectangle {
            id: config_container
            width: app_root.width - side_menu.width
            height: app_root.height

            WelcomeView {
                id: welcome_view

            }

            ProjectView {
                id:project_view
            }

            WindowView {
                id: window_view
            }

            SamplesView {
                id: samples_view
            }

            BuildView {
                id: build_view
            }

            ApplyView {
                id: apply_view
                visible: false
            }

        }
    }
    states: [
        State {
            name: "welcome"

            PropertyChanges {
                target: menu_welcome
                state: "selected"
            }
            PropertyChanges {
                target: menu_window
                state: ""
            }

            PropertyChanges {
                target: welcome_view
                visible: true
            }

            PropertyChanges {
                target: window_view
                visible: false
            }

            PropertyChanges {
                target: samples_view
                visible: false
            }

            PropertyChanges {
                target: project_view
                visible: false
            }

            PropertyChanges {
                target: build_view
                visible: false
            }

            PropertyChanges {
                target: apply_view
                visible: false
            }

        },
        State {
            name: "window"
            PropertyChanges {
                target: menu_window
                state: "selected"
            }
            PropertyChanges {
                target: menu_welcome
                state: ""
            }

            PropertyChanges {
                target: window_view
                visible: true
            }

            PropertyChanges {
                target: welcome_view
                visible: false
            }

            PropertyChanges {
                target: samples_view
                visible: false
            }

            PropertyChanges {
                target: project_view
                visible: false
            }

            PropertyChanges {
                target: build_view
                visible: false
            }

            PropertyChanges {
                target: apply_view
                visible: false
            }

        },
        State {
            name: "samples"

            PropertyChanges {
                target: welcome_view
                visible: false
            }

            PropertyChanges {
                target: window_view
                visible: false
            }

            PropertyChanges {
                target: samples_view
                visible: true
            }

            PropertyChanges {
                target: menu_samples
                state: "selected"
            }

            PropertyChanges {
                target: project_view
                visible: false
            }

            PropertyChanges {
                target: build_view
                visible: false
            }

            PropertyChanges {
                target: apply_view
                visible: false
            }

        },
        State {
            name: "project"

            PropertyChanges {
                target: welcome_view
                visible: false
            }

            PropertyChanges {
                target: project_view
                visible: true
            }

            PropertyChanges {
                target: window_view
                visible: false
            }

            PropertyChanges {
                target: samples_view
                visible: false
            }

            PropertyChanges {
                target: menu_project
                state: "selected"
            }

            PropertyChanges {
                target: build_view
                visible: false
            }

            PropertyChanges {
                target: apply_view
                visible: false
            }

        },
        State {
            name: "build"

            PropertyChanges {
                target: build_view
                visible: true
            }

            PropertyChanges {
                target: menu_build
                state: "selected"
            }

            PropertyChanges {
                target: welcome_view
                visible: false
            }

            PropertyChanges {
                target: project_view
                visible: false
            }

            PropertyChanges {
                target: window_view
                visible: false
            }

            PropertyChanges {
                target: samples_view
                visible: false
            }

            PropertyChanges {
                target: apply_view
                visible: false
            }
        },
        State {
            name: "apply"
            PropertyChanges {
                target: build_view
                visible: true
            }

            PropertyChanges {
                target: welcome_view
                visible: false
            }

            PropertyChanges {
                target: project_view
                visible: false
            }

            PropertyChanges {
                target: window_view
                visible: false
            }

            PropertyChanges {
                target: samples_view
                visible: false
            }

            PropertyChanges {
                target: apply_view
                visible: true
            }

            PropertyChanges {
                target: menu_apply
                state: "selected"
            }
        }
    ]
}
