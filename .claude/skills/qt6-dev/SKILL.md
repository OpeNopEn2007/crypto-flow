---
name: qt6-dev
description: |
  Qt 6 C++ 开发约束器（Harness）— 提供经过验证的代码模板和构建配置，避免模型凭记忆生成代码导致幻觉。
  TRIGGER when: 创建 Qt 6 项目、编写 Qt 6 CMakeLists.txt、实现 Qt 信号槽、
  开发 QML/C++ 混合应用、使用 Qt Model/View、Qt 数据库/网络编程、
  Qt 自定义控件、Qt 国际化、Qt 多线程、Qt 5 迁移到 Qt 6。
  DO NOT TRIGGER when: 纯 C++ 项目（无 Qt）、Qt 5 旧项目维护。
license: MIT
metadata:
  category: desktop-ui
  version: "2.0.0"
  sources:
    - Qt 6.11 Official Documentation (doc.qt.io)
    - Qt 6 CMake Manual
---

# Qt 6 Development Harness

本文件是 Qt 6 开发的**约束器**，不是参考文档。它的作用是：

1. **提供可直接复制的模板代码** — 模型照着写，不凭记忆
2. **列出必须遵守的规范** — 缺少任何一项都会导致构建失败
3. **标注禁止的写法** — 避免 Qt 5 遗留模式混入 Qt 6 项目

## 使用方式

当用户要求创建 Qt 6 项目或编写 Qt 6 代码时：
- 先从本文件找到对应的模板
- 按模板结构生成代码
- 逐项检查 Mandatory Checklist
- 不要凭记忆"自由发挥"

---

## CMakeLists.txt 模板（Widgets 应用）

**必须使用此模板，不要自行发挥。**

```cmake
cmake_minimum_required(VERSION 3.20)
project(MyApp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/mainwindow.h
    src/mainwindow.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)
```

**检查项：**
- `cmake_minimum_required` >= 3.16（Qt 6 最低要求）
- `CMAKE_CXX_STANDARD` = 20（Qt 6 推荐）
- `CMAKE_AUTOMOC ON` — 处理 Q_OBJECT 宏
- `CMAKE_AUTORCC ON` — 处理 .qrc 资源文件
- `CMAKE_AUTOUIC ON` — 处理 .ui 界面文件
- 缺少任何一项都会导致 MOC/UIC/RCC 不执行，构建报错

---

## CMakeLists.txt 模板（QML 应用）

**QML 项目必须使用 `qt_add_qml_module`，不要用 `qmlRegisterType`。**

```cmake
cmake_minimum_required(VERSION 3.20)
project(MyQmlApp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2)

qt_add_qml_module(${PROJECT_NAME}
    URI MyApp
    VERSION 1.0
    QML_FILES
        qml/Main.qml
    SOURCES
        src/backend.h
        src/backend.cpp
)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Quick
    Qt6::QuickControls2
)
```

---

## main.cpp 模板（Widgets 应用）

```cpp
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("MyApp");
    app.setOrganizationName("MyOrg");
    app.setApplicationVersion("1.0.0");

    MainWindow window;
    window.show();

    return app.exec();
}
```

**禁止：**
- 使用 `QGuiApplication` 替代 `QApplication`（Widgets 应用必须用 QApplication）
- 忘记设置 applicationName/organizationName（影响 QSettings 路径）

---

## main.cpp 模板（QML 应用）

```cpp
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.loadFromModule("MyApp", "Main");

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
```

**禁止：**
- 使用 `engine.load(QUrl(...))` — Qt 6 推荐 `loadFromModule`
- QML 应用使用 `QApplication`（应该用 QGuiApplication）

---

## MainWindow 模板

```cpp
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QMenuBar;
class QToolBar;
class QStatusBar;
class QTextEdit;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void newFile();
    void openFile();
    void saveFile();
    void about();

private:
    void createMenus();
    void createToolBar();
    void createStatusBar();
    void connectSignals();

    QTextEdit *m_textEdit = nullptr;
    QString m_currentFile;
};

#endif // MAINWINDOW_H
```

```cpp
#include "mainwindow.h"

#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_textEdit(new QTextEdit(this))
{
    setCentralWidget(m_textEdit);
    setWindowTitle(tr("My Application"));
    resize(800, 600);

    createMenus();
    createToolBar();
    createStatusBar();
    connectSignals();
}

void MainWindow::createMenus()
{
    // File 菜单
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *newAct = new QAction(tr("&New"), this);
    newAct->setShortcut(QKeySequence::New);
    fileMenu->addAction(newAct);

    QAction *openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(QKeySequence::Open);
    fileMenu->addAction(openAct);

    QAction *saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcut(QKeySequence::Save);
    fileMenu->addAction(saveAct);

    fileMenu->addSeparator();

    QAction *exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(QKeySequence::Quit);
    fileMenu->addAction(exitAct);

    // Help 菜单
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAct = new QAction(tr("&About"), this);
    helpMenu->addAction(aboutAct);
}

void MainWindow::createToolBar()
{
    QToolBar *toolBar = addToolBar(tr("Main"));
    toolBar->setMovable(false);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::connectSignals()
{
    // 信号槽必须使用新式语法
    // connect(sender, &Sender::signal, receiver, &Receiver::slot);
}

void MainWindow::newFile()
{
    m_textEdit->clear();
    m_currentFile.clear();
    setWindowTitle(tr("My Application"));
    statusBar()->showMessage(tr("New file"), 2000);
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, tr("Open File"), {},
        tr("Text Files (*.txt);;All Files (*)"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            m_textEdit->setPlainText(file.readAll());
            m_currentFile = fileName;
            setWindowTitle(QFileInfo(fileName).fileName());
            statusBar()->showMessage(tr("Opened: %1").arg(fileName), 2000);
        }
    }
}

void MainWindow::saveFile()
{
    if (m_currentFile.isEmpty()) {
        m_currentFile = QFileDialog::getSaveFileName(
            this, tr("Save File"), {},
            tr("Text Files (*.txt);;All Files (*)"));
    }

    if (!m_currentFile.isEmpty()) {
        QFile file(m_currentFile);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            file.write(m_textEdit->toPlainText().toUtf8());
            statusBar()->showMessage(tr("Saved: %1").arg(m_currentFile), 2000);
        }
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About"),
        tr("My Application v1.0\nA Qt 6 application."));
}
```

---

## 信号槽规范

**必须使用新式语法（编译时类型检查）：**

```cpp
// ✅ 正确：新式语法
connect(button, &QPushButton::clicked, this, &MainWindow::onClicked);
connect(button, &QPushButton::clicked, this, [this]() { doSomething(); });

// ❌ 禁止：旧式语法（运行时字符串匹配，无编译检查）
connect(button, SIGNAL(clicked()), this, SLOT(onClicked()));

// ❌ 禁止：混用新旧语法
connect(button, SIGNAL(clicked()), this, &MainWindow::onClicked);
```

**为什么禁止旧式语法：**
- 拼写错误在编译时不会报错，只在运行时默默失败
- 参数类型不匹配不会报错
- 无法连接到 lambda

---

## Model/View 模板

**自定义 Model 必须继承 QAbstractListModel 或 QAbstractTableModel：**

```cpp
class MyModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Roles { NameRole = Qt::UserRole + 1, ValueRole };

    explicit MyModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void addItem(const Item &item);
    void removeItem(int row);

private:
    QVector<Item> m_items;
};

// addItem 实现 — 必须用 beginInsertRows/endInsertRows 包裹
void MyModel::addItem(const Item &item)
{
    const int row = m_items.size();
    beginInsertRows(QModelIndex(), row, row);  // 必须在修改数据前调用
    m_items.append(item);
    endInsertRows();                            // 必须在修改数据后调用
}

// removeItem 实现
void MyModel::removeItem(int row)
{
    if (row < 0 || row >= m_items.size()) return;
    beginRemoveRows(QModelIndex(), row, row);
    m_items.removeAt(row);
    endRemoveRows();
}
```

**禁止：**
- 不调用 beginInsertRows/endInsertRows 直接修改数据（View 不会更新）
- 忘记实现 roleNames()（QML 无法访问数据）
- 在 begin/end 之间调用可能抛异常的代码

---

## QML + C++ 模板

**C++ 暴露给 QML 的类：**

```cpp
#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QQmlEngine>

class Backend : public QObject
{
    Q_OBJECT
    QML_ELEMENT  // 必须：替代旧的 qmlRegisterType

    Q_PROPERTY(QString userName READ userName NOTIFY userNameChanged)
    Q_PROPERTY(bool loading READ isLoading NOTIFY loadingChanged)

public:
    explicit Backend(QObject *parent = nullptr);

    QString userName() const { return m_userName; }
    bool isLoading() const { return m_loading; }

    Q_INVOKABLE void fetchData();  // QML 可调用的方法

signals:
    void userNameChanged();
    void loadingChanged();
    void errorOccurred(const QString &message);

private:
    void setLoading(bool loading);

    QString m_userName;
    bool m_loading = false;
};

#endif // BACKEND_H
```

**QML 中使用：**

```qml
import QtQuick
import QtQuick.Controls
import MyApp  // CMake 中 qt_add_qml_module 的 URI

ApplicationWindow {
    width: 400
    height: 300
    visible: true

    Backend {
        id: backend
        onUserNameChanged: nameLabel.text = userName
        onErrorOccurred: (msg) => console.error("Error:", msg)
    }

    Label {
        id: nameLabel
        text: backend.userName
        anchors.centerIn: parent
    }

    Button {
        text: "Fetch"
        onClicked: backend.fetchData()
        anchors.top: nameLabel.bottom
    }
}
```

**禁止：**
- 使用 `qmlRegisterType`（Qt 5 方式，Qt 6 用 QML_ELEMENT）
- Q_PROPERTY 缺少 NOTIFY 信号（QML 绑定不会更新）
- C++ 类没有 QML_ELEMENT 宏

---

## SQLite 数据库模板

```cpp
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

// 初始化
QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
db.setDatabaseName("app.db");

if (!db.open()) {
    qWarning() << "Database error:" << db.lastError().text();
    return false;
}

// 建表
QSqlQuery query;
query.exec(R"(
    CREATE TABLE IF NOT EXISTS items (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT NOT NULL,
        created_at DATETIME DEFAULT CURRENT_TIMESTAMP
    )
)");

// 参数化查询（防 SQL 注入）
query.prepare("INSERT INTO items (name) VALUES (?)");
query.addBindValue(itemName);
if (!query.exec()) {
    qWarning() << "Insert error:" << query.lastError().text();
}
```

**禁止：**
- 拼接 SQL 字符串（SQL 注入风险）
- 忘记检查 db.open() 和 query.exec() 的返回值

---

## 反模式速查

| 禁止写法 | 正确写法 | 原因 |
|---------|---------|------|
| `SIGNAL(clicked())` | `&QPushButton::clicked` | 编译时检查 vs 运行时字符串匹配 |
| `qmlRegisterType<T>()` | `QML_ELEMENT` | Qt 5 遗留 vs Qt 6 推荐 |
| `engine.load(QUrl(...))` | `engine.loadFromModule(...)` | Qt 6 推荐方式 |
| 直接修改 Model 数据 | beginInsertRows/endInsertRows | View 不会收到通知 |
| 非 GUI 线程操作控件 | QMetaObject::invokeMethod | 必然崩溃 |
| 拼接 SQL 字符串 | 参数化查询 | SQL 注入 |
| `QGuiApplication`（Widgets） | `QApplication` | Widgets 需要 QApplication |
| `CMAKE_CXX_STANDARD 17` | `CMAKE_CXX_STANDARD 20` | Qt 6 推荐 C++20 |

---

## 参考文档

详细 API 文档位于项目 `docs/referances/` 目录（71 个文件，676KB）：

| 类别 | 文件前缀 | 内容 |
|------|---------|------|
| 官方核心 | `qt6-official-` | Qt 6 模块概览 |
| 入门教程 | `qt6-tutorial-` | 信号槽、Model/View、事件、多线程 |
| 实践案例 | `qt6-practice-` | 项目结构、CMake、数据库、网络 |
| 高级主题 | `qt6-advanced-` | 性能、内存、自定义控件、QSS、i18n |
| 模块 API | `qt6-module-` | 42 个 Qt 6 模块文档 |
| 迁移指南 | `qt6-migration-` | Qt 5 → Qt 6 迁移 |
| 架构 | `qt6-meta-object-` / `qt6-plugin-` | 元对象系统、插件机制 |
