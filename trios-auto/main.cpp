#include <QtGui/QApplication>
#include <QtDeclarative>
#include "qmlapplicationviewer.h"
#include "fileutils.h"

#include "trios.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    QmlApplicationViewer viewer;
    FileUtils fu;
    viewer.rootContext()->setContextProperty("fileUtils", &fu);

    viewer.setWindowTitle("TRIOS - Automatic image operator design tool");
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/triosauto/main.qml"));
    viewer.showExpanded();

    window_t *w = win_create(4, 4, 1);

    win_free(w);

    return app->exec();
}
