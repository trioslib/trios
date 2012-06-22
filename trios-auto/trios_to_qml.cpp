#include "trios_to_qml.h"
#include "trios.h"

TRIOS_to_QML::TRIOS_to_QML(QObject *parent) :
    QObject(parent)
{
}

QVariantMap TRIOS_to_QML::read_window(QString path) {
    QVariantMap m;
    window_t *win = win_read((char *)path.toStdString().c_str());
    if (win != NULL) {
        m.insert("width", win_get_width(win));
        m.insert("height", win_get_height(win));
        QVariantList rows;
        for (int i = 0; i < win_get_height(win); i++) {
            QVariantList n;
            for (int j = 0; j < win_get_width(win); j++) {
                n.append(QVariant(win_get_point(i, j, 1, win)));
            }
            rows.append(n);
        }
        m.insert("window_data", rows);
    }
    return m;
}
