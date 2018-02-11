#include "trianglewindow.h"
#include <QGuiApplication>
#include <QtDebug>
#include <QOpenGLDebugMessage>

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QSurfaceFormat format(QSurfaceFormat::DebugContext);
    format.setSamples(16);
//    format.setMajorVersion(3);
//    format.setMinorVersion(3);
//    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setSwapBehavior(QSurfaceFormat::TripleBuffer);

    TriangleWindow window(true);
    window.setTitle(QStringLiteral("LearnOpenGL"));
    QObject::connect(&window, &TriangleWindow::messageLogged, [](const auto &message) { qDebug() << message; });
    window.setFormat(format);
    window.resize(800, 600);
    window.show();

    window.setAnimation(true);

    return app.exec();
}
