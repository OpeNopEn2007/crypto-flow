# Qt Virtual Keyboard

> **来源 URL**: https://doc.qt.io/qt-6/qtvirtualkeyboard-module.html
> **获取日期**: 2026-05-12
> **模块说明**: 虚拟键盘

---

# Qt Virtual Keyboard C++ Classes

Provides classes to implement input methods for virtual keyboards.

## Namespaces

  [QtVirtualKeyboard](qtvirtualkeyboard.html)   Contains enums used by the virtual keyboard classes

## Classes

  [QVirtualKeyboardAbstractInputMethod](qvirtualkeyboardabstractinputmethod.html)   Base class for input methods
  [QVirtualKeyboardDictionary](qvirtualkeyboarddictionary.html)                     An application-defined dictionary for virtual keyboard input methods
  [QVirtualKeyboardDictionaryManager](qvirtualkeyboarddictionarymanager.html)       Dictionary management for application-defined dictionaries
  [QVirtualKeyboardInputContext](qvirtualkeyboardinputcontext.html)                 Provides access to an input context
  [QVirtualKeyboardInputEngine](qvirtualkeyboardinputengine.html)                   InputEngine class provides an input engine that supports C++ and QML integration
  [QVirtualKeyboardObserver](qvirtualkeyboardobserver.html)                         Acts as a hub for keyboard event notifications
  [QVirtualKeyboardSelectionListModel](qvirtualkeyboardselectionlistmodel.html)     List model for selection lists
  [QVirtualKeyboardTrace](qvirtualkeyboardtrace.html)                               Trace is a data model for touch input data

## Detailed Description

To include the definitions of the module's classes, use the following directive:

```
#include <QtVirtualKeyboard>
```

### Using the Module

Using a Qt module requires linking against the module library, either directly or through other dependencies. Several build tools have dedicated support for this, including [CMake](https://cmake.org/cmake/help/latest/) and [qmake](qmake-manual.html).

#### Building with CMake

Use the `find_package()` command to locate the needed module components in the `Qt6` package:

```
find_package(Qt6 REQUIRED COMPONENTS VirtualKeyboard)
target_link_libraries(mytarget PRIVATE Qt6::VirtualKeyboard)
```

See [Getting started with CMake](cmake-get-started.html) for more information.

#### Building with qmake

To link against this module, add the following line to your qmake `.pro` file:

```
QT += virtualkeyboard
```
