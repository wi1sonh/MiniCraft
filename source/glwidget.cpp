#include "glwidget.h"
#include <utils.h>

#include <iostream>
#include <QApplication>
#include <QProcessEnvironment>
#include <QOpenGLContext>
#include <QDebug>


GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

GLWidget::~GLWidget()
{
}


void GLWidget::printGLErrorLog()
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error " << error << ": ";
        const char *e =
            error == GL_INVALID_OPERATION             ? "GL_INVALID_OPERATION" :
            error == GL_INVALID_ENUM                  ? "GL_INVALID_ENUM" :
            error == GL_INVALID_VALUE                 ? "GL_INVALID_VALUE" :
            error == GL_INVALID_INDEX                 ? "GL_INVALID_INDEX" :
            error == GL_INVALID_OPERATION             ? "GL_INVALID_OPERATION" :
            QString::number(error).toUtf8().constData();
        std::cerr << e << std::endl;
        // Throwing here allows us to use the debugger to track down the error.

        throw;
    }
}

void GLWidget::printLinkInfoLog(int prog)
{
    GLint linked;
    glGetProgramiv(prog, GL_LINK_STATUS, &linked);
    if (linked == GL_TRUE) {
        return;
    }
    std::cerr << "GLSL LINK ERROR" << std::endl;

    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar *infoLog;

    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &infoLogLen);

    if (infoLogLen > 0) {
        infoLog = new GLchar[infoLogLen];
        // error check for fail to allocate memory omitted
        glGetProgramInfoLog(prog, infoLogLen, &charsWritten, infoLog);
        std::cerr << "InfoLog:" << std::endl << infoLog << std::endl;
        delete[] infoLog;
    }
    // Throwing here allows us to use the debugger to track down the error.
    throw;
}

void GLWidget::printShaderInfoLog(int shader)
{
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_TRUE) {
        return;
    }
    std::cerr << "GLSL COMPILE ERROR" << std::endl;

    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar *infoLog;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

    if (infoLogLen > 0) {
        infoLog = new GLchar[infoLogLen];
        // error check for fail to allocate memory omitted
        glGetShaderInfoLog(shader, infoLogLen, &charsWritten, infoLog);
        std::cerr << "InfoLog:" << std::endl << infoLog << std::endl;
        delete[] infoLog;
    }
    // Throwing here allows us to use the debugger to track down the error.
    throw;
}
