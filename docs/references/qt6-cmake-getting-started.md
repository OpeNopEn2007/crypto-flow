---
source: https://doc.qt.io/qt-6/cmake-get-started.html
fetched: 2026-05-12
---

# Getting started with CMake

`CMake` is a group of tools that allow to build, test, and package applications. Just like Qt, it is available on all major development platforms. It is also supported by various IDE\'s, including [Qt Creator](https://doc.qt.io/qtcreator/index.html).

In this section we will show the most basic way to use Qt in a CMake project. First, we create a basic console application. Then, we extend the project into a GUI application that uses [Qt Widgets](qtwidgets-index.html).

If you want to know how to build an existing CMake project with Qt, see the documentation on [how to build projects with CMake on the command line](cmake-build-on-cmdline.html).

To learn the basics of getting started with CMake, take the [Building with Cmake: Getting Started with CMake and Qt](https://www.qt.io/academy/course-catalog#building-with-cmake:-getting-started-with-cmake-and-qt) course in Qt Academy.

### Building a C++ console application

A `CMake` project is defined by files written in the CMake language. The main file is called `CMakeLists.txt`, and is usually placed in the same directory as the actual program sources.

Here is a typical `CMakeLists.txt` file for a console application written in C++ using Qt:

::: pre
``` {.cpp .prettyprint translate="no"}
cmake_minimum_required(VERSION 3.16)

project(helloworld VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

qt_add_executable(helloworld
    main.cpp
)

target_link_libraries(helloworld PRIVATE Qt6::Core)
```

Let\'s go through the content.

::: pre
``` {.cpp .prettyprint translate="no"}
cmake_minimum_required(VERSION 3.16)
```

`cmake_minimum_required()` specifies the minimum CMake version that is required to successfully configure the project. See [Supported CMake versions](cmake-supported-cmake-versions.html) for the minimum version required by Qt.

::: pre
``` {.cpp .prettyprint translate="no"}
project(helloworld VERSION 1.0.0 LANGUAGES CXX)
```

`project()` sets a project name and the default project version. The `LANGUAGES` argument tells CMake that the program is written in C++.

::: pre
``` {.cpp .prettyprint translate="no"}
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```

Qt 6 requires a compiler supporting C++ version 17 or newer. Enforcing this by setting the `CMAKE_CXX_STANDARD`, `CMAKE_CXX_STANDARD_REQUIRED` variables will let CMake print an error if the compiler is too old.

::: pre
``` {.cpp .prettyprint translate="no"}
find_package(Qt6 REQUIRED COMPONENTS Core)
```

This tells CMake to look up Qt 6 and make the `Core` module available. There is no point in continuing if `CMake` cannot locate the module, so we do set the `REQUIRED` flag to let CMake abort in this case. For more information, see [Making Qt available in CMake projects](cmake-making-qt-available.html).

If successful, the module will set some CMake variables documented in [Module variables](cmake-variable-reference.html#module-variables). It furthermore imports the `Qt6::Core` target that we use below.

::: pre
``` {.cpp .prettyprint translate="no"}
qt_standard_project_setup()
```

The [qt_standard_project_setup](qt-standard-project-setup.html) command sets project-wide defaults for a typical Qt application.

Among other things, this command sets the `CMAKE_AUTOMOC` variable to `ON`, which instructs CMake to automatically set up rules so that Qt\'s [Meta-Object Compiler (moc)](moc.html) is called transparently, when required.

See [qt_standard_project_setup](qt-standard-project-setup.html)\'s reference for details.

::: pre
``` {.cpp translate="no"}
qt_add_executable(helloworld
    main.cpp
)
```

[qt_add_executable()](qt-add-executable.html) tells CMake that we want to build an executable (so not a library) called `helloworld` as a target. It is a wrapper around the built-in `add_executable()` command, and provides additional logic to automatically handle things like linking of Qt plugins in static Qt builds, platform-specific customization of library names, and so on.

The target should be built from the C++ source file `main.cpp`.

Typically, you do not list header files here. This is different from [qmake](qmake-manual.html), where header files need to be explicitly listed so that they are processed by the [Meta-Object Compiler (moc)](moc.html).

For creating libraries, see [qt_add_library](qt-add-library.html).

::: pre
``` {.cpp .prettyprint translate="no"}
target_link_libraries(helloworld PRIVATE Qt6::Core)
```

Finally, `target_link_libraries` tells CMake that the `helloworld` executable makes use of [Qt Core](qtcore-index.html) by referencing the `Qt6::Core` target imported by the `find_package()` call above. This will not only add the right arguments to the linker, but also makes sure that the right include directories, compiler definitions are passed to the C++ compiler. The `PRIVATE` keyword is not strictly necessary for an executable target, but it is good practice to specify it. If `helloworld` was a library rather than an executable, then either `PRIVATE` or `PUBLIC` should be specified (`PUBLIC` if the library mentions anything from `Qt6::Core` in its headers, `PRIVATE` otherwise).

### Building a C++ GUI application

[]

In the [last section](cmake-get-started.html#building-a-c-console-application) we showed the CMakeLists.txt file for a simple console application. We will now create a GUI application that uses the [Qt Widgets](qtwidgets-index.html) module.

This is the full project file:

::: pre
``` {.cpp .prettyprint translate="no"}
cmake_minimum_required(VERSION 3.16)

project(helloworld VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(helloworld
    mainwindow.ui
    mainwindow.cpp
    main.cpp
)

target_link_libraries(helloworld PRIVATE Qt6::Widgets)

set_target_properties(helloworld PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
```

Let\'s walk through the changes we have made.

::: pre
``` {.cpp .prettyprint translate="no"}
find_package(Qt6 REQUIRED COMPONENTS Widgets)
```

In the `find_package` call, we replace `Core` with `Widgets`. This will locate the `Qt6Widgets` module and provide the `Qt6::Widgets` targets we later link against.

::: pre
``` {.cpp .prettyprint translate="no"}
qt_standard_project_setup()
```

In addition to `CMAKE_AUTOMOC`, [qt_standard_project_setup](qt-standard-project-setup.html) sets the `CMAKE_AUTOUIC` variable to `ON`. This will automatically create rules to invoke Qt\'s [User Interface Compiler (uic)](uic.html) on `.ui` source files.

::: pre
``` {.cpp translate="no"}
qt_add_executable(helloworld
    mainwindow.ui
    mainwindow.cpp
    main.cpp
)
```

We add a [Qt Widgets Designer](qtdesigner-manual.html) file (`mainwindow.ui`) and its corresponding C++ source file (`mainwindow.cpp`) to the application target\'s sources.

**Note:** Another way to add `.ui` files to your project is to use the command [qt_add_ui](qt-add-ui.html) instead of `AUTOUIC`.

::: pre
``` {.cpp .prettyprint translate="no"}
target_link_libraries(helloworld PRIVATE Qt6::Widgets)
```

In the `target_link_libraries` command, we link against `Qt6::Widgets` instead of `Qt6::Core`. Note that the application will still link against `Qt6::Core`, because `Qt6::Widgets` depends on it.

::: pre
``` {.cpp translate="no"}
set_target_properties(helloworld PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
```

Finally, we set properties on our application target with the following effects:

- Prevent the creation of a console window on Windows.
- Create an application bundle on macOS.

See the [CMake Documentation](https://cmake.org/cmake/help/latest/) for more information about these target properties.

### Structuring projects

Projects that contain more than just one target will benefit from a clear project file structure. We will use CMake\'s [subdirectory feature](https://cmake.org/cmake/help/latest/command/add_subdirectory.html).

As we plan to extend the project with more targets, we move the source files of the application into a subdirectory and create a new `CMakeLists.txt` in there.

::: pre
``` {.cpp .plain translate="no"}
<project root>
├── CMakeLists.txt
└── src
    └── app
        ├── CMakeLists.txt
        ├── main.cpp
        ├── mainwindow.cpp
        ├── mainwindow.h
        └── mainwindow.ui
```

The top-level `CMakeLists.txt` contains the overall project setup, `find_package` and `add_subdirectory` calls:

::: pre
``` {.cpp .prettyprint translate="no"}
cmake_minimum_required(VERSION 3.16)

project(helloworld VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_subdirectory(src/app)
```

Variables that are set in this file are visible in subdirectory project files.

The application\'s project file `src/app/CMakeLists.txt` contains the executable target:

::: pre
``` {.cpp translate="no"}
qt_add_executable(helloworld
    mainwindow.ui
    mainwindow.cpp
    main.cpp
)

target_link_libraries(helloworld PRIVATE Qt6::Widgets)

set_target_properties(helloworld PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
```

Such a structure will make it easy to add more targets to the project such as libraries or unit tests.

**Note:** Add your project build directory to the list of excluded directories of any anti-virus application that runs on your system.

### Building libraries

As the project grows, you may want to turn parts of your application code into a library that is used by the application and possibly unit tests. This section shows how to create such a library.

Our application currently contains business logic directly in `main.cpp`. We extract the code into a new static library called `businesslogic` in the subdirectory `"src/businesslogic"` as explained in the [previous section](cmake-get-started.html#structuring-projects).

For the sake of simplicity, the library consists of just one C++ source file and its corresponding header file that is included by the application\'s `main.cpp`:

::: pre
``` {.cpp .plain translate="no"}
<project root>
├── CMakeLists.txt
└── src
    ├── app
    │   ├── ...
    │   └── main.cpp
    └── businesslogic
        ├── CMakeLists.txt
        ├── businesslogic.cpp
        └── businesslogic.h
```

Let\'s have a look at the library\'s project file (`src/businesslogic/CMakeLists.txt`).

::: pre
``` {.cpp .prettyprint translate="no"}
qt_add_library(businesslogic STATIC
    businesslogic.cpp
)
target_link_libraries(businesslogic PRIVATE Qt6::Core)
target_include_directories(businesslogic INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
```

Let\'s go through the content.

::: pre
``` {.cpp translate="no"}
qt_add_library(businesslogic STATIC
    businesslogic.cpp
)
```

The [add_library](https://cmake.org/cmake/help/latest/command/add_library.html) command creates the library `businesslogic`. Later, we will let the application link against this target.

The `STATIC` keyword denotes a static library. If we wanted to create a shared or dynamic library, we would use the `SHARED` keyword.

::: pre
``` {.cpp .prettyprint translate="no"}
target_link_libraries(businesslogic PRIVATE Qt6::Core)
```

We have a static library and don\'t actually have to link other libraries. But as our library uses classes from `QtCore`, we add a link dependency to `Qt6::Core`. This pulls in the necessary `QtCore` include paths and preprocessor defines.

::: pre
``` {.cpp .prettyprint translate="no"}
target_include_directories(businesslogic INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
```

The library API is defined in the header file `businesslogic/businesslogic.h`. By calling [target_include_directories](https://cmake.org/cmake/help/latest/command/target_include_directories.html), we make sure that the absolute path to the `businesslogic` directory is automatically added as an include path to all targets using our library.

This frees us in `main.cpp` from using relative paths to locate `businesslogic.h`. Instead, we can just write

::: pre
``` {.cpp .plain translate="no"}
#include <businesslogic.h>
```

Last, we must add the library\'s subdirectory to the top-level project file:

::: pre
``` {.cpp .prettyprint translate="no"}
add_subdirectory(src/app)
add_subdirectory(src/businesslogic)
```

### Using libraries

To use the library we created in the [previous section](cmake-get-started.html#building-libraries), we instruct CMake to link against it:

::: pre
``` {.cpp .prettyprint translate="no"}
target_link_libraries(helloworld PRIVATE
    businesslogic
    Qt6::Widgets
)
```

This ensures that `businesslogic.h` is found when main.cpp is compiled. Furthermore, the businesslogic static library will become a part of the `helloworld` executable.

In CMake terms, the library `businesslogic` specifies *usage requirements* (the include path) that every consumer of our library (the application) has to satisfy. The `target_link_libraries` command takes care of that.

### Adding resources

We want to display some images in our application, so we add them using the [Qt Resource System](resources.html).

::: pre
``` {.cpp .prettyprint translate="no"}
qt_add_resources(helloworld imageresources
    PREFIX "/images"
    FILES logo.png splashscreen.png
)
```

The [qt_add_resources](qt-add-resources.html) command automatically creates a Qt resource containing the referenced images. From the C++ source code, you can access the images by prepending the specified resource prefix:

::: pre
``` {.cpp .prettyprint translate="no"}
logoLabel->setPixmap(QPixmap(":/images/logo.png"));
```

The [qt_add_resources](qt-add-resources.html) command takes as the first argument either a variable name or a target name. We recommend to use the target-based variant of this command as shown in the example above.

### Adding translations

[]

Translations of strings in a Qt project are encoded in `.ts` files. These `.ts` files are compiled into binary `.qm` files, which are then loaded by the Qt application at run time. See [Internationalization with Qt](internationalization.html) for details.

This section describes how to add a German and French translation to the `helloworld` application.

Specify both languages with [qt_standard_project_setup](qt-standard-project-setup.html):

::: pre
``` {.cpp translate="no"}
qt_standard_project_setup(I18N_TRANSLATED_LANGUAGES de fr)
```

Then call [qt_add_translations](qtlinguist-cmake-qt-add-translations.html) on the target that shall load the `.qm` files:

::: pre
``` {.cpp .prettyprint translate="no"}
qt_add_translations(helloworld)
```

On the first configuration, this command creates the files `helloworld_de.ts` and `helloworld_fr.ts` in the *source directory* of the project. These files will contain the translated strings and are supposed to be put under version control.

The command also creates build system rules to automatically generate `.qm` files from the `.ts` files. By default, the `.qm` files are embedded into a resource and are accessible under the `"/i18n"` resource prefix.

To update the entries in the `.ts` file, build the `update_translations` target:

::: pre
``` {.cpp .plain translate="no"}
$ cmake --build . --target update_translations
```

To trigger the generation of the `.qm` files manually, build the `release_translations` target:

::: pre
``` {.cpp .plain translate="no"}
$ cmake --build . --target release_translations
```

For more information about how to influence the handling of `.ts` files and the embedding into a resource, see the [qt_add_translations documentation](qtlinguist-cmake-qt-add-translations.html).

The [qt_add_translations](qtlinguist-cmake-qt-add-translations.html) command is a convenience wrapper. For more fine-grained control of how `.ts` files and `.qm` files are handled, use the underlying commands [qt_add_lupdate](qtlinguist-cmake-qt-add-lupdate.html) and [qt_add_lrelease](qtlinguist-cmake-qt-add-lrelease.html).

### Further reading

The official [CMake Documentation](https://cmake.org/cmake/help/latest/) is an invaluable source for working with CMake.

The official [CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/) covers common build system tasks.

The book [Professional CMake: A Practical Guide](https://crascit.com/professional-cmake/) provides a great introduction to the most relevant CMake features.

[Building a QML application](cmake-build-qml-application.html){.nextPage}

[©]{.abbr title="Copyright"} 2026 The Qt Company Ltd. Documentation contributions included herein are the copyrights of their respective owners. The documentation provided herein is licensed under the terms of the [GNU Free Documentation License version 1.3](http://www.gnu.org/licenses/fdl.html) as published by the Free Software Foundation. Qt and respective logos are [trademarks](https://doc.qt.io/qt/trademarks.html) of The Qt Company Ltd. in Finland and/or other countries worldwide. All other trademarks are property of their respective owners.
