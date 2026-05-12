# Qt 6 Getting Started Tutorial

> Source: https://doc.qt.io/qt-6/gettingstarted.html
> Fetched: 2026-05-12
> Additional references: https://doc.qt.io/qt-6/qtwidgets-index.html, https://doc.qt.io/qt-6/cmake-get-started.html

---

## Overview

Qt 6 is a cross-platform application framework used for developing applications that can run on various operating systems including Windows, macOS, Linux, Android, and iOS. This guide covers the essential steps to get started with Qt 6 development.

## Prerequisites

- **Qt 6 SDK**: Download from https://www.qt.io/download
- **Qt Creator IDE**: Included with the Qt SDK installation
- **CMake** (3.16+): The official build system for Qt 6 (replaces qmake)
- **C++17** compatible compiler (GCC 9+, Clang 9+, MSVC 2019+)

## Installing Qt 6

### Using the Qt Online Installer

1. Download the Qt Online Installer from https://www.qt.io/download-qt-installer
2. Run the installer and follow the setup wizard
3. Select the Qt 6 version and the components you need (e.g., Qt 6.x for Desktop development)
4. Choose your installation directory

### Verifying Installation

After installation, verify by opening Qt Creator and checking:
- Help > About Qt Creator (version info)
- Kits > Desktop Qt 6.x (available kits)

## Creating Your First Application

### Project Setup with CMake

Qt 6 uses CMake as its official build system. Create a `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.16)

project(MyApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup(REQUIRES 6.5)

add_executable(MyApp
    main.cpp
)

target_link_libraries(MyApp PRIVATE Qt6::Widgets)
```

### Hello World Example

```cpp
// main.cpp
#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create main window
    QWidget window;
    window.setWindowTitle("My First Qt 6 App");
    window.resize(400, 300);

    // Create layout
    QVBoxLayout *layout = new QVBoxLayout(&window);

    // Create widgets
    QLabel *label = new QLabel("Hello, Qt 6!");
    label->setAlignment(Qt::AlignCenter);

    QPushButton *button = new QPushButton("Click Me");

    // Connect signal to slot
    QObject::connect(button, &QPushButton::clicked, [&]() {
        label->setText("Button was clicked!");
    });

    // Add widgets to layout
    layout->addWidget(label);
    layout->addWidget(button);

    window.show();
    return app.exec();
}
```

### Building and Running

```bash
# Configure
cmake -B build -DCMAKE_PREFIX_PATH=/path/to/qt/6.x.x/gcc_64

# Build
cmake --build build

# Run
./build/MyApp
```

## Key Concepts

### Widgets vs QML

Qt 6 offers two main approaches for building UIs:

| Feature | Widgets (C++) | QML (Qt Quick) |
|---------|---------------|----------------|
| Language | C++ | QML + JavaScript |
| UI Style | Traditional desktop | Modern, animated |
| Best For | Desktop apps | Mobile, touch, embedded |
| Learning Curve | Moderate | Easy for UI, JS needed |
| Customization | Full C++ control | Declarative bindings |

### Qt Creator IDE

Qt Creator is the official IDE for Qt development:

- **Design Mode**: Visual UI editing with Qt Designer
- **Edit Mode**: Code editing with IntelliSense-like features
- **Projects Mode**: Build, run, and deploy configuration
- **Debug Mode**: Integrated debugging with breakpoint support
- **Help Mode**: Offline Qt documentation browser

### Signal and Slot Mechanism

The core communication mechanism in Qt (covered in detail in qt6-tutorial-signals-slots.md):

```cpp
// Modern Qt 5/6 syntax (recommended)
connect(sender, &SenderClass::signal, receiver, &ReceiverClass::slot);

// Lambda syntax
connect(button, &QPushButton::clicked, this, [=]() {
    // handle click
});
```

### Layout Management

Qt provides several layout managers for arranging widgets:

- **QHBoxLayout**: Horizontal arrangement
- **QVBoxLayout**: Vertical arrangement
- **QGridLayout**: Grid-based arrangement
- **QFormLayout**: Label-field pair arrangement
- **QStackedLayout**: Stacked widgets (only one visible)

```cpp
QGridLayout *gridLayout = new QGridLayout;
gridLayout->addWidget(new QLabel("Name:"), 0, 0);
gridLayout->addWidget(new QLineEdit, 0, 1);
gridLayout->addWidget(new QLabel("Email:"), 1, 0);
gridLayout->addWidget(new QLineEdit, 1, 1);
gridLayout->addWidget(new QPushButton("Submit"), 2, 0, 1, 2);
```

## Building Systems

### CMake (Recommended for Qt 6)

```cmake
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Network)

qt_standard_project_setup(REQUIRES 6.5)

add_executable(myapp main.cpp mainwindow.cpp mainwindow.h)

target_link_libraries(myapp PRIVATE
    Qt6::Widgets
    Qt6::Core
    Qt6::Network
)
```

### qmake (Legacy, still supported)

```pro
QT += widgets network
TARGET = myapp
TEMPLATE = app
SOURCES += main.cpp mainwindow.cpp
HEADERS += mainwindow.h
```

## Qt Academy Courses

For visual learning, Qt provides structured courses:

| Course | Topic |
|--------|-------|
| How to Install Qt | Installation walkthrough |
| Introduction to Qt Quick | QML fundamentals |
| Create a Simple Qt Quick App | First QML project |
| Install Qt Design Studio | Visual design tool |
| Create Your First App with Qt Design Studio | Design-driven development |
| Must-knows of UI Design | UI/UX principles |

## Recommended Learning Path

1. **Install Qt 6** and Qt Creator
2. **Follow Qt Academy** introductory courses
3. **Build Hello World** with Widgets
4. **Learn Signals & Slots** (core mechanism)
5. **Explore Model/View** for data-driven UIs
6. **Try Qt Quick/QML** for modern UIs
7. **Study event handling** and multithreading
8. **Build a real project** to consolidate knowledge

## Official Resources

- **Documentation**: https://doc.qt.io/qt-6/
- **Examples Index**: https://doc.qt.io/qt-6/examples-index.html
- **Qt Forum**: https://forum.qt.io/
- **Qt Blog**: https://www.qt.io/blog
- **Qt Wiki**: https://wiki.qt.io/
