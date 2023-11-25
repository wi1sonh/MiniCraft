#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>


MyGL::MyGL(QWidget *parent) : GLWidget(parent),
      geom_cube(this),
      prog_lambert(this), prog_flat(this), mousemove(false),
      gl_camera()
{
    // Connect the timer to a function so that when the timer ticks the function is executed
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    // Tell the timer to redraw 60 times per second
    timer.start(16);
    //setFocusPolicy(Qt::ClickFocus);

    setCursor(Qt::BlankCursor); // 隐藏光标
    setMouseTracking(true); // 捕获鼠标移动事件
    setFocusPolicy(Qt::StrongFocus); // 焦点策略
}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    geom_cube.destroy();
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context


    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.37f, 0.74f, 1.0f, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    //Create the instance of Cube
    geom_cube.create();

    // Create and set up the diffuse shader
    prog_lambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    prog_flat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    // Set a color with which to draw geometry since you won't have one
    // defined until you implement the Node classes.
    // This makes your geometry render green.
    prog_lambert.setGeometryColor(glm::vec4(0,1,0,1));

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
//    vao.bind();
    glBindVertexArray(vao);

    scene.CreateTestScene();
}

void MyGL::resizeGL(int w, int h)
{
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.

    gl_camera = Camera(w, h,
                        glm::vec3(scene.dimensions.x/2, scene.dimensions.y/2 + 2, scene.dimensions.z/2 ),
                        glm::vec3(scene.dimensions.x/2, scene.dimensions.y/2 + 2, scene.dimensions.z/2 + 1),
                        glm::vec3(0,1,0));
    glm::mat4 viewproj = gl_camera.getViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    prog_lambert.setViewProjMatrix(viewproj);
    prog_flat.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function updateGL is called, paintGL is called implicitly.
//DO NOT CONSTRUCT YOUR SCENE GRAPH IN THIS FUNCTION!
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    prog_flat.setViewProjMatrix(gl_camera.getViewProj());
    prog_lambert.setViewProjMatrix(gl_camera.getViewProj());

    GLDrawScene();
}

void MyGL::GLDrawScene()
{
    for(int x = 0; x < scene.objects.size(); x++)
    {
        QList<QList<bool>> Xs = scene.objects[x];
        for(int y = 0; y < Xs.size(); y++)
        {
            QList<bool> Ys = Xs[x];
            for(int z = 0; z < Ys.size(); z++)
            {
                if(Ys[z])
                {
                    prog_lambert.setModelMatrix(glm::translate(glm::mat4(), glm::vec3(y, x, z)));
                    prog_lambert.draw(geom_cube);
                }
            }
        }
    }
}


void MyGL::keyPressEvent(QKeyEvent *e)
{

    float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 5.0f;
    }

    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    }
    if (e->key() == Qt::Key_1) {
        gl_camera.fovy += amount;
    }
    if (e->key() == Qt::Key_2) {
        gl_camera.fovy -= amount;
    }
    if (e->key() == Qt::Key_W) {
        gl_camera.TranslateAlongLook(amount);
    }
    if (e->key() == Qt::Key_S) {
        gl_camera.TranslateAlongLook(-amount);
    }
    if (e->key() == Qt::Key_D) {
        gl_camera.TranslateAlongRight(amount);
    }
    if (e->key() == Qt::Key_A) {
        gl_camera.TranslateAlongRight(-amount);
    }
    if (e->key() == Qt::Key_F) {
        gl_camera.TranslateAlongUp(-amount);
    }
    if (e->key() == Qt::Key_Space) {
        gl_camera.TranslateAlongUp(amount);
    }
    if (e->key() == Qt::Key_R) {
        gl_camera = Camera(this->width(), this->height());
    }
    gl_camera.RecomputeAttributes();

    update();  // Calls paintGL, among other things
}

void MyGL::mouseMoveEvent(QMouseEvent* event)
{
    if (!mousemove)
    {
        mousemove = true;
        mouse_oldpos = event->pos();
        return;
    }

    if (mouse_oldpos.x() == width() - 1 || mouse_oldpos.x() == 0\
        || mouse_oldpos.y() == height() - 1 || mouse_oldpos.y() == 0)
    {
        QCursor::setPos(width() / 2, height() / 2);
        mouse_oldpos = QPoint(width() / 2, height() / 2);
        return;
    }

    QPoint p = event->pos() - mouse_oldpos;
    mouse_oldpos = event->pos();

    float delta_w = 2 * float(p.x()) / float(width()) * glm::length(gl_camera.H);
    float delta_h = 2 * float(p.y()) / float(height()) * glm::length(gl_camera.V);
    float theta = atan(delta_w / glm::length(gl_camera.ref - gl_camera.eye)) * 180;
    float fai = atan(delta_h / glm::length(gl_camera.ref - gl_camera.eye)) * 180;

    gl_camera.RotateAboutUp(-theta);
    gl_camera.RotateAboutRight(-fai);
    gl_camera.RecomputeAttributes();

    gl_skyboxCamera.RotateAboutUp(-theta);
    gl_skyboxCamera.RotateAboutRight(-fai);

    update();
}

void MyGL::timerUpdate()
{
}
