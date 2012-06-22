#include "fileutils.h"
#include <QFileDialog>
#include <stdio.h>

FileUtils::FileUtils(QObject *parent) :
    QObject(parent)
{

}

QString FileUtils::fileOpenDialog() {
    return fileOpenDialog("Choose an image");
}


QString FileUtils::fileOpenDialog(QString caption) {
    return fileOpenDialog(caption, "", "ImageFiles (*.pgm)");
}

QString FileUtils::fileOpenDialog(QString caption, QString path, QString file_type) {
    return QFileDialog::getOpenFileName(0, caption, path, file_type);
}


QString FileUtils::fileSaveDialog(QString caption) {
    return QFileDialog::getSaveFileName(0, caption);
}

QString FileUtils::fileSaveDialog(QString caption, QString path, QString file_type) {
    return QFileDialog::getSaveFileName(0, caption, path, file_type);
}
