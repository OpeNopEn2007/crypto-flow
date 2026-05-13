# Qt 6 - Positioning Module

> **来源**: https://doc.qt.io/qt-6/qtpositioning-module.html
> **获取日期**: 2026-05-12

# Qt Positioning C++ Classes

The Positioning module provides positioning information via QML and C++ interfaces. [More\...](#details)

## Classes(#classes "Direct link to this headline")

  [QGeoAddress](qgeoaddress.html)                                       Represents an address of a QGeoLocation
  [QGeoAreaMonitorInfo](qgeoareamonitorinfo.html)                       Describes the parameters of an area or region to be monitored for proximity
  [QGeoAreaMonitorSource](qgeoareamonitorsource.html)                   Enables the detection of proximity changes for a specified set of coordinates
  [QGeoCircle](qgeocircle.html)                                         Defines a circular geographic area
  [QGeoCoordinate](qgeocoordinate.html)                                 Defines a geographical position on the surface of the Earth
  [QGeoLocation](qgeolocation.html)                                     Represents basic information about a location
  [QGeoPath](qgeopath.html)                                             Defines a geographic path
  [QGeoPolygon](qgeopolygon.html)                                       Defines a geographic polygon
  [QGeoPositionInfo](qgeopositioninfo.html)                             Contains information gathered on a global position, direction and velocity at a particular point in time
  [QGeoPositionInfoSource](qgeopositioninfosource.html)                 Abstract base class for the distribution of positional updates
  [QGeoPositionInfoSourceFactory](qgeopositioninfosourcefactory.html)   Factory class used as the plugin interface for external providers of positioning data
  [QGeoRectangle](qgeorectangle.html)                                   Defines a rectangular geographic area
  [QGeoSatelliteInfo](qgeosatelliteinfo.html)                           Contains basic information about a satellite
  [QGeoSatelliteInfoSource](qgeosatelliteinfosource.html)               Abstract base class for the distribution of satellite information updates
  [QGeoShape](qgeoshape.html)                                           Defines a geographic area
  [QNmeaPositionInfoSource](qnmeapositioninfosource.html)               Positional information using a NMEA data source
  [QNmeaSatelliteInfoSource](qnmeasatelliteinfosource.html)             Satellite information using an NMEA data source

## Detailed Description(#details "Direct link to this headline")

To load the Qt Positioning module, add the following statement to your .qml files

``` {.qml .prettyprint translate="no"}
import QtPositioning
```

For C++ projects include the header appropriate for the current use case, for example applications using routes may use

``` {.cpp .prettyprint translate="no"}
#include
```

### Using the Module(#using-the-module "Direct link to this headline")

Using a Qt module requires linking against the module library, either directly or through other dependencies. Several build tools have dedicated support for this, including [CMake](https://cmake.org/cmake/help/latest/) and [qmake](qmake-manual.html).

#### Building with CMake(#building-with-cmake "Direct link to this headline")

Use the `find_package()` command to locate the needed module components in the `Qt6` package:

``` {.cpp .plain translate="no"}
find_package(Qt6 REQUIRED COMPONENTS Positioning)
target_link_libraries(mytarget PRIVATE Qt6::Positioning)
```

#### Building with qmake(#building-with-qmake "Direct link to this headline")

To configure the module for building with qmake, add the module as a value of the `QT` variable in the project's .pro file:

``` {.cpp .plain translate="no"}
QT += positioning
```

See more in the [Qt Positioning Overview](qtpositioning-index.html).
