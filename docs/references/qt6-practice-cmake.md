# Qt 6 与 CMake 构建系统集成最佳实践

> 来源: https://doc.qt.io/qt-6/cmake-get-started.html
> 获取日期: 2026-05-12

---

## 一、基础：控制台应用 CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.16)

project(helloworld VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

qt_add_executable(helloworld
    main.cpp
)

target_link_libraries(helloworld PRIVATE Qt6::Core)
```

### 关键命令说明

| 命令 | 作用 |
|------|------|
| `cmake_minimum_required(VERSION 3.16)` | 指定最低 CMake 版本 |
| `project()` | 设置项目名和版本 |
| `find_package(Qt6 REQUIRED COMPONENTS Core)` | 查找 Qt 6 Core 模块 |
| `qt_standard_project_setup()` | 设置项目全局默认值（自动开启 AUTOMOC） |
| `qt_add_executable()` | 创建可执行目标（包装 add_executable） |
| `target_link_libraries()` | 链接 Qt 模块 |

### 注意事项
- Qt 6 要求 C++17 或更新的编译器
- `qt_standard_project_setup()` 自动设置 `CMAKE_AUTOMOC=ON`
- 头文件无需在 `qt_add_executable()` 中列出（不同于 qmake）

---

## 二、GUI 应用 (Qt Widgets)

```cmake
cmake_minimum_required(VERSION 3.16)

project(helloworld VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(helloworld
    mainwindow.ui
    mainwindow.cpp
    main.cpp
)

target_link_libraries(helloworld PRIVATE Qt6::Widgets)

set_target_properties(helloworld PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
```

### 关键变化
- `find_package` 使用 `Widgets` 替代 `Core`
- 添加 `.ui` 文件（Qt Designer 文件）
- `qt_standard_project_setup()` 同时开启 `CMAKE_AUTOUIC=ON`（自动处理 .ui 文件）
- `set_target_properties` 设置平台属性：Windows 隐藏控制台窗口，macOS 创建 bundle

---

## 三、项目结构（子目录模式）

### 顶层 CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.16)

project(helloworld VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_subdirectory(src/app)
add_subdirectory(src/businesslogic)
```

### 应用子目录 src/app/CMakeLists.txt

```cmake
qt_add_executable(helloworld
    mainwindow.ui
    mainwindow.cpp
    main.cpp
)

target_link_libraries(helloworld PRIVATE
    businesslogic
    Qt6::Widgets
)

set_target_properties(helloworld PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
```

---

## 四、构建静态库

### src/businesslogic/CMakeLists.txt

```cmake
qt_add_library(businesslogic STATIC
    businesslogic.cpp
)

target_link_libraries(businesslogic PRIVATE Qt6::Core)

target_include_directories(businesslogic INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
```

### 使用库

```cmake
target_link_libraries(helloworld PRIVATE
    businesslogic
    Qt6::Widgets
)
```

- 使用 `STATIC` 创建静态库，`SHARED` 创建动态库
- `target_include_directories` 使用 `INTERFACE` 关键字让消费方自动获取头文件路径
- 使用时只需 `#include <businesslogic.h>` 即可

---

## 五、添加资源文件

```cmake
qt_add_resources(helloworld imageresources
    PREFIX "/images"
    FILES logo.png splashscreen.png
)
```

C++ 中访问资源：
```cpp
logoLabel->setPixmap(QPixmap(":/images/logo.png"));
```

---

## 六、添加国际化翻译

```cmake
qt_standard_project_setup(I18N_TRANSLATED_LANGUAGES de fr)

qt_add_translations(helloworld)
```

构建命令：
```bash
# 更新翻译文件
cmake --build . --target update_translations

# 生成 .qm 文件
cmake --build . --target release_translations
```

---

## 七、Qt 6 CMake 核心 API 速查

| CMake 命令 | 用途 |
|-----------|------|
| `qt_standard_project_setup()` | 设置全局默认值（AUTOMOC, AUTOUIC 等） |
| `qt_add_executable()` | 创建可执行文件 |
| `qt_add_library()` | 创建库（STATIC/SHARED） |
| `qt_add_qml_module()` | 添加 QML 模块 |
| `qt_add_resources()` | 添加 Qt 资源 |
| `qt_add_translations()` | 添加翻译支持 |
| `qt_add_ui()` | 处理 .ui 文件（替代 AUTOUIC） |
| `qt_generate_deploy_app_script()` | 生成部署脚本 |

---

## 八、常见 CMake 模式

### 条件编译

```cmake
if(QT_VERSION_MAJOR EQUAL 6)
    qt_standard_project_setup()
endif()
```

### 多平台构建

```cmake
if(WIN32)
    set(APP_ICON_RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/app.rc")
elseif(APPLE)
    set(APP_ICON_RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/app.icns")
endif()
```

### 设置应用版本

```cmake
project(MyApp VERSION 1.2.3 LANGUAGES CXX)

target_compile_definitions(MyApp PRIVATE
    APP_VERSION="${PROJECT_VERSION}"
)
```

---

## 九、推荐阅读

- Qt CMake 入门: https://doc.qt.io/qt-6/cmake-get-started.html
- Qt CMake 手册: https://doc.qt.io/qt-6/cmake-manual.html
- Professional CMake: A Practical Guide (书籍)
- Modern CMake with C++ (Henry Schreiner)
