#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QObject>

class FileUtils : public QObject
{
    Q_OBJECT
public:
    explicit FileUtils(QObject *parent = 0);
    Q_INVOKABLE QString fileOpenDialog();
    Q_INVOKABLE QString fileOpenDialog(QString caption);
    Q_INVOKABLE QString fileOpenDialog(QString caption, QString path, QString file_type);
    Q_INVOKABLE QString fileSaveDialog(QString caption);
    Q_INVOKABLE QString fileSaveDialog(QString caption, QString path, QString file_type);

signals:
    
public slots:
    
};

#endif // FILEUTILS_H
