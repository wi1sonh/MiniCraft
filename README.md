# MiniCraft
A Minecraft like voxel game based only on OpenGL and Qt

## Updates

### 11.22 - Initial commit

Overview: 实现了一个随鼠标转动视角的，可以移动摄像头的蓝天绿地场景

Teamwork: 之后的每次代码更新提供类似下文的文件说明 接口说明

Updates:

框架

```cpp
main
glwidget  // 继承 QtOpenGLWidget 的应用主类
mygl.
    class MyGL; // 继承 glwidget 的应用主类，包含所有全局对象与变量
        GLDrawScene(); // 主绘制
                       // 目前是用 Scene 的01三维张量，用 lambert 着色器绘制例子 geom_cube
camera.
    class Camera. // perspective-projection 视角框架
shaderprogram.
    class ShaderProgram. // 着色器源代码-对象 vert + frag
        attrPos/Nor/Col; // AttrPointer用于传输输入变量
        setXXX();        // UniformPointer用于设置uniform变量：模型矩阵、视图投影矩阵、几何体颜色
        draw(Drawable &d); // 输入 Drawable 对象的 VBO 绑定到对应 AttrPointer 后
                           // 调用 context->glDrawElements 绘制
drawable.
    class Drawable. // 任何几何对象都应当继承它，以让Shader绘制
        VBO of Position, Normal, Color;
        element count;
utils
la // convert glm data types to Qt data types
    
GLSL\  // 着色器源代码
    // 无光照
    flat.frag
    flat.vert 
    // Lambert 光照模型（完全漫反射表面）
    lambert.vert  // 输入顶点数据，把顶点+法线传递给片段着色器，输出矩阵变换后的顶点
    lambert.frag  // 输入顶点颜色+顶点法线+光源反向，计算颜色=顶点颜色*(漫反射项+环境光照项)
```

scene\

```cpp
scene.
    class Scene.
        QList<QList<QList<bool>>> objects; // 目前只是01的三维张量
        glm::ivec3 dimensions; // 有限世界大小
        CreateTestScene();
transform.
    class Transform; // 不懂
cube.
    class Cube.  // Drawable
        create(); // 创建立方体的顶点位置VBO，顶点法向量VBO，颜色VBO（目前都是绿的），顶点索引EBO
                  // 使用Drawable父类函数绑定bufferObj
```

