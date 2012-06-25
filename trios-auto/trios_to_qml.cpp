#include "trios_to_qml.h"
#include "trios.h"

#include <QDir>
#include <QFile>

#include <iostream>

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
        m.insert("error", false);
    } else {
        m.insert("error", true);
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


bool TRIOS_to_QML::write_window(QVariantMap window, QString path) {
    int width = QVariant(window["width"]).toInt();
    int height = QVariant(window["height"]).toInt();
    QVariantList data = QVariant(window["window_data"]).toList();

    window_t *win = win_create(height, width, 1);
    int k = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++, k++) {
            win_set_point(i, j, 1, data[k].toInt(), win);
        }
    }

    int r = win_write((char *)path.toStdString().c_str(), win);
    return r == 1;
}

bool TRIOS_to_QML::write_imgset(QVariantMap imgset, QString path) {
    int ngroups = imgset.size();
    int grpsize = QVariant(imgset["0"]).toList().length();

    QDir path_dir(path);
    QString filename = path_dir.dirName();
    path_dir.cdUp();
    QString input_path(filename + "-input");
    path_dir.mkdir(input_path);
    QString ideal_path(filename + "-ideal");
    path_dir.mkdir(ideal_path);

    QDir input_dir(path_dir.filePath(input_path));
    QDir ideal_dir(path_dir.filePath(ideal_path));

    imgset_t *is = imgset_create(ngroups, grpsize);
    imgset_set_dname(is, 1, (char *)input_path.toStdString().c_str());
    imgset_set_dname(is, 2, (char *)ideal_path.toStdString().c_str());
    for (int i = 0; i < ngroups; i++) {
        QVariantList group = QVariant(imgset[QString::number(i)]).toList();
        QString input = QVariant(group[0]).toString();
        QString ideal = QVariant(group[1]).toString();

        QDir d(input);
        QFile::copy(input, input_dir.filePath(d.dirName()));
        imgset_set_fname(is, 1, i+1, (char *)d.dirName().toStdString().c_str());
        d = QDir(ideal);
        QFile::copy(ideal, ideal_dir.filePath(d.dirName()));
        imgset_set_fname(is, 2, i+1, (char *)d.dirName().toStdString().c_str());

    }
    int r = imgset_write((char *)path.toStdString().c_str(), is);
    return r == 1;
}
