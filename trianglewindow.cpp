#include "trianglewindow.h"

#include <QOpenGLShaderProgram>
#include <QScreen>
#include <cmath>

namespace {

static const GLfloat vertices[] = {
    0, std::sqrt(2)/2,
    -0.5, -0.5,
    0.5, -0.5
};

static const GLfloat colors[] = {
    1, 0, 0,
    0, 1, 0,
    0, 0, 1
};

class RenderData {
public:
    RenderData();

    const QMatrix4x4 &baseMatrix() const { return m_baseMatrix; }

private:
    QMatrix4x4 m_baseMatrix;
};

RenderData::RenderData() {
    m_baseMatrix.perspective(60, 4.0 / 3.0, 0.1, 100);
    m_baseMatrix.translate(0, 0, -2);
}

static const RenderData renderData;
}

TriangleWindow::TriangleWindow(bool enableLogger, QWindow *parent)
    : OpenGLWindow(enableLogger, parent)
    , m_program(nullptr)
    , m_frame(0) {
    QSurfaceFormat format(QSurfaceFormat::DebugContext);
    format.setSamples(16);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setSwapBehavior(QSurfaceFormat::TripleBuffer);
    setFormat(format);
}

void TriangleWindow::initialize() {
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, QStringLiteral(":/shaders/vertexshader.vsh"));
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, QStringLiteral(":/shaders/fragmentshader.fsh"));
    m_program->link();
    m_posAttr = m_program->attributeLocation(QStringLiteral("posAttr"));
    m_colAttr = m_program->attributeLocation(QStringLiteral("colAttr"));
    m_matrixUniform = m_program->uniformLocation(QStringLiteral("matrix"));
}

void TriangleWindow::render() {
    const QSize &sz = size() * devicePixelRatio();
    glViewport(0, 0, sz.width(), sz.height());

    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    QMatrix4x4 matrix(renderData.baseMatrix());
    matrix.rotate(100.0 * m_frame / screen()->refreshRate(), 0, 1, 0);

    m_program->setUniformValue(m_matrixUniform, matrix);

    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    m_program->release();

    ++m_frame;
}
