#include "trianglewindow.h"
#include <QGuiApplication>
#include <QtDebug>
#include <QOpenGLDebugMessage>

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    TriangleWindow window(true);
    window.setTitle(QStringLiteral("LearnOpenGL"));
    QObject::connect(&window, &TriangleWindow::messageLogged, [](const auto &message) { qDebug() << message; });
    window.resize(800, 600);
    window.show();

    window.setAnimation(true);

    return app.exec();
}
