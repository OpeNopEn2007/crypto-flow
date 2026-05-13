---
source: https://doc.qt.io/qt-6/qtcore-index.html
fetched: 2026-05-12
---

# Qt Core

The Qt Core module adds these features to C++:

- a very powerful mechanism for seamless object communication called signals and slots
- queryable and designable object properties
- hierarchical and queryable object trees that organize object ownership in a natural way with guarded pointers ([QPointer](qpointer.html))
- a dynamic cast that works across library boundaries

The following pages provide more information about Qt\'s core features:

- [The Meta-Object System](metaobjects.html)
- [The Property System](properties.html)
- [Object Model](object.html)
- [Object Trees & Ownership](objecttrees.html)
- [Signals & Slots](signalsandslots.html)

## Using the Module

Using a Qt module\'s C++ API requires linking against the module library, either directly or through other dependencies. Several build tools have dedicated support for this, including [CMake](https://cmake.org/cmake/help/latest/) and [qmake](qmake-manual.html).

### Building with CMake

Use the `find_package()` command to locate the needed module component in the `Qt6` package:

``` {.cpp .prettyprint translate="no"}
find_package(Qt6 REQUIRED COMPONENTS Core)
target_link_libraries(mytarget PRIVATE Qt6::Core)
```

For more details, see the [Build with CMake](cmake-manual.html) overview.

### Building with qmake

If you use [qmake](qmake-manual.html) to build your projects, Qt Core is linked by default.

## Threading and Concurrent Programming

Qt provides thread support in the form of platform-independent [threading classes](thread.html), a thread-safe way of posting events, and signal-slot connections across threads. Multithreaded programming is also a useful paradigm for performing time-consuming operations without freezing the user interface of an application.

The [Multi-threading in Qt](threads.html) page contains information on implementing threads in applications. Additional concurrent classes are provided by the [Qt Concurrent](qtconcurrent-index.html) module.

## Input/Output, Resources, and Containers

Qt provides a resource system for organizing application files and assets, a set of containers, and classes for receiving input and printing output.

- [Container Classes](containers.html)
- [Serializing Qt Data Types](datastreamformat.html)
- [Implicit Sharing](implicit-sharing.html)

In addition, Qt Core provides a platform-independent mechanism for storing binary files in the application\'s executable.

- [The Qt Resource System](resources.html)

## Additional Frameworks

Qt Core also provides some of Qt\'s key frameworks.

- [The Animation Framework](animation-overview.html)
- [JSON Support in Qt](json.html)
- [CBOR Support in Qt](cbor.html)
- [Inter-Process Communication](ipc.html)
- [How to Create Qt Plugins](plugins-howto.html)
- [The Event System](eventsandfilters.html)
- [Application Permissions](permissions.html)

## Reference

- [C++ Classes](qtcore-module.html)
  - [Animation Classes](animation.html)
  - [Threading Classes](thread.html)
  - [Container Classes](containers.html)
  - [Plugin Classes](plugins.html)
  - [Implicitly Shared Classes](shared.html)
  - [Input/Output Classes](io.html)
  - [Event Classes](events.html)
- CMake API
  - [CMake Commands](cmake-commands-qtcore.html)
  - [CMake Variables](cmake-variables-qtcore.html)
  - [CMake Target Properties](cmake-target-properties-qtcore.html)

## Module Evolution

[Changes to Qt Core](qtcore-changes-qt6.html) lists important changes in the module API and functionality that were made for the Qt 6 series of Qt.

## Licenses and Attributions

Qt Core is available under commercial licenses from [The Qt Company](https://www.qt.io/about-us/). In addition, it is available under free software licenses: The [GNU Lesser General Public License, version 3](https://www.gnu.org/licenses/lgpl-3.0.html), or the [GNU General Public License, version 2](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html). See [Qt Licensing](licensing.html) for further details.

Executables on Windows potentially link against [The QtEntryPoint Library](qtentrypoint.html). This library is available under commercial licenses and also under the [BSD 3-clause \"New\" or \"Revised\" License](https://spdx.org/licenses/BSD-3-Clause.html).

Furthermore, Qt Core in Qt 6.11.0 may contain third party modules under the following permissive licenses:

  --------------------------------------------------------------------------------------------------------------------------------------------- ---------------------------------------------------------------------------------------
  [Apache Tika MimeType Definitions, version 408c26e1e03e018a623e732dff6fb047a2fb8e19](qtcore-attribution-tika-mimetypes.html)                  Apache License 2.0
  [BLAKE2 (reference implementation), version ed1974ea83433eba7b2d95c5dcd9ac33cb847913](qtcore-attribution-blake2.html)                         Creative Commons Zero v1.0 Universal or Apache License 2.0
  [Data Compression Library (zlib), version 1.3.2](qtcore-attribution-zlib.html)                                                                zlib License
  [Easing Equations by Robert Penner](qtcore-attribution-easing.html)                                                                           BSD 3-clause \"New\" or \"Revised\" License
  [Efficient Binary-Decimal and Decimal-Binary Conversion Routines for IEEE Doubles, version 3.4.0](qtcore-attribution-doubleconversion.html)   BSD 3-clause \"New\" or \"Revised\" License
  [MD4](qtcore-attribution-md4.html)                                                                                                            Public Domain
  [MD5](qtcore-attribution-md5.html)                                                                                                            Public Domain
  [PCRE2 - Stack-less Just-In-Time Compiler, version 10.47](qtcore-attribution-pcre2-sljit.html)                                                BSD 2-clause \"Simplified\" License
  [PCRE2, version 10.47](qtcore-attribution-pcre2.html)                                                                                         BSD 3-clause \"New\" or \"Revised\" License with PCRE2 binary-like Packages Exception
  [QEventDispatcher on macOS](qtcore-attribution-qeventdispatcher-cf.html)                                                                      BSD 3-clause \"New\" or \"Revised\" License
  [Secure Hash Algorithm SHA-1](qtcore-attribution-sha1.html)                                                                                   Public Domain
  [Secure Hash Algorithm SHA-3 - Keccak, version 3.2](qtcore-attribution-sha3-keccak.html)                                                      Creative Commons Zero v1.0 Universal
  [Secure Hash Algorithm SHA-3 - brg_endian, version 1.0.0](qtcore-attribution-sha3-endian.html)                                                BSD 2-clause \"Simplified\" License
  [Secure Hash Algorithms SHA-384 and SHA-512](qtcore-attribution-rfc6234.html)                                                                 BSD 3-clause \"New\" or \"Revised\" License
  [SipHash Algorithm](qtcore-attribution-siphash.html)                                                                                          Creative Commons Zero v1.0 Universal
  [TinyCBOR, version 7.0](qtcore-attribution-tinycbor.html)                                                                                     MIT License
  [Unicode Character Database (UCD), version 36](qtcore-attribution-unicode-character-database.html)                                            Unicode License Agreement - Data Files and Software (2016)
  [Unicode Common Locale Data Repository (CLDR), version v48.1](qtcore-attribution-unicode-cldr.html)                                           Unicode License v3
  [forkfd](qtcore-attribution-forkfd.html)                                                                                                      MIT License
  [tl::expected, version 41d3e1f48d682992a2230b2a715bca38b848b269](qtcore-attribution-tlexpected.html)                                          Creative Commons Zero v1.0 Universal
  --------------------------------------------------------------------------------------------------------------------------------------------- ---------------------------------------------------------------------------------------

[©]{.abbr title="Copyright"} 2026 The Qt Company Ltd. Documentation contributions included herein are the copyrights of their respective owners. The documentation provided herein is licensed under the terms of the [GNU Free Documentation License version 1.3](http://www.gnu.org/licenses/fdl.html) as published by the Free Software Foundation. Qt and respective logos are [trademarks](https://doc.qt.io/qt/trademarks.html) of The Qt Company Ltd. in Finland and/or other countries worldwide. All other trademarks are property of their respective owners.

###### **Contents**

- [Using the Module](#using-the-module)

- [Building with CMake](#building-with-cmake)

- [Building with qmake](#building-with-qmake)

- [Threading and Concurrent Programming](#threading-and-concurrent-programming)

- [Input/Output, Resources, and Containers](#input-output-resources-and-containers)

- [Additional Frameworks](#additional-frameworks)

- [Reference](#reference)

- [Module Evolution](#module-evolution)

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
