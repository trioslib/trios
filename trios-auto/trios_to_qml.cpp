#include "trios_to_qml.h"
#include "trios.h"

TRIOS_to_QML::TRIOS_to_QML(QObject *parent) :
    QObject(parent)
{
}

QVariantMap TRIOS_to_QML::read_window(QString path) {
    QVariantMap m;
    window_t *win = win_read((char *)path.toStdString().c_str());
    m.insert("width", 3);
    m.insert("height", 3);

    return m;
}
