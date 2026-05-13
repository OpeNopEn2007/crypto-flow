# Qt 6 Multithreading

> Source: https://doc.qt.io/qt-6/threads-technologies.html
> Additional: https://doc.qt.io/qt-6/qthread.html
> Fetched: 2026-05-12

---

## Overview

Qt offers many classes and functions for working with threads. This guide covers four different approaches that Qt programmers can use to implement multithreaded applications, along with synchronization mechanisms and best practices.

## Multithreading Technologies in Qt

### 1. QThread: Low-Level API with Optional Event Loops

QThread is the foundation of all thread control in Qt. Each QThread instance represents and controls one thread.

QThread can either be instantiated directly or subclassed. Instantiating a QThread provides a parallel event loop, allowing QObject slots to be invoked in a secondary thread. Subclassing a QThread allows the application to initialize the new thread before starting its event loop, or to run parallel code without an event loop.

#### Worker Object Approach (Recommended)

```cpp
class Worker : public QObject
{
    Q_OBJECT

public slots:
    void doWork(const QString &parameter) {
        QString result;
        /* ... here is the expensive or blocking operation ... */
        emit resultReady(result);
    }

signals:
    void resultReady(const QString &result);
};

class Controller : public QObject
{
    Q_OBJECT
    QThread workerThread;

public:
    Controller() {
        Worker *worker = new Worker;
        worker->moveToThread(&workerThread);
        connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &Controller::operate, worker, &Worker::doWork);
        connect(worker, &Worker::resultReady, this, &Controller::handleResults);
        workerThread.start();
    }

    ~Controller() {
        workerThread.quit();
        workerThread.wait();
    }

public slots:
    void handleResults(const QString &);

signals:
    void operate(const QString &);
};
```

**Key points:**
- Worker lives in the new thread
- Communication via queued signal-slot connections (thread-safe)
- Worker's slots execute in the worker thread
- Clean shutdown with `quit()` and `wait()`

#### Subclassing QThread

```cpp
class WorkerThread : public QThread
{
    Q_OBJECT

public:
    explicit WorkerThread(QObject *parent = nullptr) : QThread(parent) { }

protected:
    void run() override {
        QString result;
        /* ... here is the expensive or blocking operation ... */
        emit resultReady(result);
    }

signals:
    void resultReady(const QString &s);
};

void MyObject::startWorkInAThread()
{
    WorkerThread *workerThread = new WorkerThread(this);
    connect(workerThread, &WorkerThread::resultReady,
            this, &MyObject::handleResults);
    connect(workerThread, &WorkerThread::finished,
            workerThread, &QObject::deleteLater);
    workerThread->start();
}
```

**Key points:**
- `run()` executes in the new thread
- The thread exits after `run()` returns
- No event loop unless you call `exec()`
- The QThread instance itself lives in the **old** thread, not the new one

**Important**: A QThread instance lives in the old thread that instantiated it, not in the new thread that calls `run()`. This means all of QThread's queued slots and invoked methods will execute in the old thread. To invoke slots in the new thread, use the worker-object approach.

#### QThread::create() (Qt 6)

```cpp
// Create thread with a function
QThread *thread = QThread::create([this]() {
    // This runs in a new thread
    doExpensiveOperation();
    emit operationComplete();
});
thread->start();
```

### 2. QThreadPool and QRunnable: Reusing Threads

Creating and destroying threads frequently can be expensive. To reduce this overhead, existing threads can be reused for new tasks. QThreadPool is a collection of reusable QThreads.

```cpp
class MyTask : public QRunnable
{
    void run() override {
        // Your task code here
        qDebug() << "Task running in thread" << QThread::currentThread();
    }
};

// Use the global thread pool
QThreadPool::globalInstance()->start(new MyTask);

// Or use a custom thread pool
QThreadPool pool;
pool.setMaxThreadCount(4);
pool.start(new MyTask);
```

**Key points:**
- Each Qt application has a global thread pool via `QThreadPool::globalInstance()`
- Automatically maintains optimal number of threads based on CPU cores
- Tasks are `QRunnable` subclasses with `run()` implemented
- Tasks run once and are deleted after completion

### 3. QtConcurrent: High-Level API

The Qt Concurrent module provides high-level functions for common parallel computation patterns: map, filter, and reduce. These functions never require the use of low-level threading primitives such as mutexes or semaphores.

#### QtConcurrent::run()

```cpp
// Run a function in a thread pool thread
QFuture<QString> future = QtConcurrent::run([]() {
    return QString("Result from thread");
});

// With arguments
QFuture<void> future = QtConcurrent::run([](int a, int b) {
    qDebug() << "Sum:" << a + b;
}, 10, 20);

// Using QFutureWatcher for signal-slot interface
QFutureWatcher<QString> *watcher = new QFutureWatcher<QString>(this);
connect(watcher, &QFutureWatcher<QString>::finished, this, [watcher]() {
    qDebug() << "Result:" << watcher->result();
});
watcher->setFuture(future);
```

#### QtConcurrent::map() - Apply operation to all items

```cpp
QList<QImage> images = loadImages();

// Apply thumbnail generation to all images in parallel
QFuture<void> future = QtConcurrent::map(images, [](QImage &image) {
    image = image.scaled(100, 100, Qt::KeepAspectRatio);
});
```

#### QtConcurrent::filter() - Filter items

```cpp
QList<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// Keep only even numbers
QFuture<void> future = QtConcurrent::filter(numbers, [](int &n) {
    n = (n % 2 == 0) ? n : 0;
});
```

#### QtConcurrent::mappedReduced() - Map and reduce

```cpp
QList<QString> words = {"hello", "world", "qt", "concurrent"};

// Count total characters
QFuture<int> future = QtConcurrent::mappedReduced(
    words,
    [](const QString &word) { return word.length(); },  // map
    [](int &result, const int &value) { result += value; }  // reduce
);
// future.result() == 15
```

### 4. WorkerScript: Threading in QML

The WorkerScript QML type lets JavaScript code run in parallel with the GUI thread.

```qml
// main.qml
import QtQuick 2.15

Item {
    WorkerScript {
        id: worker
        source: "worker.js"
        onMessage: function(message) {
            console.log("Reply:", message.result)
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: worker.sendMessage({ 'text': 'Hello from GUI' })
    }
}
```

```javascript
// worker.js
WorkerScript.onMessage = function(message) {
    // Heavy computation here
    var result = expensiveCalculation(message.text)
    WorkerScript.sendMessage({ 'result': result })
}
```

## Comparison of Solutions

| Feature | QThread | QRunnable/QThreadPool | QtConcurrent::run() | Qt Concurrent (Map/Filter/Reduce) | WorkerScript |
|---------|---------|----------------------|---------------------|-----------------------------------|--------------|
| Language | C++ | C++ | C++ | C++ | QML |
| Thread priority | Yes | Yes | No | No | No |
| Event loop | Yes | No | No | No | No |
| Signal updates | Yes | No | No | Yes (via QFutureWatcher) | Yes |
| QFuture monitoring | No | No | Partial | Yes | No |
| Pause/resume/cancel | No | No | No | Yes | No |

## Choosing an Approach

### When to Use Each

| Thread Lifetime | Operation | Solution |
|----------------|-----------|----------|
| **One call** | Run a new linear function in another thread, with optional progress | `QThread::run()`, `QRunnable::run()`, or `QtConcurrent::run()` |
| **One call** | Run an existing function and get return value | `QtConcurrent::run()` + `QFutureWatcher` |
| **One call** | Perform operation on all items of a container | `QtConcurrent::filter()` + `QtConcurrent::map()` |
| **One call/Permanent** | Long computation in pure QML app | `WorkerScript` |
| **Permanent** | Object performing different tasks on request | Worker object + `QThread` + `moveToThread()` |
| **Permanent** | Repeated expensive operation without signals/events | Subclass `QThread` + override `run()` |

## Thread Synchronization

### QMutex

Provides mutual exclusion for protecting shared data:

```cpp
class ThreadSafeCounter
{
public:
    void increment() {
        QMutexLocker locker(&m_mutex);
        m_counter++;
    }

    int value() const {
        QMutexLocker locker(&m_mutex);
        return m_counter;
    }

private:
    mutable QMutex m_mutex;
    int m_counter = 0;
};
```

### QReadWriteLock

Allows concurrent reads but exclusive writes:

```cpp
QReadWriteLock lock;

// Reading (multiple threads can read simultaneously)
{
    QReadLocker locker(&lock);
    // Read shared data
}

// Writing (exclusive access)
{
    QWriteLocker locker(&lock);
    // Write shared data
}
```

### QSemaphore

Provides a counting semaphore for thread synchronization:

```cpp
QSemaphore sem(5); // 5 available resources

// Acquire a resource
sem.acquire();

// Try to acquire (non-blocking)
if (sem.tryAcquire()) {
    // Got a resource
}

// Release a resource
sem.release();
```

### QWaitCondition

Allows threads to wait for a condition to be met:

```cpp
QMutex mutex;
QWaitCondition condition;
bool dataReady = false;

// Producer
{
    QMutexLocker locker(&mutex);
    dataReady = true;
    condition.wakeOne();
}

// Consumer
{
    QMutexLocker locker(&mutex);
    while (!dataReady) {
        condition.wait(&mutex);
    }
    // Process data
}
```

### QAtomicInt / QAtomicPointer

Lock-free atomic operations:

```cpp
QAtomicInt counter;

// Atomic operations
counter.fetchAndAddOrdered(1);
counter.loadAcquire();
counter.storeRelease(42);
```

### QtConcurrent High-Level Synchronization

```cpp
// QtConcurrent handles synchronization automatically
QFuture<int> future = QtConcurrent::mappedReduced(
    largeList,
    mapFunction,
    reduceFunction
);
// No need for manual mutex/semaphore management
```

## Best Practices

### General

1. **Prefer high-level APIs**: Use `QtConcurrent` or `WorkerScript` when possible instead of raw `QThread`
2. **Use the worker-object pattern**: Move worker objects to threads with `moveToThread()` rather than subclassing `QThread`
3. **Avoid shared state**: Minimize shared data between threads; use signals/slots for communication
4. **Never access GUI from worker threads**: All GUI operations must happen in the main thread

### QThread Specific

1. **Don't subclass QThread for slot invocation**: The QThread instance lives in the old thread; use `moveToThread()` instead
2. **Always wait for threads**: Call `wait()` after `quit()` to ensure clean shutdown
3. **Use `requestInterruption()`**: For clean thread termination instead of `terminate()`

```cpp
void Worker::doWork() {
    while (!QThread::currentThread()->isInterruptionRequested()) {
        // Process work items
    }
}

// Request interruption
workerThread->requestInterruption();
workerThread->quit();
workerThread->wait();
```

4. **Connect `finished()` to `deleteLater()`**: For automatic cleanup

### Synchronization

1. **Minimize lock duration**: Hold mutexes for the shortest time possible
2. **Use `QReadWriteLock`**: When reads significantly outnumber writes
3. **Avoid deadlocks**: Always acquire locks in the same order
4. **Prefer `QMutexLocker`**: RAII pattern for safe mutex handling
5. **Use `QSemaphore` for producer-consumer**: When multiple threads share a limited resource pool

### Performance

1. **Profile before optimizing**: Not all operations benefit from threading
2. **Consider `QtConcurrent::mappedReduced`**: For data-parallel operations
3. **Use thread pools**: Reuse threads instead of creating/destroying
4. **Batch operations**: Group small tasks to reduce thread switching overhead
5. **Monitor with `QFutureWatcher`**: For progress reporting without blocking

## Common Patterns

### Producer-Consumer with QSemaphore

```cpp
const int BufferSize = 10;
char buffer[BufferSize];
QSemaphore freeSlots(BufferSize);
QSemaphore usedSlots(0);

class Producer : public QThread
{
    void run() override {
        for (int i = 0; i < 100; ++i) {
            freeSlots.acquire();
            buffer[i % BufferSize] = 'A' + (i % 26);
            usedSlots.release();
        }
    }
};

class Consumer : public QThread
{
    void run() override {
        for (int i = 0; i < 100; ++i) {
            usedSlots.acquire();
            qDebug() << buffer[i % BufferSize];
            freeSlots.release();
        }
    }
};
```

### Thread-Safe Singleton

```cpp
class Singleton
{
public:
    static Singleton *instance() {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        static Singleton *inst = nullptr;
        if (!inst) {
            inst = new Singleton();
        }
        return inst;
    }

private:
    Singleton() = default;
};
```

### Worker Object Pattern (Complete Example)

```cpp
// worker.h
class Worker : public QObject
{
    Q_OBJECT

public slots:
    void process(const QString &input) {
        // Simulate long processing
        QString result;
        for (int i = 0; i < input.size(); ++i) {
            result.prepend(input.at(i));
            QThread::msleep(10); // Simulate work
        }
        emit finished(result);
    }

signals:
    void finished(const QString &result);
};

// mainwindow.h
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow() {
        auto *btn = new QPushButton("Start");
        connect(btn, &QPushButton::clicked, this, &MainWindow::startWork);
        setCentralWidget(btn);
    }

    ~MainWindow() {
        m_thread.quit();
        m_thread.wait();
    }

public slots:
    void startWork() {
        auto *worker = new Worker;
        worker->moveToThread(&m_thread);

        connect(&m_thread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &MainWindow::operate, worker, &Worker::process);
        connect(worker, &Worker::finished, this, &MainWindow::handleResult);

        m_thread.start();
        emit operate("Hello World");
    }

    void handleResult(const QString &result) {
        qDebug() << "Result:" << result;
    }

signals:
    void operate(const QString &);

private:
    QThread m_thread;
};
```

## See Also

- QThread
- QThreadPool
- QRunnable
- QtConcurrent
- QMutex
- QReadWriteLock
- QSemaphore
- QWaitCondition
- Synchronizing Threads
- Mandelbrot Example
- Producer and Consumer using Semaphores
- Producer and Consumer using Wait Conditions
