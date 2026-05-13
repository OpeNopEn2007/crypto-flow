---
source: https://doc.qt.io/qt-6/qtwidgets-index.html
fetched: 2026-05-12
---

# Qt Widgets

The [Qt Widgets Module](qtwidgets-module.html) provides a set of UI elements to create classic desktop-style user interfaces. See the [User Interfaces](topics-ui.html) overview for more information on using widgets.

## Widgets

Widgets are the primary elements for creating user interfaces in Qt. [Widgets](widget-classes.html#the-widget-classes) can display data and status information, receive user input, and provide a container for other widgets that should be grouped together. A widget that is not embedded in a parent widget is called a [window](application-windows.html).

![Parent widget containing various child widgets](images/parent-child-widgets.png)

The [QWidget](qwidget.html) class provides the basic capability to render to the screen and to handle user input events. All UI elements that Qt provides are either subclasses of [QWidget](qwidget.html) or are used in connection with a [QWidget](qwidget.html) subclass. To create custom widgets, subclass [QWidget](qwidget.html) or a suitable subclass and reimplement the virtual event handlers.

- [Window and Dialog Widgets](application-windows.html)
- [Application Main Window](mainwindow.html)
- [Dialog Windows](dialogs.html)
- [Keyboard Focus in Widgets](focus.html)

## Styles

[Styles](style-reference.html) draw on behalf of widgets and encapsulate the look and feel of a GUI. Qt\'s built-in widgets use the [QStyle](qstyle.html) class to perform nearly all of their drawing, ensuring that they look exactly like the equivalent native widgets.

  ----------------------------------------------------------------- ------------------------------------------------------------------ --------------------------------------------------------
  ![Windows style tab widgets](images/windowsvista-tabwidget.png)   ![Fusion desktop style tab widgets](images/fusion-tabwidget.png)   ![macOS style tab widgets](images/macos-tabwidget.png)
  ----------------------------------------------------------------- ------------------------------------------------------------------ --------------------------------------------------------

[Qt Style Sheets](stylesheet.html) are a powerful mechanism that lets you customize the appearance of widgets, in addition to what is already possible by subclassing [QStyle](qstyle.html).

## Layouts

[Layouts](layout.html) are an elegant and flexible way to automatically arrange child widgets within their container. Each widget reports its size requirements to the layout through the [sizeHint](qwidget.html#sizeHint-prop) and [sizePolicy](qwidget.html#sizePolicy-prop) properties, and the layout distributes the available space accordingly.

  -------------------------------------------------------------- --------------------------------------------------------------
  ![5-children layout](images/qgridlayout-with-5-children.png)   ![6-children layout](images/qformlayout-with-6-children.png)
  -------------------------------------------------------------- --------------------------------------------------------------

[Qt Widgets Designer](qtdesigner-manual.html) is a powerful tool for interactively creating and arranging widgets in layouts.

## Model/View Classes

The [model/view](model-view-programming.html) architecture provides classes that manage the way data is presented to the user. Data-driven applications which use lists and tables are structured to separate the data and view using models, views, and delegates.

![File directory using a tree view](images/fusion-treeview.png)

## Graphics View

The [Graphics View Framework](graphicsview.html) is for managing and interacting with a large number of custom-made 2D graphical items, and a view widget for visualizing the items, with support for zooming and rotation.

![Various shapes and graphics on a grid](images/graphicsview-items.png)

## Using the Module

Using a Qt module\'s C++ API requires linking against the module library, either directly or through other dependencies. Several build tools have dedicated support for this, including [CMake](https://cmake.org/cmake/help/latest/) and [qmake](qmake-manual.html).

### Building with CMake

Use the `find_package()` command to locate the needed module component in the `Qt6` package:

``` {.cpp .prettyprint translate="no"}
find_package(Qt6 REQUIRED COMPONENTS Widgets)
target_link_libraries(mytarget PRIVATE Qt6::Widgets)
```

For more details, see the [Build with CMake](cmake-manual.html) overview.

### Building with qmake

To configure the module for building with qmake, add the module as a value of the `QT` variable in the project\'s .pro file:

``` {.cpp .prettyprint translate="no"}
QT += widgets
```

## Articles and Guides

- [Widgets Tutorial](widgets-tutorial.html)
- [Getting Started Programming with Qt Widgets](widgets-getting-started.html)
- [Qt Creator: Tutorial: Qt Widgets application](https://doc.qt.io/qtcreator/creator-writing-program.html)
- [Model/View Tutorial](modelview.html)

## Examples

- [Qt Widgets Examples](examples-widgets.html)
- [Layout Examples](layout.html#layout-examples)
- [Widget Tools Examples](examples-widgets-tools.html)

## API Reference

- [Qt Widgets C++ Classes](qtwidgets-module.html)
  - [Basic Widget Classes](widget-classes.html#basic-widget-classes)
  - [Advanced Widget Classes](widget-classes.html#advanced-widget-classes)
  - [Abstract Widget Classes](widget-classes.html#abstract-widget-classes)
  - [Organizer Widget Classes](widget-classes.html#organizer-widget-classes)
  - [Graphics View Classes](widget-classes.html#graphics-view-classes)
  - [Model/View Classes](model-view-programming.html#the-model-view-classes)
  - [Main Window and Related Classes](widget-classes.html#main-window-and-related-classes)
  - [Widget Appearance and Style Related Classes](widget-classes.html#widget-appearance-and-style-related-classes)
  - [Layout Classes](widget-classes.html#layout-classes)
- [Qt Style Sheets Reference](stylesheet-reference.html)

## Module Evolution

[Changes to Qt Widgets](widgets-changes-qt6.html) lists important changes in the module API and functionality that were done for the Qt 6 series of Qt.

## Licenses

The Qt Widgets module is available under commercial licenses from [The Qt Company](https://www.qt.io/about-us/). In addition, it is available under free software licenses: The [GNU Lesser General Public License, version 3](https://www.gnu.org/licenses/lgpl-3.0.html), or the [GNU General Public License, version 2](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html). See [Qt Licensing](licensing.html) for further details.

[©]{.abbr title="Copyright"} 2026 The Qt Company Ltd. Documentation contributions included herein are the copyrights of their respective owners. The documentation provided herein is licensed under the terms of the [GNU Free Documentation License version 1.3](http://www.gnu.org/licenses/fdl.html) as published by the Free Software Foundation. Qt and respective logos are [trademarks](https://doc.qt.io/qt/trademarks.html) of The Qt Company Ltd. in Finland and/or other countries worldwide. All other trademarks are property of their respective owners.

###### **Contents**

- [Widgets](#widgets)

- [Styles](#styles)

- [Layouts](#layouts)

- [Model/View Classes](#model-view-classes)

- [Graphics View](#graphics-view)

- [Using the Module](#using-the-module)

- [Building with CMake](#building-with-cmake)

- [Building with qmake](#building-with-qmake)

- [Articles and Guides](#articles-and-guides)

- [Examples](#examples)

- [API Reference](#api-reference)

- [Module Evolution](#module-evolution)

- [Licenses](#licenses)

[!](https://www.qt.io/?hsLang=en)

{.fm_button .fm_twitter target="_blank" rel="noopener"} {.fm_button .fm_facebook target="_blank" rel="noopener"} {.fm_button .fm_youtube target="_blank" rel="noopener"} {.fm_button .fm_linkedin target="_blank" rel="noopener"}

[Contact Us](https://www.qt.io/contact-us?hsLang=en)

- [Qt Group](javascript:;){aria-haspopup="true" aria-expanded="false" role="menuitem"}
  - [Our Story](https://www.qt.io/group)
  - [Brand](https://www.qt.io/brand)
  - [News](https://www.qt.io/newsroom)
  - [Careers](https://www.qt.io/careers)
  - [Investors](https://www.qt.io/investors)
  - [Qt Products](https://www.qt.io/product)
  - [Software Quality Products](https://www.qt.io/product/quality-assurance)
- [Licensing](javascript:;){aria-haspopup="true" aria-expanded="false" role="menuitem"}
  - [License Agreement](https://www.qt.io/terms-conditions)
  - [Open Source](https://www.qt.io/licensing/open-source-lgpl-obligations)
  - [Plans and pricing](https://www.qt.io/pricing)
  - [Download](https://www.qt.io/download)
  - [FAQ](https://www.qt.io/faq/overview)
- [Learn Qt](javascript:;){aria-haspopup="true" aria-expanded="false" role="menuitem"}
  - [For Learners](https://www.qt.io/academy)
  - [For Students and Teachers](https://www.qt.io/qt-educational-license)
  - [Qt Documentation](https://doc.qt.io/){role="menuitem" target="_blank" rel="noopener"}
  - [Qt Forum](https://forum.qt.io/){role="menuitem" target="_blank" rel="noopener"}
- [Support & Services](javascript:;){aria-haspopup="true" aria-expanded="false" role="menuitem"}
  - [Professional Services](https://www.qt.io/qt-professional-services)
  - [Customer Success](https://www.qt.io/customer-success)
  - [Support Services](https://www.qt.io/qt-support/)
  - [Partners](https://www.qt.io/contact-us/partners)
  - [Qt World](https://www.qt.io/qt-world){role="menuitem" target="_blank" rel="noopener"}

- [© 2026 The Qt Company](javascript:;)
- [Feedback](mailto:feedback@qt.io?Subject=Feedback%20about%20doc.qt.io%20site)

Qt Group includes The Qt Company Oy and its global subsidiaries and affiliates.
