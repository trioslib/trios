#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QObject>

class FileUtils : public QObject
{
    Q_OBJECT
public:
    explicit FileUtils(QObject *parent = 0);
    Q_INVOKABLE QString fileDialog();
    Q_INVOKABLE QString fileDialog(QString caption);

signals:
    
public slots:
    
};

#endif // FILEUTILS_H
