---
source: https://doc.qt.io/qt-6/qtqml-index.html
fetched: 2026-05-12
---

# Qt Qml

The Qt Qml module provides a framework for developing applications and libraries with the [QML language](qmlreference.html). It defines and implements the language and engine infrastructure, and provides an API to enable application developers to [register](qtqml-index.html#registering-qml-types-and-qml-modules) custom QML types and modules and integrate QML code with JavaScript and C++. The Qt Qml module provides both a [QML API](qtqml-qmlmodule.html) and a [C++ API](qtqml-module.html).

## Using the Module

### QML API

The QML types of the module are available through the `QtQml` import. To use the types, add the following import statement to your .qml file:

``` {.qml .prettyprint translate="no"}
import QtQml
```

The Qt Qml module contains the QML framework and important QML types used in applications. The constructs of QML are described in the [The QML Reference](qmlreference.html).

The [QML API](qtqml-qmlmodule.html) of the Qt Qml module provides a number of [QML Object Types](qtqml-typesystem-objecttypes.html), [QML Value Types](qtqml-typesystem-valuetypes.html) and namespaces.

### C++ API

Using a Qt module\'s C++ API requires linking against the module library, either directly or through other dependencies. Several build tools have dedicated support for this, including [CMake](https://cmake.org/cmake/help/latest/) and [qmake](qmake-manual.html).

The C++ API contains some [important classes](qtqml-cppclasses-topic.html) you should get familiar with. It also provides [types](qtqml-integrating-with-js-values-from-cpp.html) to hold JavaScript values.

#### Building with CMake

Use the `find_package()` command to locate the needed module component in the `Qt6` package:

``` {.cpp .prettyprint translate="no"}
find_package(Qt6 REQUIRED COMPONENTS Qml)
target_link_libraries(mytarget PRIVATE Qt6::Qml)
```

For more details, see the [Build with CMake](cmake-manual.html) overview.

To provide foreign QML type support for a non-QML library, locate the `QmlIntegration` module as follows:

``` {.cpp .prettyprint translate="no"}
find_package(Qt6 REQUIRED COMPONENTS QmlIntegration)
target_link_libraries(mytarget PRIVATE Qt6::QmlIntegration)
```

See [qt6_generate_foreign_qml_types](qt-generate-foreign-qml-types.html) for details.

#### Building with qmake

To configure the module for building with qmake, add the module as a value of the `QT` variable in the project\'s .pro file:

``` {.cpp .prettyprint translate="no"}
QT += qml
```

## Registering QML Types and QML Modules

In order to register types for usage with QML you first need to define a [QML module](qtqml-writing-a-module.html), preferably using [qt_add_qml_module](qt-add-qml-module.html) in CMake. Then, you can add C++ headers to your new module, and [define types](qtqml-cppintegration-definetypes.html) to be [exposed to QML](qtqml-cppintegration-exposecppattributes.html) in them.

## Tweaking the engine

There are a number of knobs you can turn to customize the behavior of the QML engine. The page on [configuring the JavaScript engine](qtqml-javascript-finetuning.html) lists the environment variables you may use to this effect. The description of the [QML Disk Cache](qmldiskcache.html) describes the options related to how your QML components are compiled and loaded.

## Articles and Guides

These articles contain information about Qt Qml.

- [The QML Reference](qmlreference.html)
- [Qt Qml Tooling](qtqml-tooling.html)
- [Writing QML Modules](qtqml-writing-a-module.html)
- [Singletons in QML](qml-singleton.html)

## Reference

- [C++ Classes](qtqml-module.html)
- [QML Types](qtqml-qmlmodule.html)

## Licenses and Attributions

Qt Qml is available under commercial licenses from [The Qt Company](https://www.qt.io/about-us/). In addition, it is available under free software licenses. Since Qt 5.4, these free software licenses are [GNU Lesser General Public License, version 3](https://www.gnu.org/licenses/lgpl-3.0.html), or the [GNU General Public License, version 2](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html). See [Qt Licensing](licensing.html) for further details.

Furthermore Qt Qml in Qt 6.11.0 may contain third party modules under following permissive licenses:

  --------------------------------------------------------------- -------------------------------------
  [JavaScriptCore Macro Assembler](qtqml-attribution-masm.html)   BSD 2-clause \"Simplified\" License
  --------------------------------------------------------------- -------------------------------------

[©]{.abbr title="Copyright"} 2026 The Qt Company Ltd. Documentation contributions included herein are the copyrights of their respective owners. The documentation provided herein is licensed under the terms of the [GNU Free Documentation License version 1.3](http://www.gnu.org/licenses/fdl.html) as published by the Free Software Foundation. Qt and respective logos are [trademarks](https://doc.qt.io/qt/trademarks.html) of The Qt Company Ltd. in Finland and/or other countries worldwide. All other trademarks are property of their respective owners.

###### **Contents**

- [Using the Module](#using-the-module)

- [QML API](#qml-api)

- [C++ API](#c-api)

- [Registering QML Types and QML Modules](#registering-qml-types-and-qml-modules)

- [Tweaking the engine](#tweaking-the-engine)

- [Articles and Guides](#articles-and-guides)

- [Reference](#reference)

- [Licenses and Attributions](#licenses-and-attributions)

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
