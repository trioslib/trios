#include "fileutils.h"
#include <QFileDialog>
#include <stdio.h>

FileUtils::FileUtils(QObject *parent) :
    QObject(parent)
{

}

QString FileUtils::fileDialog() {
    return fileDialog("Choose an image");
}


QString FileUtils::fileDialog(QString caption) {
    return QFileDialog::getOpenFileName(0, caption, "", "ImageFiles (*.pgm)");
}
