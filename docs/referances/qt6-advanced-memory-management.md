# Qt 6 内存管理与智能指针

> 来源: https://doc.qt.io/qt-6/qobject.html, https://doc.qt.io/qt-6/objecttrees.html, https://doc.qt.io/qt-6/qscopedpointer.html, https://doc.qt.io/qt-6/qsharedpointer.html
> 获取日期: 2026-05-12

---

## 1. QObject 父子对象树

Qt 的核心内存管理机制是基于 `QObject` 的**所有权模型**。

### 1.1 基本原理

当一个 `QObject` 被赋予父对象时，父对象析构时会自动删除所有子对象，形成树形结构实现自动清理。

```cpp
// 父子关系 - 子对象随父对象自动删除
int main()
{
    QWidget window;
    QPushButton quit("Quit", &window);  // quit 的父对象是 window
    // ...
}
// 退出作用域时: quit 析构 -> window 析构 (顺序正确)
```

### 1.2 堆上创建

```cpp
// 方式 1: 在构造时指定父对象
QPushButton *button = new QPushButton("Click", parentWidget);
// parentWidget 析构时自动删除 button

// 方式 2: 创建后设置父对象
QPushButton *button = new QPushButton("Click");
button->setParent(parentWidget);

// 方式 3: 通过布局管理
QVBoxLayout *layout = new QVBoxLayout(this);
layout->addWidget(new QLabel("Hello"));  // label 成为 layout 的子对象
```

### 1.3 关键 API

```cpp
// 获取父对象
QObject *parent = object->parent();

// 获取所有直接子对象
QObjectList children = object->children();

// 查找子对象
QObject *child = object->findChild<QPushButton*>("buttonName");
QList<QObject*> allLabels = object->findChildren<QLabel*>();
```

---

## 2. Qt 智能指针

### 2.1 QScopedPointer (类似 std::unique_ptr)

独占所有权，作用域结束时自动释放。

```cpp
void processFile()
{
    QScopedPointer<FileHandler> handler(new FileHandler());
    
    if (handler->open("data.csv")) {
        handler->process();
    }
    // handler 离开作用域时自动删除
}

// 不可复制，但可移动
QScopedPointer<Data> ptr1(new Data());
// QScopedPointer<Data> ptr2 = ptr1;  // 错误! 编译失败
QScopedPointer<Data> ptr2 = std::move(ptr1);  // 正确! 移动所有权
```

### 2.2 QSharedPointer (类似 std::shared_ptr)

引用计数，多个指针共享同一对象。

```cpp
// 基本用法
QSharedPointer<Config> config(new Config());
QSharedPointer<Config> configCopy = config;  // 引用计数 +1
// 两个指针都析构后，Config 对象才被删除

// 自定义删除器
QSharedPointer<File> file(new File("data.bin"), [](File *f) {
    f->close();
    delete f;
});

// 检查引用计数
qDebug() << "Reference count:" << configStrongRef;

// 线程安全
// QSharedPointer 的引用计数操作是原子的
```

### 2.3 QPointer (弱引用，类似 std::weak_ptr)

观察 QObject 而不拥有它，对象被删除后自动置为 `nullptr`。

```cpp
// 场景: 安全地检查对象是否还存在
QPointer<ImportantObject> observer = importantObject;

// ... 一段时间后 ...
if (observer) {
    // 对象仍然存在，安全使用
    observer->doSomething();
} else {
    // 对象已被删除
    qDebug() << "Object was deleted";
}

// 常用于跨模块/跨线程的安全引用
class Worker : public QObject {
    Q_OBJECT
public:
    QPointer<QProgressBar> progressBar;
    
    void updateProgress(int value) {
        if (progressBar) {
            progressBar->setValue(value);
        }
    }
};
```

### 2.4 Qt 6 中的 std 智能指针

```cpp
// std::unique_ptr - 适用于非 QObject 类型
std::unique_ptr<NonQObject> data = std::make_unique<NonQObject>();

// std::shared_ptr - 适用于需要共享所有权的场景
std::shared_ptr<Config> config = std::make_shared<Config>();

// 重要: 不要对有父对象的 QObject 使用 std::unique_ptr!
// 错误: 双重删除
auto button = std::make_unique<QPushButton>("Click", parentWidget);
// parentWidget 删除 button，然后 unique_ptr 也尝试删除 -> 崩溃!

// 正确: QObject + 父子关系
auto button = new QPushButton("Click", parentWidget);
// 由 parentWidget 管理生命周期
```

---

## 3. 最佳实践对照表

| 场景 | 推荐方式 | 原因 |
|------|----------|------|
| UI 组件 (QWidget) | 父子对象树 | Qt 原生机制，自动清理 |
| 非 QObject 的局部对象 | `std::unique_ptr` / `QScopedPointer` | 确定性析构 |
| 共享数据 | `std::shared_ptr` / `QSharedPointer` | 引用计数自动释放 |
| 观察 QObject (不拥有) | `QPointer` | 安全检查对象是否存活 |
| 跨线程引用 | `QPointer` | 原子操作保证线程安全 |
| 延迟删除 | `deleteLater()` | 在事件循环中安全删除 |

---

## 4. 常见陷阱

### 4.1 双重删除

```cpp
// 错误: std::unique_ptr + Qt 父子关系
auto widget = std::make_unique<QWidget>(parent);
// 父对象删除 widget，unique_ptr 再次删除 -> 崩溃

// 正确: 二选一
// 方式 A: 使用父子关系
auto widget = new QWidget(parent);
// 方式 B: 使用智能指针，不设父对象
auto widget = std::make_unique<QWidget>();
// 手动管理生命周期
```

### 4.2 长期持有裸指针

```cpp
// 错误: 可能成为悬空指针
class MyWidget : public QWidget {
    SomeObject *dangerousPtr;  // 对象可能被其他地方删除
};

// 正确: 使用 QPointer
class MyWidget : public QWidget {
    QPointer<SomeObject> safePtr;  // 对象删除后自动置 nullptr
};
```

### 4.3 循环引用

```cpp
// 错误: 循环引用导致内存泄漏
class A : public QObject {
    Q_OBJECT
public:
    QSharedPointer<B> bRef;
};

class B : public QObject {
    Q_OBJECT
public:
    QSharedPointer<A> aRef;  // 循环引用!
};

// 正确: 一方使用弱引用
class B : public QObject {
    Q_OBJECT
public:
    QWeakPointer<A> aWeakRef;  // 打破循环
};
```

### 4.4 容器中的指针

```cpp
// QList<T*> 不拥有指针
QList<QPushButton*> buttons;
// 析构时不会删除按钮

// 方案 1: 手动清理
qDeleteAll(buttons);

// 方式 2: 使用智能指针容器
QVector<std::unique_ptr<QPushButton>> smartButtons;

// 方式 3: 使用 Qt 容器 + QObject 父子关系
// 让布局管理组件生命周期，容器只存引用
```

---

## 5. deleteLater() 详解

```cpp
// deleteLater() 在事件循环中安全删除对象
// 避免在信号/槽调用链中直接 delete

// 场景: 从槽函数中安全删除发送者
void MyClass::onButtonClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (btn) {
        btn->deleteLater();  // 安全: 在当前事件处理完后删除
        // 不要直接 delete btn; 可能导致崩溃
    }
}

// 场景: 跨线程安全删除
void MyClass::cleanup()
{
    // 不能直接 delete，因为对象可能在其他线程
    object->deleteLater();  // 安全: 在对象所属线程的事件循环中删除
}
```

---

## 6. QScopedValueRollback

临时修改值并在离开作用域时恢复:

```cpp
void MyClass::temporaryState()
{
    QScopedValueRollback<bool> rollback(m_processing);
    m_processing = true;  // 临时设置
    
    // 执行处理...
    doWork();
    
}  // m_processing 自动恢复为 false
```

---

## 7. 内存分析工具

- **Valgrind**: 经典内存泄漏检测
- **Heaptrack**: 跟踪内存分配，生成分析报告
- **Qt Creator 内置分析器**: CPU + 内存分析
- **AddressSanitizer (ASan)**: 编译时启用，检测越界、双重释放等

```cmake
# 启用 AddressSanitizer
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
```

---

## 参考资源

- [QObject](https://doc.qt.io/qt-6/qobject.html)
- [Object Trees & Ownership](https://doc.qt.io/qt-6/objecttrees.html)
- [QScopedPointer](https://doc.qt.io/qt-6/qscopedpointer.html)
- [QSharedPointer](https://doc.qt.io/qt-6/qsharedpointer.html)
- [QPointer](https://doc.qt.io/qt-6/qpointer.html)
- [deleteLater()](https://doc.qt.io/qt-6/qobject.html#deleteLater)
