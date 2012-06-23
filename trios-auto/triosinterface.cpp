#include "triosinterface.h"
#include "trios.h"
#include "misc.h"

#include <QDir>

TriosInterface::TriosInterface(QObject *parent) :
    QObject(parent)
{
}

QString TriosInterface::apply(QString input_path, QString operator_path) {

    QString result_path = "result.pgm";
    QDir dir;
    dir.remove("result.pgm");
    int r = lpapplic((char *)input_path.toStdString().c_str(), (char *)operator_path.toStdString().c_str(), NULL, 0, 0, 255, (char *)result_path.toStdString().c_str());
    if (r == 1) {
        return dir.absoluteFilePath(result_path);
    } else {
        return "";
    }
}

QString TriosInterface::build(QString window_path, QString imgset_path) {
    char *w_path = fromQString(window_path);
    char *s_path = fromQString(imgset_path);
    if (!lcollec(s_path, w_path, NULL, 1, 1, 0, "result.xpl", NULL)) {
        return "";
    }
    if (!ldecision("result.xpl",  1, 0, AVERAGE, 0, 0, "decision.mtm")) {
        return "";
    }
    window_t *w = win_read(w_path);
    itv_t *i = itv_gen_itv(w, 1, BB, 0, 1, 0);
    itv_write("itv_temp.itv", i, w);
    win_free(w);
    itv_free(i);
    if (!lisi("decision.mtm", "itv_temp.itv", 3, 5, 0, 0, "itv_final.itv", 0, NULL, NULL)) {
        return "";
    }
    return "itv_final.itv";
}
