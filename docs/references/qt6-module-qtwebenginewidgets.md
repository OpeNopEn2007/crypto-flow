# Qt WebEngine Widgets

> **来源**: [https://doc.qt.io/qt-6/qtwebenginewidgets-module.html](https://doc.qt.io/qt-6/qtwebenginewidgets-module.html)
> **获取日期**: 2026-05-12
> **描述**: Web 引擎 Widgets 模块，提供在 Qt Widgets 应用中嵌入 Web 内容的组件

# Qt WebEngine Widgets C++ Classes

Provides C++ classes for rendering web content in a [QWidget](qwidget.html) based application.

## Classes

  [QWebEngineView](qwebengineview.html)   Widget that is used to view and edit web documents

## Detailed Description

The Qt WebEngine Widgets module provides a web browser engine as well as C++ classes to render and interact with web content.

To link against the module using build with qmake, add the following QT variable to your qmake .pro file:

```
QT += webenginewidgets
```

For build with CMake use the `find_package()` command to locate the needed module components in the Qt6 package and `target_link_libraries()` to link against the module:

```
find_package(Qt6 REQUIRED COMPONENTS WebEngineWidgets)
target_link_libraries(target PRIVATE Qt6::WebEngineWidgets)
```

The minimum amount of code needed to load and display an HTML page requires just implementing the `QWebEngineView` class.

```
#include <QApplication>
#include <QWebEngineView>

int main(int argc, char *argv)

```
