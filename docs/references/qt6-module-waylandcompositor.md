# Qt Wayland Compositor

> **来源 URL**: https://doc.qt.io/qt-6/qtwaylandcompositor-module.html
> **获取日期**: 2026-05-12
> **模块说明**: Wayland 合成器

---

# Qt Wayland Compositor C++ Classes

Provides C++ classes for writing custom Wayland display servers.

## Classes

  [QWaylandBufferRef](qwaylandbufferref.html)                                       Holds the reference to a surface buffer
  [QWaylandClient](qwaylandclient.html)                                             Represents a client connecting to the QWaylandCompositor
  [QWaylandCompositor](qwaylandcompositor.html)                                     Manages the Wayland display server
  [QWaylandCompositorExtension](qwaylandcompositorextension.html)                   The base class for compositor extensions
  [QWaylandCompositorExtensionTemplate](qwaylandcompositorextensiontemplate.html)   Convenience class for subclassing QWaylandCompositorExtension
  [QWaylandIdleInhibitManagerV1](qwaylandidleinhibitmanagerv1.html)                 Provides an extension that allows to inhibit the idle behavior of the compositor
  [QWaylandIviApplication](qwaylandiviapplication.html)                             Extension for embedded-style user interfaces
  [QWaylandIviSurface](qwaylandivisurface.html)                                     Simple way to identify and resize a surface
  [QWaylandKeyboard](qwaylandkeyboard.html)                                         Represents a keyboard device
  [QWaylandObject](qwaylandobject.html)                                             The base class for objects that can contain Wayland extensions
  [QWaylandOutput](qwaylandoutput.html)                                             Represents a displayable area managed by the compositor
  [QWaylandOutputMode](qwaylandoutputmode.html)                                     Holds the resolution and refresh rate of an output
  [QWaylandPointer](qwaylandpointer.html)                                           Represents a pointer device
  [QWaylandQtTextInputMethodManager](qwaylandqttextinputmethodmanager.html)         Provides access to input methods in the compositor
  [QWaylandQuickItem](qwaylandquickitem.html)                                       Qt Quick item representing a QWaylandView
  [QWaylandQuickShellIntegration](qwaylandquickshellintegration.html)               Provides support for shell surface integration with QtQuick
  [QWaylandQuickShellSurfaceItem](qwaylandquickshellsurfaceitem.html)               Qt Quick item that represents a QWaylandShellSurface
  [QWaylandResource](qwaylandresource.html)                                         Container for a wl_resource
  [QWaylandSeat](qwaylandseat.html)                                                 Access to keyboard, mouse, and touch input
  [QWaylandShell](qwaylandshell.html)                                               Abstract class for implementing a wayland shell
  [QWaylandShellSurface](qwaylandshellsurface.html)                                 Common interface for surface roles specified by shell extensions
  [QWaylandShellSurfaceTemplate](qwaylandshellsurfacetemplate.html)                 Convenience class for creating custom shell surface classes
  [QWaylandSurface](qwaylandsurface.html)                                           Represents a rectangular area on an output device
  [QWaylandSurfaceGrabber](qwaylandsurfacegrabber.html)                             Allows to read the content of a QWaylandSurface
  [QWaylandSurfaceRole](qwaylandsurfacerole.html)                                   Represents the role of the surface in context of wl_surface
  [QWaylandTextInputManager](qwaylandtextinputmanager.html)                         Provides access to input methods in the compositor
  [QWaylandTouch](qwaylandtouch.html)                                               Access to a touch device
  [QWaylandView](qwaylandview.html)                                                 Represents a view of a surface on an output
  [QWaylandViewporter](qwaylandviewporter.html)                                     Provides an extension for surface resizing and cropping
  [QWaylandWlShell](qwaylandwlshell.html)                                           Extension for desktop-style user interfaces
  [QWaylandWlShellSurface](qwaylandwlshellsurface.html)                             Desktop-style compositor-specific features to a surface
  [QWaylandXdgDecorationManagerV1](qwaylandxdgdecorationmanagerv1.html)             Provides an extension for negotiation of server-side and client-side window decorations
  [QWaylandXdgOutputManagerV1](qwaylandxdgoutputmanagerv1.html)                     Provides an extension for describing outputs in a desktop oriented fashion
  [QWaylandXdgOutputV1](qwaylandxdgoutputv1.html)                                   Represents a display in a compositor
  [QWaylandXdgPopup](qwaylandxdgpopup.html)                                         Represents the popup specific parts of an xdg surface
  [QWaylandXdgShell](qwaylandxdgshell.html)                                         Extension for desktop-style user interfaces
  [QWaylandXdgSurface](qwaylandxdgsurface.html)                                     Desktop-style compositor-specific features to an xdg surface
  [QWaylandXdgToplevel](qwaylandxdgtoplevel.html)                                   Represents the toplevel window specific parts of an xdg surface

## Detailed Description

### Using the Module

Using a Qt module's C++ API requires linking against the module library, either directly or through other dependencies. Several build tools have dedicated support for this, including [CMake](https://cmake.org/cmake/help/latest/) and [qmake](qmake-manual.html).

#### Building with CMake

Use the `find_package()` command to locate the needed module component in the `Qt6` package:

```
find_package(Qt6 REQUIRED COMPONENTS WaylandCompositor)
target_link_libraries(mytarget PRIVATE Qt6::WaylandCompositor)
```

For more details, see the [Build with CMake](cmake-manual.html) overview.

#### Building with qmake

To configure the module for building with qmake, add the module as a value of the `QT` variable in the project's .pro file:

```
QT += waylandcompositor
```

For more information about using these classes in your application, see the [Qt Wayland Compositor](qtwaylandcompositor-index.html) documentation.
