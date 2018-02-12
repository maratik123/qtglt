#include "trianglewindow.h"
#include <QGuiApplication>
#include <QtDebug>
#include <QOpenGLDebugMessage>
#include <QHash>
#include <QCommandLineParser>
#include <QStringBuilder>

namespace {
template<class T>
int startApp() {
    T window(true);
    window.setTitle(QStringLiteral("LearnOpenGL"));
    QObject::connect(&window, &T::messageLogged, [](const auto &message) { qDebug() << message; });
    window.resize(800, 600);
    window.show();

    window.setAnimation(true);

    return QGuiApplication::exec();
}

class StartupFactory {
public:
    StartupFactory();
    int run(const QString &code) const;
    const QHash<QString, int (*)()> &map() const { return m_map; }
private:
    QHash<QString, int (*)()> m_map;
};

StartupFactory::StartupFactory() {
#define REGISTER_STARTUP(Func) m_map[QStringLiteral(#Func)] = &startApp<Func>

    REGISTER_STARTUP(TriangleWindow);

#undef REGISTER_STARTUP
}

int StartupFactory::run(const QString &code) const {
    if (m_map.contains(code)) {
        return (*m_map[code])();
    }
    qCritical() << "Window:" << code << "is not exists";
    qCritical() << "Exiting";
    return EXIT_FAILURE;
}

const StartupFactory startupFactory;
}

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QGuiApplication::setApplicationName(QStringLiteral("LearnOpenGL"));
    QGuiApplication::setApplicationVersion(QStringLiteral("1.0"));

    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral("Run LearnOpenGL"));
    parser.addHelpOption();
    parser.addVersionOption();

    QString availableWindows(QLatin1Char('['));
    const auto &map = startupFactory.map();
    for(auto it = map.keyBegin(); it != map.keyEnd(); ++it) {
        availableWindows += *it;
    }
    availableWindows += QLatin1Char(']');
    parser.addPositionalArgument(QStringLiteral("window"), QStringLiteral("Window to show"), availableWindows);

    parser.process(app);

    const QStringList &args = parser.positionalArguments();
    const QString &windowName = args.size() > 0
            ? args[0]
            : QStringLiteral("TriangleWindow");

    return startupFactory.run(windowName);
}
