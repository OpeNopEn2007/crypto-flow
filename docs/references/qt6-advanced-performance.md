# Qt 6 性能优化指南

> 来源: https://doc.qt.io/qt-6/performance.html, https://doc.qt.io/qt-6/qrhi.html, https://doc.qt.io/qt-6/qtquick-performance.html
> 获取日期: 2026-05-12

---

## 1. 渲染与图形优化

### 1.1 RHI (Rendering Hardware Interface)

Qt 6 引入了 RHI 抽象层，支持 OpenGL、Vulkan、Metal、Direct3D 等后端，自动选择最优 GPU 加速方案。

```cpp
// 选择渲染后端
// Qt 会根据平台自动选择最佳后端（Metal on macOS, Vulkan/OpenGL on Linux, D3D11 on Windows）
// 可通过环境变量覆盖:
// QT_RHI_BACKEND=opengl  (或 vulkan, metal, d3d11, d3d12)
```

### 1.2 减少过度绘制 (Overdraw)

- 使用 `QSG_RENDER_TIMING=1` 环境变量检测渲染瓶颈
- 在 Qt Quick 中谨慎使用 `layer.enabled: true`，仅在复杂项上启用图层缓存
- 减少透明和半透明元素的嵌套层级

### 1.3 帧捕获与性能分析工具

| 工具 | 平台 | 支持 API | 用途 |
|------|------|----------|------|
| RenderDoc | Windows, Linux | OpenGL, Vulkan, D3D11, D3D12 | 帧捕获、管线状态分析 |
| NVIDIA Nsight Graphics | Windows, Linux (NVIDIA) | 全部 | 图形调试、时序分析 |
| AMD Radeon GPU Profiler | AMD 平台 | 全部 | 渲染分析、硬件级剖析 |
| PresentMon | 多平台 | 多厂商 | 实时帧率覆盖 |
| XCode Metal Debugger | macOS | Metal | 帧捕获、着色器调试 |

macOS 13+ 可通过 `MTL_HUD_ENABLED=1` 启用性能覆盖层。

### 1.4 Debug Markers

```cpp
// 为渲染通道添加调试标记
painter->debugMarkBegin("Background Pass");
// ... 绘制操作 ...
painter->debugMarkEnd();
painter->debugMarkMsg("Starting main render");
```

---

## 2. C++ 层优化

### 2.1 QString 内存预分配

```cpp
// 错误: 频繁重新分配
QString result;
for (int i = 0; i < 1000; ++i) {
    result.append("item" + QString::number(i));  // 多次 realloc
}

// 正确: 预分配内存
QString result;
result.reserve(1000);  // 预分配空间
for (int i = 0; i < 1000; ++i) {
    result.append("item" + QString::number(i));
}
result.squeeze();  // 释放多余空间
```

### 2.2 减少 C++/QML 边界跨越

```cpp
// 不推荐: 频繁跨边界调用
for (int i = 0; i < count; ++i) {
    // 每次循环都跨 C++/QML 边界
    QMetaObject::invokeMethod(qmlObj, "updateItem", 
        Q_ARG(QVariant, QVariant::fromValue(data[i])));
}

// 推荐: 批量传递数据
QVariantList items;
for (int i = 0; i < count; ++i) {
    items.append(QVariant::fromValue(data[i]));
}
QMetaObject::invokeMethod(qmlObj, "updateAllItems", 
    Q_ARG(QVariant, QVariant::fromValue(items)));
```

### 2.3 使用 Q_PROPERTY 高效通知

```cpp
class CryptoPrice : public QObject {
    Q_OBJECT
    Q_PROPERTY(double price READ price NOTIFY priceChanged)
    Q_PROPERTY(QString symbol READ symbol CONSTANT)
    
public:
    double price() const { return m_price; }
    QString symbol() const { return m_symbol; }
    
signals:
    void priceChanged(double newPrice);
    
private:
    double m_price = 0.0;
    QString m_symbol;
};
```

---

## 3. QML 启动性能

### 3.1 使用 QML 编译器 (AOT)

```cmake
# CMakeLists.txt - 启用 QML 编译器
find_package(Qt6 REQUIRED COMPONENTS QmlCompiler)
qt_standard_project_setup()
```

QML 编译器将 QML 预编译为 C++，减少启动时解析时间。

### 3.2 懒加载组件

```qml
// 使用 Loader 延迟加载
Loader {
    id: settingsLoader
    active: false  // 初始不加载
    
    sourceComponent: SettingsPage {
        // 只在 active=true 时才创建
    }
}

// 按需激活
Button {
    text: "Settings"
    onClicked: settingsLoader.active = true
}
```

### 3.3 减少初始 QML 解析时间

- 将大型 QML 文件拆分为多个小文件
- 使用 `QtObject` 模式替代内联组件
- 避免在根组件中导入不必要的模块

---

## 4. 线程与并发

### 4.1 QtConcurrent::run

```cpp
#include <QtConcurrent>

// 基本用法 - 在默认线程池中执行
QFuture<QString> future = QtConcurrent::run([]() {
    // 耗时操作
    return processLargeDataset();
});

// 自定义线程池
QThreadPool pool;
pool.setMaxThreadCount(4);
QFuture<void> future = QtConcurrent::run(&pool, []() {
    heavyComputation();
});

// 监听完成
QFutureWatcher<QString> *watcher = new QFutureWatcher<QString>(this);
connect(watcher, &QFutureWatcher<QString>::finished, this, [=]() {
    QString result = future.result();
    // 处理结果
});
watcher->setFuture(future);
```

### 4.2 QThread 资源清理最佳实践

```cpp
QThread *thread = new QThread(this);
MyWorker *worker = new MyWorker();
worker->moveToThread(thread);

// 连接信号用于工作和清理
connect(thread, &QThread::started, worker, &MyWorker::doWork);
connect(worker, &MyWorker::finished, thread, &QThread::quit);
connect(thread, &QThread::finished, worker, &QObject::deleteLater);
connect(thread, &QThread::finished, thread, &QObject::deleteLater);

thread->start();
```

关键点:
- 使用 `finished()` 信号触发清理
- 连接到 `QObject::deleteLater()` 实现安全删除
- `deleteLater()` 在对象的事件循环上调度删除，确保所有待处理事件完成后再删除

### 4.3 QThreadPool 复用

```cpp
// 复用线程池避免频繁创建/销毁线程
static QThreadPool *globalPool() {
    static QThreadPool pool;
    pool.setMaxThreadCount(QThread::idealThreadCount());
    return &pool;
}

// 使用
QtConcurrent::run(globalPool(), []() { /* work */ });
```

---

## 5. 基准测试

### 5.1 使用 QBENCHMARK

```cpp
#include <QTest>

class PerformanceTest : public QObject {
    Q_OBJECT

private slots:
    void stringComparisonBenchmark() {
        QString string1 = "hello world";
        QString string2 = "hello Qt";
        
        QBENCHMARK {
            string1.localeAwareCompare(string2);
        }
    }
    
    void dataProcessingBenchmark() {
        QVector<int> data;
        for (int i = 0; i < 100000; ++i) {
            data.append(i);
        }
        
        QBENCHMARK {
            std::sort(data.begin(), data.end());
        }
    }
};
```

### 5.2 CPU 计时

```cpp
#include <QElapsedTimer>

QElapsedTimer timer;
timer.start();

// 执行操作
heavyFunction();

qint64 elapsed = timer.elapsed();  // 毫秒
qDebug() << "Operation took" << elapsed << "ms";
```

### 5.3 GPU 计时

```cpp
// 启用时间戳
QRhiInitParams params;
params.enableTimestamps = true;

// 获取 GPU 时间
float gpuTime = commandBuffer->lastCompletedGpuTime();  // 仅部分 API 支持
```

---

## 6. 内存管理

### 6.1 避免不必要的 QVariant 复制

```cpp
// 不推荐: 频繁创建 QVariant
for (int i = 0; i < count; ++i) {
    QVariant v = QVariant::fromValue(data[i]);
    // 每次创建新的 QVariant 对象
}

// 推荐: 使用自定义注册类型
qRegisterMetaType<MyStruct>("MyStruct");
// 直接传递 MyStruct，避免 QVariant 包装开销
```

### 6.2 QLinkedList 替代方案

Qt 6 移除了 `QLinkedList`，使用 `std::deque` 替代:

```cpp
// Qt 5
QLinkedList<int> list;

// Qt 6 替代
std::deque<int> list;
// 或根据场景选择 std::list 或 QList
```

### 6.3 分析工具

- **Valgrind / Heaptrack**: 内存泄漏检测
- **Qt Creator 内置分析器**: CPU 和内存分析
- **Intel VTune / AMD uProf**: 底层性能分析

---

## 7. 二进制大小与部署

```cmake
# 静态构建或选择性链接
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
# 只链接需要的模块

# 启用 LTO (Link-Time Optimization)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

# Release 构建设置
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
```

最佳实践:
- 使用静态构建减小依赖体积
- 启用 LTO 优化
- Release 构建剥离调试符号
- 使用 Qt 6 CMake 模块化导入

---

## 8. 环境变量参考

| 变量 | 用途 |
|------|------|
| `QSG_RENDER_TIMING=1` | 输出渲染时序信息 |
| `QML_USE_GLYPHCACHE_WORKAROUND=1` | 修复 OpenGL 驱动字形损坏问题 |
| `QT_RHI_BACKEND=opengl` | 强制指定渲染后端 |
| `MTL_HUD_ENABLED=1` | macOS Metal 性能覆盖层 |

---

## 参考资源

- [Qt 6 Performance](https://doc.qt.io/qt-6/performance.html)
- [Qt 6 QRhi](https://doc.qt.io/qt-6/qrhi.html)
- [Qt Quick Performance](https://doc.qt.io/qt-6/qtquick-performance.html)
- [Qt Concurrent](https://doc.qt.io/qt-6/qtconcurrent.html)
- [KDAB Qt Performance Talks](https://www.kdab.com/resources)
- [Qt Wiki - Performance](https://wiki.qt.io/Performance)
