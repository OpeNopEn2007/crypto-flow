# Qt WebEngine Quick

> **来源**: [https://doc.qt.io/qt-6/qtwebenginequick-module.html](https://doc.qt.io/qt-6/qtwebenginequick-module.html)
> **获取日期**: 2026-05-12
> **描述**: Web 引擎 Quick 模块，提供 QML 中嵌入 Web 内容的类型

# Qt WebEngine Quick C++ Classes

Exposes C++ functionality to Qt Quick.

## Namespaces

  [QtWebEngineQuick](qtwebenginequick.html)   Helper functions for the Qt WebEngine (Qt Quick) module

## Classes

  [QQuickWebEngineProfile](qquickwebengineprofile.html)   Web engine profile shared by multiple pages

## Detailed Description

The Qt WebEngine Quick module exposes C++ functionality to Qt Quick.

To link against the module, add the following to your qmake project file:

```
QT += webenginequick
```

For build with CMake use the `find_package()` command to locate the needed module components in the Qt6 package and `target_link_libraries()` to link against the module:

```
find_package(Qt6 REQUIRED COMPONENTS WebEngineQuick)
target_link_libraries(target PRIVATE Qt6::WebEngineQuick)
```
