#include "trios_to_qml.h"
#include "trios.h"

#include <QDir>

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

QVariantMap TRIOS_to_QML::read_imgset(QString path) {
    QVariantMap m;
    imgset_t *imgset = imgset_read((char *)path.toStdString().c_str());
    QDir file_dir(path);
    file_dir.cdUp();
    if (imgset) {
        for (int i = 0; i < imgset_get_ngroups(imgset); i++) {
            QVariantList images;
            for (int j = 0; j < imgset_get_grpsize(imgset); j++) {
                char *str = imgset_get_ffullname(imgset, j+1, i+1);
                QDir img_dir(str);
                if (!img_dir.isAbsolute()) {
                    QString path = file_dir.absoluteFilePath(QString(str));
                    images.append(path.toStdString().c_str());
                } else {
                    images.append(img_dir.absolutePath().toStdString().c_str());
                }
            }
            m.insert(QString::number(i), images);
        }
    }
    return m;
}
