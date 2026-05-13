# Qt 6 QML 与 C++ 混合开发最佳实践

> 来源: https://doc.qt.io/qt-6/qtqml-index.html, https://doc.qt.io/qt-6/qqmlengine.html
> 获取日期: 2026-05-12

---

## 一、架构概述

Qt 6 中 QML 与 C++ 的混合开发模式：
- **C++**: 业务逻辑、数据处理、系统集成
- **QML**: UI 表现层、动画、布局
- **桥梁**: 通过 `qt_add_qml_module()` 在 CMake 中定义模块

### CMake 基础配置

```cmake
find_package(Qt6 REQUIRED COMPONENTS Quick)
target_link_libraries(myapp PRIVATE Qt6::Quick)
```

---

## 二、qt_add_qml_module - 核心 CMake 命令

Qt 6 的标准 QML 模块定义方式：

```cmake
qt_add_executable(MyApp
    main.cpp
)

qt_add_qml_module(MyApp
    URI MyApp
    VERSION 1.0
    QML_FILES Main.qml
    QML_FILES Settings.qml
    SOURCES
        backend.h backend.cpp
        datamodel.h datamodel.cpp
)

target_link_libraries(MyApp PRIVATE Qt6::Quick)
```

### 参数说明

| 参数 | 用途 |
|------|------|
| `URI` | QML 模块的导入路径（如 `import MyApp`） |
| `VERSION` | 模块版本号 |
| `QML_FILES` | QML 文件列表 |
| `SOURCES` | C++ 源文件（需要暴露给 QML 的类型） |
| `RESOURCES` | 资源文件 |

---

## 三、C++ 类暴露给 QML

### 1. 定义 C++ 类

```cpp
// backend.h
#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>
#include <QQmlEngine>

class Backend : public QObject
{
    Q_OBJECT
    // QML 属性：可读可写
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    // QML 属性：只读
    Q_PROPERTY(int itemCount READ itemCount NOTIFY itemCountChanged)
    // QML 可调用方法的信号
    Q_PROPERTY(bool processing READ isProcessing NOTIFY processingChanged)

public:
    explicit Backend(QObject *parent = nullptr);

    QString userName() const { return m_userName; }
    void setUserName(const QString &name);

    int itemCount() const { return m_items.size(); }
    bool isProcessing() const { return m_processing; }

    // 可从 QML 调用的方法
    Q_INVOKABLE void loadData(const QString &url);
    Q_INVOKABLE QVariantMap getItem(int index) const;

signals:
    void userNameChanged();
    void itemCountChanged();
    void processingChanged();
    void dataLoaded(const QVariantList &data);
    void errorOccurred(const QString &errorMessage);

private:
    QString m_userName;
    QVariantList m_items;
    bool m_processing = false;
};

#endif
```

### 2. 实现 C++ 类

```cpp
// backend.cpp
#include "backend.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>

Backend::Backend(QObject *parent) : QObject(parent) {}

void Backend::setUserName(const QString &name)
{
    if (m_userName != name) {
        m_userName = name;
        emit userNameChanged();
    }
}

void Backend::loadData(const QString &url)
{
    m_processing = true;
    emit processingChanged();

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl(url));

    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply, manager]() {
        m_processing = false;
        emit processingChanged();

        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            m_items = doc.array().toVariantList();
            emit itemCountChanged();
            emit dataLoaded(m_items);
        } else {
            emit errorOccurred(reply->errorString());
        }

        reply->deleteLater();
        manager->deleteLater();
    });
}

QVariantMap Backend::getItem(int index) const
{
    if (index >= 0 && index < m_items.size()) {
        return m_items[index].toMap();
    }
    return {};
}
```

---

## 四、main.cpp 启动配置

```cpp
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "backend.h"
#include "datamodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // 方法1: 设置上下文属性（简单但不推荐用于新项目）
    Backend backend;
    engine.rootContext()->setContextProperty("backend", &backend);

    // 方法2: 通过 QML 模块注册（推荐）
    // 在 qt_add_qml_module 的 SOURCES 中列出即可自动注册

    engine.loadFromModule("MyApp", "Main");

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
```

---

## 五、QML 端使用 C++ 对象

```qml
// Main.qml
import QtQuick
import QtQuick.Window
import QtQuick.Controls

Window {
    width: 400
    height: 300
    visible: true
    title: "Qt 6 QML + C++ Demo"

    Column {
        anchors.centerIn: parent
        spacing: 10

        // 绑定 C++ 属性
        Text {
            text: "Hello, " + backend.userName
            font.pixelSize: 24
        }

        Text {
            text: "Items: " + backend.itemCount
        }

        // 调用 C++ 方法
        TextField {
            id: nameInput
            placeholderText: "Enter your name"
        }

        Button {
            text: "Set Name"
            onClicked: backend.userName = nameInput.text
        }

        Button {
            text: "Load Data"
            onClicked: backend.loadData("https://api.example.com/items")
            enabled: !backend.processing
        }

        // 连接 C++ 信号
        Connections {
            target: backend

            function onDataLoaded(data) {
                console.log("Loaded", data.length, "items")
            }

            function onErrorOccurred(message) {
                errorDialog.text = message
                errorDialog.open()
            }
        }

        // 显示加载的数据
        ListView {
            width: 380
            height: 150
            model: backend.itemCount > 0 ? backend.itemCount : 0
            delegate: Text {
                text: backend.getItem(index).name || "Item " + index
            }
        }
    }

    Dialog {
        id: errorDialog
        property alias text: errorLabel.text
        title: "Error"
        modal: true
        anchors.centerIn: parent
        Label { id: errorLabel }
        standardButtons: Dialog.Ok
    }
}
```

---

## 六、数据模型暴露给 QML

### QAbstractListModel 子类

```cpp
// datamodel.h
#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QAbstractListModel>
#include <QQmlEngine>

class DataModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        ValueRole,
        StatusRole
    };

    explicit DataModel(QObject *parent = nullptr);

    // QAbstractListModel 接口
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // 自定义方法
    Q_INVOKABLE void addItem(const QString &name, int value);
    Q_INVOKABLE void removeItem(int index);
    Q_INVOKABLE void updateItem(int index, const QString &name, int value);

signals:
    void countChanged();

private:
    struct Item {
        QString name;
        int value;
        bool active;
    };
    QList<Item> m_items;
};

#endif
```

```cpp
// datamodel.cpp
#include "datamodel.h"

DataModel::DataModel(QObject *parent) : QAbstractListModel(parent) {}

int DataModel::rowCount(const QModelIndex &) const
{
    return m_items.size();
}

QVariant DataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_items.size())
        return {};

    const Item &item = m_items[index.row()];
    switch (role) {
    case NameRole:   return item.name;
    case ValueRole:  return item.value;
    case StatusRole: return item.active;
    default:         return {};
    }
}

QHash<int, QByteArray> DataModel::roleNames() const
{
    return {
        {NameRole, "name"},
        {ValueRole, "value"},
        {StatusRole, "active"}
    };
}

void DataModel::addItem(const QString &name, int value)
{
    beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
    m_items.append({name, value, true});
    endInsertRows();
    emit countChanged();
}

void DataModel::removeItem(int index)
{
    if (index < 0 || index >= m_items.size()) return;
    beginRemoveRows(QModelIndex(), index, index);
    m_items.removeAt(index);
    endRemoveRows();
    emit countChanged();
}

void DataModel::updateItem(int index, const QString &name, int value)
{
    if (index < 0 || index >= m_items.size()) return;
    m_items[index].name = name;
    m_items[index].value = value;
    QModelIndex modelIndex = createIndex(index, 0);
    emit dataChanged(modelIndex, modelIndex, {NameRole, ValueRole});
}
```

### QML 中使用模型

```qml
ListView {
    model: dataModel  // C++ QAbstractListModel 实例
    delegate: Item {
        width: parent.width
        height: 40

        Row {
            spacing: 10
            Text { text: model.name }
            Text { text: model.value }
            Text { text: model.active ? "Active" : "Inactive" }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: dataModel.removeItem(index)
        }
    }

    header: Text { text: "Items (" + dataModel.count + ")" }
}
```

---

## 七、QML 单例 (Singleton)

```cpp
// config.h
class Config : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QString theme READ theme WRITE setTheme NOTIFY themeChanged)

public:
    QString theme() const { return m_theme; }
    void setTheme(const QString &t) {
        if (m_theme != t) {
            m_theme = t;
            emit themeChanged();
        }
    }

signals:
    void themeChanged();

private:
    QString m_theme = "light";
};
```

QML 中使用：
```qml
import MyApp

Text {
    color: Config.theme === "dark" ? "white" : "black"
}
```

---

## 八、最佳实践总结

### 架构原则

1. **C++ 负责逻辑，QML 负责表现** - 不要在 QML 中写复杂业务逻辑
2. **使用 `qt_add_qml_module`** - 这是 Qt 6 的标准模块化方式
3. **使用 `Q_PROPERTY`** 暴露数据，配合 `NOTIFY` 信号实现自动绑定
4. **使用 `Q_INVOKABLE`** 暴露方法给 QML
5. **使用 `QML_ELEMENT`** 宏自动注册类型（Qt 6 推荐方式）

### 性能优化

1. **避免在 QML 中做大量计算** - 委托给 C++ 后端
2. **使用 `QAbstractListModel`** 替代 `QVariantList` 模型（大列表场景）
3. **QML 文件预编译** - `qt_add_qml_module` 自动处理
4. **使用 `QtObject`** 减少 QML 对象层级

### 信号槽跨层调用

```cpp
// C++ 端定义信号
signals:
    void dataUpdated(const QVariantMap &data);

// QML 端连接
Connections {
    target: backend
    function onDataUpdated(data) {
        // 更新 UI
    }
}
```

```qml
// QML 端触发 C++ 方法
Button {
    onClicked: backend.loadData(urlInput.text)
}
```
