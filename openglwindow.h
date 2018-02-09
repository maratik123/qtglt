#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions>

class QOpenGLPaintDevice;
class QOpenGLDebugLogger;
class QOpenGLDebugMessage;

class OpenGLWindow : public QWindow, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit OpenGLWindow(bool enableLogger = false, QWindow *parent = nullptr);
    ~OpenGLWindow();

    virtual void render(const QPainter &/*painter*/) {}
    virtual void render();

    virtual void initialize() {}

    void setAnimation(bool animating);

public slots:
    void renderLater();
    void renderNow();

signals:
    void messageLogged(const QOpenGLDebugMessage &debugMessage);

protected:
    bool event(QEvent *event) override;

    void exposeEvent(QExposeEvent */*event*/) override { renderNow(); }

private:
    bool m_update_pending;
    bool m_animating;
    bool m_enableLogger;

    QOpenGLContext *m_context;
    QOpenGLPaintDevice *m_device;
    QOpenGLDebugLogger *m_logger;
};

#endif // OPENGLWINDOW_H
