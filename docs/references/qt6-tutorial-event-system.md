# Qt 6 Event System

> Source: https://doc.qt.io/qt-6/events.html
> Fetched: 2026-05-12
> Additional: https://doc.qt.io/qt-6/qobject.html#event

---

## Overview

Qt's event system is the foundation of all user interaction and internal communication within a Qt application. Events are objects that represent something that happened inside or outside the application. Qt receives events from the windowing system and dispatches them to the appropriate `QObject` subclass via `QObject::event()`.

## Event Loop

Qt uses an event-driven architecture. The `QApplication::exec()` (or `QCoreApplication::exec()` for non-GUI apps) starts the event loop that receives and dispatches events.

```cpp
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // ... setup ...
    return app.exec(); // Starts the event loop
}
```

The event loop:
1. Waits for events from the windowing system
2. Dispatches events to the appropriate objects
3. Calls event handlers (`event()`, `mousePressEvent()`, etc.)
4. Processes queued signals/slots (queued connections)
5. Repeats until `exit()` is called

## Event Classes

All event classes in Qt inherit from `QEvent`. The following table lists the most important event types:

| Event Class | Event Type | Description |
|-------------|-----------|-------------|
| `QMouseEvent` | `QEvent::MouseButtonPress/Release/Move/DblClick` | Mouse button clicks, movement, double-clicks |
| `QKeyEvent` | `QEvent::KeyPress/KeyRelease` | Keyboard input |
| `QWheelEvent` | `QEvent::Wheel` | Mouse wheel scrolling |
| `QResizeEvent` | `QEvent::Resize` | Widget resizing |
| `QMoveEvent` | `QEvent::Move` | Widget moving |
| `QPaintEvent` | `QEvent::Paint` | Widget painting |
| `QCloseEvent` | `QEvent::Close` | Window close requests |
| `QFocusEvent` | `QEvent::FocusIn/FocusOut` | Widget focus changes |
| `QTimerEvent` | `QEvent::Timer` | Timer-based events |
| `QShowEvent` | `QEvent::Show` | Widget shown |
| `QHideEvent` | `QEvent::Hide` | Widget hidden |
| `QEnterEvent` | `QEvent::Enter` | Mouse enters widget |
| `QHoverEvent` | `QEvent::HoverEnter/HoverLeave/HoverMove` | Hover events |
| `QDragEnterEvent` | `QEvent::DragEnter` | Drag enters widget |
| `QDragLeaveEvent` | `QEvent::DragLeave` | Drag leaves widget |
| `QDragMoveEvent` | `QEvent::DragMove` | Drag in progress |
| `QDropEvent` | `QEvent::Drop` | Drop completed |
| `QTouchEvent` | `QEvent::TouchBegin/Update/End` | Touch events |
| `QGestureEvent` | `QEvent::Gesture` | Gesture events |
| `QContextMenuEvent` | `QEvent::ContextMenu` | Context menu request |
| `QHelpEvent` | `QEvent::Help` | "What's This?" help request |
| `QTabletEvent` | `QEvent::TabletPress/Move/Release` | Tablet (pen) input |
| `QNativeGestureEvent` | `QEvent::NativeGesture` | Native gesture events |
| `QWindowStateChangeEvent` | `QEvent::WindowStateChange` | Window state changed |
| `QChildEvent` | `QEvent::ChildAdded/ChildRemoved` | Child object events |
| `QFileOpenEvent` | `QEvent::FileOpen` | File open request |

### All Event Classes (Complete List)

- `QAbstractEventDispatcher` - Interface to manage Qt's event queue
- `QActionEvent` - Generated when a QAction is added, removed, or changed
- `QBasicTimer` - Timer events for objects
- `QChildEvent` - Contains event parameters for child object events
- `QChildWindowEvent` - Contains event parameters for child window changes
- `QChronoTimer` - Repetitive and single-shot timers (Qt 6)
- `QCloseEvent` - Contains parameters that describe a close event
- `QContextMenuEvent` - Contains parameters that describe a context menu event
- `QDragEnterEvent` - Sent to a widget when a drag action enters it
- `QDragLeaveEvent` - Sent to a widget when a drag action leaves it
- `QDragMoveEvent` - Sent while a drag action is in progress
- `QDropEvent` - Sent when a drag action is completed
- `QDynamicPropertyChangeEvent` - Contains event parameters for dynamic property change events
- `QEnterEvent` - Contains parameters that describe an enter event
- `QEvent` - The base class of all event classes
- `QExposeEvent` - Contains event parameters for expose events
- `QFileOpenEvent` - Sent when there is a request to open a file or URL
- `QFocusEvent` - Contains event parameters for widget focus events
- `QGestureEvent` - Description of triggered gestures
- `QHelpEvent` - Used to request helpful information about a particular point
- `QHideEvent` - Sent after a widget is hidden
- `QHoverEvent` - Contains parameters that describe a mouse event
- `QIconDragEvent` - Indicates that a main icon drag has begun
- `QInputEvent` - Base class for events that describe user input
- `QInputMethodEvent` - Parameters for input method events
- `QKeyEvent` - Describes a key event
- `QMouseEvent` - Contains parameters that describe a mouse event
- `QMoveEvent` - Contains event parameters for move events
- `QNativeGestureEvent` - Contains parameters that describe a gesture event
- `QPaintEvent` - Contains event parameters for paint events
- `QPlatformSurfaceEvent` - Used to notify about native platform surface events
- `QPointingDevice` - Describes a device from which events originate
- `QPointingDeviceUniqueId` - Identifies a unique object used with a pointing device
- `QResizeEvent` - Contains event parameters for resize events
- `QScrollEvent` - Sent when scrolling
- `QScrollPrepareEvent` - Sent in preparation of scrolling
- `QShortcut` - Used to create keyboard shortcuts
- `QShortcutEvent` - Generated when user presses a key combination
- `QShowEvent` - Sent when a widget is shown
- `QStatusTipEvent` - Used to show messages in a status bar
- `QTabletEvent` - Contains parameters that describe a Tablet event
- `QTimer` - Repetitive and single-shot timers
- `QTimerEvent` - Contains parameters that describe a timer event
- `QTouchEvent` - Contains parameters that describe a touch event
- `QWhatsThisClickedEvent` - Handles hyperlinks in "What's This?" text
- `QWheelEvent` - Contains parameters that describe a wheel event
- `QWindowStateChangeEvent` - The window state before a change

## Event Handling Methods

### The `event()` Function

The `QObject::event()` function is the central event dispatcher. You can override it to handle events:

```cpp
bool MyClass::event(QEvent *event) override
{
    switch (event->type()) {
    case QEvent::KeyPress: {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Tab) {
            // Handle tab key
            return true; // Event handled
        }
        break;
    }
    case QEvent::MouseButtonPress: {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        // Handle mouse press
        break;
    }
    default:
        break;
    }
    return QWidget::event(event); // Pass to base class
}
```

### Specific Event Handlers

Qt provides specific event handler functions for common events:

```cpp
class MyWidget : public QWidget
{
protected:
    // Mouse events
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    // Keyboard events
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    // Paint
    void paintEvent(QPaintEvent *event) override;

    // Resize
    void resizeEvent(QResizeEvent *event) override;

    // Close
    void closeEvent(QCloseEvent *event) override;

    // Focus
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

    // Wheel
    void wheelEvent(QWheelEvent *event) override;
};
```

### Example: Custom Mouse Handling

```cpp
void MyWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_lastPos = event->position().toPoint();
        event->accept();
    } else {
        QWidget::mousePressEvent(event);
    }
}

void MyWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        QPoint delta = event->position().toPoint() - m_lastPos;
        move(pos() + delta);
        m_lastPos = event->position().toPoint();
        event->accept();
    } else {
        QWidget::mouseMoveEvent(event);
    }
}
```

### Example: Custom Keyboard Handling

```cpp
void MyWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
        move(pos() + QPoint(-10, 0));
        break;
    case Qt::Key_Right:
        move(pos() + QPoint(10, 0));
        break;
    case Qt::Key_Up:
        move(pos() + QPoint(0, -10));
        break;
    case Qt::Key_Down:
        move(pos() + QPoint(0, 10));
        break;
    case Qt::Key_Escape:
        close();
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}
```

## Event Filters

Event filters allow you to intercept events sent to another object before it processes them. This is useful for:

- Intercepting events before the target object handles them
- Monitoring events in child widgets
- Implementing global keyboard shortcuts

```cpp
class FilterObject : public QObject
{
    Q_OBJECT

protected:
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Tab) {
                qDebug() << "Tab key intercepted in" << obj;
                return true; // Event filtered - not passed to object
            }
        }
        return QObject::eventFilter(obj, event); // Pass to base class
    }
};
```

### Installing an Event Filter

```cpp
// Install filter on a widget
FilterObject *filter = new FilterObject(this);
targetWidget->installEventFilter(filter);

// Install filter on the application (all events)
qApp->installEventFilter(filter);
```

### Example: Intercepting Mouse Events on Child Widgets

```cpp
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow() {
        // Install filter on the central widget to intercept all child events
        installEventFilter(this);
    }

protected:
    bool eventFilter(QObject *obj, QEvent *event) override {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            qDebug() << "Mouse press on" << obj->metaObject()->className()
                     << "at" << mouseEvent->position();
        }
        return QMainWindow::eventFilter(obj, event);
    }
};
```

## Events vs Signals and Slots

| Feature | Events | Signals & Slots |
|---------|--------|-----------------|
| Level | Low-level | High-level |
| Direction | One-way (event -> handler) | Many-to-many |
| Scope | Single object | Cross-object |
| Thread | Same thread (unless queued) | Any thread (with queued connections) |
| Use case | System notifications, widget behavior | Component communication |
| Custom | Override `event()` or specific handlers | Define new signals/slots |

**Key difference**: Events are low-level notifications from the windowing system or Qt itself. They are handled within the object that receives them. Signals and slots are a high-level communication mechanism between objects.

## Custom Events

You can create custom events by subclassing `QEvent`:

```cpp
// Define custom event type
class DownloadEvent : public QEvent
{
public:
    static const QEvent::Type EventType =
        static_cast<QEvent::Type>(QEvent::User + 1);

    DownloadEvent(const QString &url, int progress)
        : QEvent(EventType), m_url(url), m_progress(progress) {}

    QString url() const { return m_url; }
    int progress() const { return m_progress; }

private:
    QString m_url;
    int m_progress;
};

// Send custom event
void Downloader::downloadProgress(const QString &url, int progress)
{
    DownloadEvent *event = new DownloadEvent(url, progress);
    QApplication::postEvent(targetObject, event);
}

// Handle custom event
bool DownloadWidget::event(QEvent *event)
{
    if (event->type() == DownloadEvent::EventType) {
        DownloadEvent *de = static_cast<DownloadEvent*>(event);
        qDebug() << "Download" << de->url() << "progress:" << de->progress();
        return true;
    }
    return QWidget::event(event);
}
```

### Event Posting

```cpp
// Post event (asynchronous - event is queued)
QApplication::postEvent(receiver, event);

// Send event (synchronous - event is dispatched immediately)
QApplication::sendEvent(receiver, event);
```

**Important**: When posting events, the event loop takes ownership of the event object. When sending events, the caller is responsible for the event object's lifetime.

## Timer Events

### Using QTimerEvent

```cpp
class TimerWidget : public QWidget
{
    Q_OBJECT

public:
    TimerWidget(QWidget *parent = nullptr) : QWidget(parent) {
        startTimer(1000); // 1 second interval
    }

protected:
    void timerEvent(QTimerEvent *event) override {
        m_counter++;
        update(); // Triggers paintEvent
    }

private:
    int m_counter = 0;
};
```

### Using QTimer (Preferred)

```cpp
QTimer *timer = new QTimer(this);
connect(timer, &QTimer::timeout, this, &MyWidget::onTimerTimeout);
timer->start(1000); // 1 second interval

// One-shot timer
QTimer::singleShot(500, this, &MyWidget::doSomething);
```

## Event Processing Priority

```cpp
// Events are processed in this order:
// 1. Event filters installed on the object
// 2. The object's event() function
// 3. Specific event handlers (mousePressEvent, etc.)
// 4. Base class event() function
```

## Best Practices

1. **Accept or ignore events**: Call `event->accept()` when you handle an event, or `event->ignore()` to pass it up the chain
2. **Use specific handlers**: Prefer `mousePressEvent()` over overriding `event()` for mouse events
3. **Event filters for cross-cutting concerns**: Use event filters when you need to intercept events in multiple widgets
4. **Don't block the event loop**: Long operations should use timers or background threads
5. **Use QTimer for periodic tasks**: Instead of raw timer events
6. **Post vs Send**: Use `postEvent()` for async processing, `sendEvent()` for synchronous handling
7. **Custom events**: Use `QEvent::User + N` for custom event types to avoid conflicts

## See Also

- QObject::event()
- QEvent
- Events and Event Filters
- QAbstractEventDispatcher
