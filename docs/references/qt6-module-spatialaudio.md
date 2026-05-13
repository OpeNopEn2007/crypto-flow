# Qt Spatial Audio

> **来源 URL**: https://doc.qt.io/qt-6/qtspatialaudio-module.html
> **获取日期**: 2026-05-12
> **模块说明**: 空间音频

---

# Qt Spatial Audio Module C++ Classes

The [Qt Spatial Audio](qtspatialaudio-index.html) module provides functionality for 3D audio.

## Classes

  [QAmbientSound](qambientsound.html)     A stereo overlay sound
  [QAudioEngine](qaudioengine.html)       Manages a three dimensional sound field
  [QAudioListener](qaudiolistener.html)   Defines the position and orientation of the person listening to a sound field defined by QAudioEngine
  [QAudioRoom](qaudioroom.html)
  [QSpatialSound](qspatialsound.html)     A sound object in 3D space

## Detailed Description

### Using the Module

Using a Qt module requires linking against the module library, either directly or through other dependencies. Several build tools have dedicated support for this, including [CMake](https://cmake.org/cmake/help/latest/) and [qmake](qmake-manual.html).

#### Building with CMake

Use the `find_package()` command to locate the needed module components in the `Qt6` package:

```
find_package(Qt6 REQUIRED COMPONENTS SpatialAudio)
target_link_libraries(my_project PRIVATE Qt6::SpatialAudio)
```
