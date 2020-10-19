# VR实验1报告：迷宫

陶天骅 2017010255

## 任务内容

主要任务为搭建一个可交互的迷宫游戏环境。

本次实验中，我首先在 Windows 上使用 GLFW 实现了一个版本。

然后我将其移植到了 Android 平台上。

## 成果展示

在 Windows 上的运行展示：

![Maze Windows](https://github.com/TianhuaTao/VR-Course-Assignment/blob/master/assignment-1-maze/Maze%20Windows.png?raw=true)
*Windows 运行截图，使用鼠标和 WASD 控制，视角无限制，可以进入物体内部*

在 Android 上的运行展示：

![Maze Android](https://github.com/TianhuaTao/VR-Course-Assignment/blob/master/assignment-1-maze/Maze%20Android.jpg?raw=true)
*Android 运行截图，使用按钮控制，视角限制高度（ y坐标），不可以进入物体内部*

## 运行代码

### Windows 平台

代码中给出了 Visual Studio 工程。编译运行即可。

依赖库（已包括在项目中）：

- GLFW
- gl3w
- glm
- stb_imgae.h

### Android 平台

给出了 Android Studio 工程，编译运行即可。

有以下注意事项：

- 代码使用 OpenGL ES3，在模拟器中运行时，需要修改模拟器设置使其支持 ES3。
- 真机调试时需要设备支持 OpenGL ES3，并且不同设备的驱动支持存在差异。
- 在 Pixel 3a API_30_x86 模拟器上测试可用。
- 在搭载 MIUI Global 11.0.3 的 Mi MIX 2 设备上测试可用。

## 实现参考和素材来源

程序的实现细节和贴图素材来自教程 https://learnopengl.com/ 或者中文版 https://learnopengl-cn.github.io/。

## 实现细节

我之所以首先在 Windows 平台实现，是因为对该平台的 OpenGL 开发更为熟悉。Windows 上使用的是 OpenGL Core Profile，安卓上的为 OpenGL ES3，代码大致相同，移植的时候稍作了一些修改。

### Windows 平台

特性：

- 使用三种光线：平行光、点光源、聚光灯（手电筒）。
- 使用纹理和镜面贴图。
- Camera 类负责视角变换，用欧拉角实现。
- 使用 glm 库完成 CPU 端的矩阵运算。
- 使用 stb_image 读取 png 文件。
- 使用 GLFW 和 gl3w 搭建 OpenGL 环境。

迷宫场景用 1、0 矩阵表示，立方体的绘制逻辑在 ContainerBox 类中。

具体的 GLSL 代码在 `shaders/` 中，贴图在 `textures/` 中。

### Android 平台

Android 平台上除了不使用 GLFW 外，和 Windows 上几乎没有区别。

Android 上的绘制在一个 GLSurfaceView 上完成。虽然在安卓上可以使用 Java 和 Kotlin 编程，但是写 OpenGL 的时候没有比 C++ 更妙的语言了。所以我决定使用 C++ 编写，这样我就可以把 Windows 上的代码和库几乎不动地搬过来，这需要使用 Android NDK。

Java 和 C++ 的接口其实就是 `GLSurfaceView.Renderer` 要重写的三个函数，只需要把它们变成 native 方法，并用 C++ 实现。按钮是在MainActivity 中放在 GLSurfaceView 上层的。

C++ 部分，主要操作就是把代码用 CMake 配置成一个库 MazeGame-lib，就可以被安卓调用。由于不再能使用 GLFW，所以需要新建一个类 MazeGame 来包装游戏逻辑。调用 OpenGL 的时候要 `#include <GLES3/gl3.h>`。贴图和 shaders 保存为 assets，需要稍微写一点代码处理一下才能读取。

## 待补充特性

增加天空贴图和照明。

修改迷宫生成逻辑，可生成任意尺寸迷宫。

增加遮挡阴影。
