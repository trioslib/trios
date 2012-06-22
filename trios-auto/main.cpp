#include <QtGui/QApplication>
#include <QtDeclarative>
#include "qmlapplicationviewer.h"
#include "fileutils.h"
#include "trios_to_qml.h"
#include "triosinterface.h"
#include "trios.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    QmlApplicationViewer viewer;
    FileUtils fu;
    viewer.rootContext()->setContextProperty("fileUtils", &fu);

    TRIOS_to_QML t2qml;
    viewer.rootContext()->setContextProperty("trios2qml", &t2qml);

    TriosInterface ti;
    viewer.rootContext()->setContextProperty("trios", &ti);

    viewer.setWindowTitle("TRIOS - Automatic image operator design tool");
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/triosauto/main.qml"));
    viewer.showExpanded();

    return app->exec();
}
