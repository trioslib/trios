var window_button_component = null;

function clear_grid() {
    var i;
    for (i = 0; i < root.children.length; i++) {
        root.children[i].destroy();
    }
}

function create_grid() {
    var i, j;
    for (i = 0; i < root.window_height; i++) {
        for (j = 0; j < root.window_width; j++) {
            var box = window_button_component.createObject(root, {"i": i, "j": j, "selected": true});
        }
    }

//    var cmp =
}

function update() {
    if (window_button_component == null) {
        window_button_component = Qt.createComponent("WindowConfigurationButton.qml");
        if (window_button_component == null) {
            console.log("ERRO!");
        }
    }
    clear_grid();
    create_grid();
}
