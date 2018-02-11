#include "trianglewindow.h"
#include <QGuiApplication>
#include <QtDebug>
#include <QOpenGLDebugMessage>

namespace {
template<class T>
int startApp(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    T window(true);
    window.setTitle(QStringLiteral("LearnOpenGL"));
    QObject::connect(&window, &T::messageLogged, [](const auto &message) { qDebug() << message; });
    window.resize(800, 600);
    window.show();

    window.setAnimation(true);

    return app.exec();
}
}

int main(int argc, char *argv[]) {
    return startApp<TriangleWindow>(argc, argv);
}
