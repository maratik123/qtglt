#include "trianglewindow.h"
#include <QGuiApplication>
#include <QtDebug>
#include <QOpenGLDebugMessage>
#include <QHash>
#include <QCommandLineParser>
#include <QStringBuilder>
#include <functional>

namespace {

template<class T>
int startApp(const QString &applicationName) {
    T window(true);
    window.setTitle(applicationName);
    QObject::connect(&window, &T::messageLogged, [](const auto &message) { qDebug() << message; });
    window.resize(800, 600);
    window.show();

    window.setAnimation(true);

    return QGuiApplication::exec();
}

class StartupFactory {
public:
    explicit StartupFactory(const QString &defaultWindow);
    int run(const QString &code, const QString &applicationName) const;
    QString concatenateKeys(const QString &delimiter, const QString &prefix = QString(), const QString &postfix = QString()) const;
private:
    int concatenatedKeysSize(const QString &delimiter, const QString &prefix, const QString &postfix) const;

    QHash<QString, std::function<int(const QString &)>> m_map;
};

#define REGISTER_STARTUP(Func) std::make_pair(QStringLiteral(#Func), startApp<Func>)

StartupFactory::StartupFactory(const QString &defaultWindow) :
    m_map({
        REGISTER_STARTUP(TriangleWindow),
        std::make_pair(defaultWindow, startApp<TriangleWindow>)
        }) {}

#undef REGISTER_STARTUP

int StartupFactory::run(const QString &code, const QString &applicationName) const {
    if (m_map.contains(code)) {
        return m_map[code](applicationName);
    }
    qCritical() << "Window:" << code << "is not exists";
    qCritical() << "Exiting";
    return EXIT_FAILURE;
}

int StartupFactory::concatenatedKeysSize(const QString &delimiter, const QString &prefix, const QString &postfix) const {
    int result = prefix.size() + (m_map.size() - 1) * delimiter.size() + postfix.size();
    const auto &end = m_map.cend();
    for (auto it = m_map.cbegin(); it != end; ++it) {
        result += it.key().size();
    }
    return result;
}

QString StartupFactory::concatenateKeys(const QString &delimiter, const QString &prefix, const QString &postfix) const {
    QString availableWindows;
    availableWindows.reserve(concatenatedKeysSize(delimiter, prefix, postfix));
    availableWindows += prefix;
    const auto &end = m_map.cend();
    auto it = m_map.cbegin();
    if (it != end) {
        availableWindows += it.key();
        for(++it; it != end; ++it) {
            availableWindows += delimiter % it.key();
        }
    }
    availableWindows += postfix;
    return availableWindows;
}

}

int main(int argc, char *argv[]) {
    const QGuiApplication app(argc, argv);

    const QString &applicationName = QStringLiteral("LearnOpenGL");
    const QString &defaultWindow = QStringLiteral("Default");

    const StartupFactory startupFactory(defaultWindow);

    QGuiApplication::setApplicationName(applicationName);
    QGuiApplication::setApplicationVersion(QStringLiteral("1.0"));

    QCommandLineParser parser;
    parser.setApplicationDescription(QLatin1String("Run ") % applicationName);
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument(
                QStringLiteral("window"),
                QLatin1String("Window to show: \n\t  ") % startupFactory.concatenateKeys(QStringLiteral("\n\t| ")),
                QStringLiteral("[window]"));

    parser.process(app);

    const QStringList &args = parser.positionalArguments();
    const QString &windowName = args.size() > 0 ? args[0] : defaultWindow;

    return startupFactory.run(windowName, applicationName);
}
