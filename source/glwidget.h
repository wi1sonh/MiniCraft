#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_2_Core>
#include <QTimer>


class GLWidget : public QOpenGLWidget, public QOpenGLFunctions_3_2_Core
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent);
    ~GLWidget();

    void printGLErrorLog();
    void printLinkInfoLog(int prog);
    void printShaderInfoLog(int shader);

private slots:

    virtual void timerUpdate(){}

};


#endif // GLWIDGET_H
