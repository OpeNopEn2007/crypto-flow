# Qt UI Tools Module

> **Source**: <https://doc.qt.io/qt-6/qtuitools-module.html>
> **Fetched**: 2026-05-12

---

# Qt UI Tools C++ Classes

Provides classes to handle forms created with Qt Widgets Designer. [More\...](#details)

## Classes

  [QUiLoader](quiloader.html)   Enables standalone applications to dynamically create user interfaces at run-time using the information stored in UI files or specified in plugin paths

## Detailed Description

These forms are processed at run-time to produce dynamically-generated user interfaces. In order to generate a form at run-time, a resource file containing a UI file is needed. Applications that use the form handling classes need to be configured to be built against the QtUiTools module. This is done by including the following declaration in a `qmake` project file to ensure that the application is compiled and linked appropriately.

``` cpp
QT += uitools
```

A form loader object, provided by the [QUiLoader](quiloader.html) class, is used to construct the user interface. This user interface can be retrieved from any [QIODevice](qiodevice.html); for example, a [QFile](qfile.html) object can be used to obtain a form stored in a project's resources. The [QUiLoader::load](quiloader.html#load)() function takes the user interface description contained in the file and constructs the form widget.
