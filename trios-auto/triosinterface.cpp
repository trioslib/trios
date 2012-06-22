#include "triosinterface.h"
#include "trios.h"

TriosInterface::TriosInterface(QObject *parent) :
    QObject(parent)
{
}

QString TriosInterface::apply(QString input_path, QString operator_path) {
    QString result_path = "result.pgm";
    int r = lpapplic((char *)input_path.toStdString().c_str(), (char *)operator_path.toStdString().c_str(), NULL, 0, 0, 255, (char *)result_path.toStdString().c_str());
    if (r == 1) {
        return result_path;
    } else {
        return "";
    }
}
