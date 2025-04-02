## env
opengl > 4.6, ubuntu version >= 22.04

## ubuntu install the following library: 
``` 
sudo apt-get install libxcursor-dev libx11-dev  libxi-dev libxrandr-dev  libxinerama-dev  mesa-utils ninja-build cmake
sudo apt-get install curl build-essential libfreetype-dev libz-dev pkg-config libglu1-mesa-dev mesa-common-dev  libglfw3-dev libgles2-mesa-dev
```


## Build
Windows: 执行以下指令，在build目录下会有sln(windows)
```
mkdir build
cd build
cmake ..
```

Ubuntu:
```
mkdir build
cd build
cmake ..
make
```

## check opengl version
```
glxinfo | grep "OpenGL version"

```


## 项目编译
在`CMakeLists.txt`中配置`PROJECT_NAME`和`PROJECT_FILE`即可切换生成的项目。`PROJECT_NAME`对应VS中的项目名称以及编译出的exe的名称，`PROJECT_FILE`是项目根目录名称，`INCLUDE_MODULES`添加子模块，`MACRO_NAME`添加预编译宏

Core是引擎目录，MeshPartitioner和OrthoCam是两个示例项目
