#ifndef TRIOSINTERFACE_H
#define TRIOSINTERFACE_H

#include <QObject>
#include <QThread>

class TriosInterface : public QObject
{
    Q_OBJECT
public:
    explicit TriosInterface(QObject *parent = 0);
    Q_INVOKABLE QString apply(QString input_path, QString operator_path);
    Q_INVOKABLE QString build(QString window_path, QString imgset_path);

    void send_build_finished(QString op);
    
signals:
    void build_finished(QString operator_path);

    
public slots:
    
};

#endif // TRIOSINTERFACE_H
