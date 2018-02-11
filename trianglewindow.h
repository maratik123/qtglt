#ifndef TRIANGLEWINDOW_H
#define TRIANGLEWINDOW_H

#include "openglwindow.h"

class QOpenGLShaderProgram;

class TriangleWindow : public OpenGLWindow {
    Q_OBJECT

public:
    explicit TriangleWindow(bool enableLogger = false, QWindow *parent = nullptr);

    void initialize() override;
    void render() override;

private:
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

    QOpenGLShaderProgram *m_program;
    int m_frame;
};

#endif // TRIANGLEWINDOW_H
