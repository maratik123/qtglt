#include "openglwindow.h"

#include <QOpenGLPaintDevice>
#include <QCoreApplication>
#include <QPainter>
#include <QOpenGLDebugLogger>

OpenGLWindow::OpenGLWindow(bool enableLogger, QWindow *parent)
    : QWindow(parent)
    , m_update_pending(false)
    , m_animating(false)
    , m_enableLogger(enableLogger)
    , m_context(nullptr)
    , m_device(nullptr)
    , m_logger(nullptr) {
    setSurfaceType(QWindow::OpenGLSurface);
}

void OpenGLWindow::render() {
    if (Q_UNLIKELY(m_device == nullptr)) {
        m_device = new QOpenGLPaintDevice;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_device->setSize(size());

    render(QPainter(m_device));
}

void OpenGLWindow::renderLater() {
    if (Q_LIKELY(!m_update_pending)) {
        m_update_pending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

bool OpenGLWindow::event(QEvent *event) {
    switch (event->type()) {
    case QEvent::UpdateRequest:
        m_update_pending = false;
        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void OpenGLWindow::renderNow() {
    if (Q_UNLIKELY(!isExposed())) {
        return;
    }

    if (Q_UNLIKELY(m_context == nullptr)) {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        m_context->create();
        m_context->makeCurrent(this);

        if(m_enableLogger) {
            m_logger = new QOpenGLDebugLogger(this);
            if (m_logger->initialize()) {
                connect(m_logger, &QOpenGLDebugLogger::messageLogged, this, &OpenGLWindow::messageLogged);
                m_logger->startLogging();
                for (const auto &message : m_logger->loggedMessages()) {
                    emit messageLogged(message);
                }
            }
        }

        initializeOpenGLFunctions();
        initialize();
    } else {
        m_context->makeCurrent(this);
    }

    render();

    m_context->swapBuffers(this);

    if (Q_LIKELY(m_animating)) {
        renderLater();
    }
}

void OpenGLWindow::setAnimation(bool animating) {
    m_animating = animating;

    if (animating) {
        renderLater();
    }
}

OpenGLWindow::~OpenGLWindow() {
    delete m_device;
}
