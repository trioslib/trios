#ifndef TRIOSINTERFACE_H
#define TRIOSINTERFACE_H

#include <QObject>

class TriosInterface : public QObject
{
    Q_OBJECT
public:
    explicit TriosInterface(QObject *parent = 0);
    Q_INVOKABLE QString apply(QString input_path, QString operator_path);
    
signals:
    
public slots:
    
};

#endif // TRIOSINTERFACE_H
