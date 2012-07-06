#ifndef TRIOS_TO_QML_H
#define TRIOS_TO_QML_H

#include <QObject>
#include <QVariant>
#include <QVariantMap>

class TRIOS_to_QML : public QObject
{
    Q_OBJECT
public:
    explicit TRIOS_to_QML(QObject *parent = 0);
    Q_INVOKABLE QVariantMap read_window(QString path);
    Q_INVOKABLE QVariantMap read_imgset(QString path);
    Q_INVOKABLE bool write_window(QVariantMap window, QString path);
    Q_INVOKABLE bool write_imgset(QVariantMap imgset, QString path);
    
signals:
    
public slots:
    
};

#endif // TRIOS_TO_QML_H