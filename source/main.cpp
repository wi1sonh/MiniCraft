#include <QApplication>
#include <QSurfaceFormat>
#include <QDebug>
#include "mygl.h"


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MyGL w;
    w.setGeometry(400, 300, 1000, 600); // 设置窗口位置和大小
    w.setWindowTitle("MiniCraft");  // 设置窗口标题

    // Set OpenGL 3.2 and, optionally, 4-sample multisampling
    QSurfaceFormat format;
    format.setVersion(3, 2);
    format.setOption(QSurfaceFormat::DeprecatedFunctions, false);
    format.setProfile(QSurfaceFormat::CoreProfile);
    //format.setSamples(4);  // Uncomment for nice antialiasing. Not always supported.


    QSurfaceFormat::setDefaultFormat(format);

    w.show();

    return a.exec();
}
