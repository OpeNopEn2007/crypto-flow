# Qt Serial Port Module

> **Source**: <https://doc.qt.io/qt-6/qtserialport-module.html>
> **Fetched**: 2026-05-12

---

# Qt Serial Port C++ Classes

List of C++ classes that enable access to a serial port. [More\...](#details)

## Classes

  [QSerialPort](qserialport.html)           Provides functions to access serial ports
  [QSerialPortInfo](qserialportinfo.html)   Provides information about existing serial ports

## Detailed Description

To use [QSerialPort](qserialport.html) in your application, add the following include statement:

``` cpp
#include <QSerialPort>
```

For [QSerialPortInfo](qserialportinfo.html), use the following include statement:

``` cpp
#include <QSerialPortInfo>
```

To use the module with cmake, use the `find_package()` command to locate the needed module components in the `Qt6` package:

``` cpp
find_package(Qt6 REQUIRED COMPONENTS SerialPort)
target_link_libraries(mytarget PRIVATE Qt6::SerialPort)
```

To use the module for building with qmake, add the module as a value of the `QT` variable in the project's .pro file:

``` cpp
QT += serialport
```
