#include "trianglewindow.h"
#include <QGuiApplication>
#include <QtDebug>
#include <QOpenGLDebugMessage>

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);

    TriangleWindow window(true);
    QObject::connect(&window, &TriangleWindow::messageLogged, [](const auto &message) { qDebug() << message; });
    window.setFormat(format);
    window.resize(640, 480);
    window.show();

    window.setAnimation(true);

    return app.exec();
}
