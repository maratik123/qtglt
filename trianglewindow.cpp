#include "trianglewindow.h"

#include <QOpenGLShaderProgram>
#include <QScreen>
#include <cmath>

namespace {
static const char *vertexShaderSource =
        "attribute highp vec4 posAttr;\n"
        "attribute lowp vec4 colAttr;\n"
        "varying lowp vec4 col;\n"
        "uniform highp mat4 matrix;\n"
        "void main() {\n"
        "  col = colAttr;\n"
        "  gl_Position = matrix * posAttr;\n"
        "}\n";

static const char *fragmentShaderSource =
        "varying lowp vec4 col;\n"
        "void main() {\n"
        "  gl_FragColor = col;\n"
        "}\n";

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
    , m_program(0)
    , m_frame(0) {
}

void TriangleWindow::initialize() {
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");
}

void TriangleWindow::render() {
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

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