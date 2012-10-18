#include "triosinterface.h"
#include "trios.h"
#include "misc.h"

#include <iostream>

#include <QDir>
#include <QtConcurrentRun>

TriosInterface::TriosInterface(QObject *parent) :
    QObject(parent)
{
}

QString TriosInterface::apply(QString input_path, QString operator_path) {

    QString result_path = "result.pgm";
    QDir dir;
    dir.remove("result.pgm");
    int r = lpapplic_disk((char *)input_path.toStdString().c_str(), (char *)operator_path.toStdString().c_str(), NULL, 0, 0, 255, (char *)result_path.toStdString().c_str());
    if (r == 1) {
        return dir.absoluteFilePath(result_path);
    } else {
        return "";
    }
}

void build_run(char *w_path, char *s_path, TriosInterface *trios) {
    if (!lcollec(s_path, w_path, NULL, 1, 1, 0, "result.xpl", NULL)) {
        trios->send_build_finished("");
    }
    if (!ldecision_disk("result.xpl",  1, 0, AVERAGE, 0, 0, "decision.mtm")) {
        trios->send_build_finished("");
    }
    window_t *w = win_read(w_path);
    itv_t *i = itv_gen_itv(w, 1, BB, 0, 1, 0);
    itv_write("itv_temp.itv", i, w);
    win_free(w);
    itv_free(i);
    if (!lisi_disk("decision.mtm", "itv_temp.itv", 3, 5, 0, 0, "itv_final.itv", 0, NULL, NULL)) {
        trios->send_build_finished("");
    }
    trios->send_build_finished("itv_final.itv");

}

QString TriosInterface::build(QString window_path, QString imgset_path) {
    char *w_path = fromQString(window_path);
    char *s_path = fromQString(imgset_path);
    std::cout << "WWWWW" << std::endl;
    QtConcurrent::run(build_run, w_path, s_path, this);
    return "";
}

void TriosInterface::send_build_finished(QString op) {
    emit build_finished(op);
}
