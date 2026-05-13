# Qt 6 自定义控件开发

> 来源: https://doc.qt.io/qt-6/qpainter.html, https://doc.qt.io/qt-6/qwidget.html, https://doc.qt.io/qt-6/signalsandslots.html
> 获取日期: 2026-05-12

---

## 1. 自定义控件基础

### 1.1 创建自定义 Widget

```cpp
#ifndef CRYPTOWIDGET_H
#define CRYPTOWIDGET_H

#include <QWidget>

class CryptoWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QColor valueColor READ valueColor WRITE setValueColor)

public:
    explicit CryptoWidget(QWidget *parent = nullptr);

    double value() const { return m_value; }
    void setValue(double val);
    
    QColor valueColor() const { return m_valueColor; }
    void setValueColor(const QColor &color) { m_valueColor = color; update(); }

signals:
    void valueChanged(double newValue);
    void clicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

private:
    double m_value = 0.0;
    QColor m_valueColor = Qt::green;
    bool m_hovered = false;
};

#endif
```

### 1.2 Q_OBJECT 宏的作用

```cpp
class MyWidget : public QWidget
{
    Q_OBJECT  // 必须! 启用信号/槽、元对象系统、动态属性

public:
    // ...
};
```

Q_OBJECT 宏提供的功能:
- 信号和槽机制
- 元对象系统 (introspection)
- 动态属性
- `qobject_cast<>()` 安全类型转换
- `tr()` 国际化支持

---

## 2. QPainter 绘制

### 2.1 基本绘制模式

```cpp
void CryptoWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    
    // 启用抗锯齿
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 绘制背景
    painter.fillRect(rect(), m_hovered ? QColor(40, 40, 40) : QColor(30, 30, 30));
    
    // 绘制边框
    painter.setPen(QPen(QColor(80, 80, 80), 1));
    painter.drawRect(rect().adjusted(1, 1, -1, -1));
    
    // 绘制文字
    painter.setPen(m_valueColor);
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(rect(), Qt::AlignCenter, QString("$%1").arg(m_value, 0, 'f', 2));
}
```

### 2.2 坐标变换

```cpp
void CryptoWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 缩放到 widget 尺寸
    painter.scale(width() / 200.0, height() / 200.0);
    
    // 平移到中心
    painter.translate(100.0, 100.0);
    
    // 旋转
    painter.rotate(-45.0);
    
    // 绘制形状
    painter.setPen(QPen(Qt::blue, 2));
    painter.setBrush(QColor(100, 150, 255, 128));
    painter.drawEllipse(-50, -50, 100, 100);
}
```

### 2.3 渐变与画刷

```cpp
void CryptoWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 线性渐变
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0.0, QColor(0, 100, 200));
    gradient.setColorAt(1.0, QColor(0, 50, 100));
    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 8, 8);
    
    // 径向渐变
    QRadialGradient radial(width()/2, height()/2, qMin(width(), height())/2);
    radial.setColorAt(0, QColor(255, 255, 255, 200));
    radial.setColorAt(1, QColor(255, 255, 255, 0));
    painter.setBrush(radial);
    painter.drawRect(rect());
}
```

### 2.4 QPainterPath

```cpp
void CryptoWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    QPainterPath path;
    path.moveTo(10, 80);
    path.lineTo(30, 10);
    path.cubicTo(80, 10, 60, 60, 90, 80);  // 贝塞尔曲线
    path.lineTo(10, 80);
    
    // 填充
    painter.setBrush(QColor(100, 200, 100, 128));
    painter.setPen(QPen(Qt::darkGreen, 2));
    painter.drawPath(path);
}
```

### 2.5 绘制优化

```cpp
void CryptoWidget::paintEvent(QPaintEvent *event)
{
    // 只重绘受影响的区域
    QRect dirtyRegion = event->region().boundingRect();
    
    QPainter painter(this);
    painter.setClipRect(dirtyRegion);  // 裁剪到脏区域
    // ... 只绘制需要的部分
}

// 使用 update() 而非 repaint()
void CryptoWidget::updateValue(double newVal)
{
    m_value = newVal;
    update();  // 推荐: 允许合并多个更新请求
    // repaint();  // 避免使用: 强制立即重绘
}
```

---

## 3. 事件处理

### 3.1 鼠标事件

```cpp
void CryptoWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked();
        update();
    }
    QWidget::mousePressEvent(event);  // 调用基类
}

void CryptoWidget::enterEvent(QEnterEvent *event)
{
    m_hovered = true;
    setCursor(Qt::PointingHandCursor);
    update();
    QWidget::enterEvent(event);
}

void CryptoWidget::leaveEvent(QEvent *event)
{
    m_hovered = false;
    unsetCursor();
    update();
    QWidget::leaveEvent(event);
}
```

### 3.2 键盘事件

```cpp
void CryptoWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        setValue(m_value + 0.1);
        break;
    case Qt::Key_Down:
        setValue(m_value - 0.1);
        break;
    case Qt::Key_Space:
        emit clicked();
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}
```

### 3.3 事件过滤器

```cpp
// 在父对象中拦截子对象事件
bool ParentWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_childWidget && event->type() == QEvent::MouseButtonPress) {
        // 处理子对象的鼠标点击
        return true;  // 事件已处理，不再传递
    }
    return QWidget::eventFilter(watched, event);
}

// 安装过滤器
m_childWidget->installEventFilter(this);
```

### 3.4 大小策略

```cpp
QSize CryptoWidget::sizeHint() const
{
    return QSize(200, 100);  // 理想大小
}

QSize CryptoWidget::minimumSizeHint() const
{
    return QSize(100, 50);  // 最小可接受大小
}
```

---

## 4. Q_PROPERTY 系统

### 4.1 声明属性

```cpp
class CryptoWidget : public QWidget
{
    Q_OBJECT
    
    // 属性声明: 类型 名称 READ 读函数 [WRITE 写函数] [NOTIFY 变化信号]
    Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(bool animated READ isAnimated WRITE setAnimated)
    Q_PROPERTY(QString format READ format WRITE setFormat)
    Q_PROPERTY(int decimalPlaces READ decimalPlaces WRITE setDecimalPlaces)

public:
    // ...
};
```

### 4.2 动态属性

```cpp
// 运行时添加属性 (不需要在类中声明)
widget->setProperty("customRole", "danger");
widget->setProperty("animationEnabled", true);

// 读取动态属性
QVariant val = widget->property("customRole");

// 使用动态属性选择器 (QSS)
// widget[customRole="danger"] { background-color: red; }
```

---

## 5. 信号与槽

### 5.1 声明与连接

```cpp
class CryptoWidget : public QWidget
{
    Q_OBJECT

signals:
    void valueChanged(double newValue);
    void clicked();
    void customContextMenuRequested(const QPoint &pos);

public slots:
    void setValue(double val);
    void reset();

public:
    // ...
};
```

### 5.2 连接方式

```cpp
// 方式 1: 函数指针 (推荐，编译时类型检查)
connect(m_button, &QPushButton::clicked, this, &MyClass::onButtonClicked);

// 方式 2: Lambda
connect(m_button, &QPushButton::clicked, [=]() {
    qDebug() << "Button clicked!";
});

// 方式 3: 新语法 + 上下文对象 (对象销毁时自动断开)
connect(m_button, &QPushButton::clicked, this, [this]() {
    m_cryptoWidget->setValue(0);
});

// 断开连接
disconnect(m_connection);
```

### 5.3 自定义信号发射

```cpp
void CryptoWidget::setValue(double val)
{
    if (qFuzzyCompare(m_value, val))
        return;
    
    m_value = val;
    update();  // 触发重绘
    emit valueChanged(m_value);  // 通知外部
}
```

---

## 6. 自定义复合控件

### 6.1 组合已有控件

```cpp
class CryptoTicker : public QWidget
{
    Q_OBJECT

public:
    explicit CryptoTicker(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        auto layout = new QHBoxLayout(this);
        
        m_iconLabel = new QLabel();
        m_nameLabel = new QLabel();
        m_priceLabel = new QLabel();
        m_changeLabel = new QLabel();
        
        layout->addWidget(m_iconLabel);
        layout->addWidget(m_nameLabel);
        layout->addStretch();
        layout->addWidget(m_priceLabel);
        layout->addWidget(m_changeLabel);
    }

    void setData(const QString &name, double price, double change)
    {
        m_nameLabel->setText(name);
        m_priceLabel->setText(QString("$%1").arg(price, 0, 'f', 2));
        
        QColor changeColor = change >= 0 ? Qt::green : Qt::red;
        m_changeLabel->setText(QString("%1%2%")
            .arg(change >= 0 ? "+" : "")
            .arg(change, 0, 'f', 2));
        m_changeLabel->setStyleSheet(
            QString("color: %1; font-weight: bold;").arg(changeColor.name()));
    }

private:
    QLabel *m_iconLabel;
    QLabel *m_nameLabel;
    QLabel *m_priceLabel;
    QLabel *m_changeLabel;
};
```

---

## 7. 动画效果

### 7.1 QPropertyAnimation

```cpp
// 为自定义属性添加动画
class AnimatedWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)

public:
    qreal opacity() const { return m_opacity; }
    void setOpacity(qreal op) { m_opacity = op; update(); }

private:
    qreal m_opacity = 1.0;
};

// 使用动画
auto anim = new QPropertyAnimation(widget, "opacity");
anim->setDuration(300);
anim->setStartValue(0.0);
anim->setEndValue(1.0);
anim->setEasingCurve(QEasingCurve::OutCubic);
anim->start(QAbstractAnimation::DeleteWhenStopped);
```

---

## 8. 发布自定义控件

### 8.1 Qt Designer 插件

```cpp
// mywidgetplugin.h
#include <QDesignerCustomWidgetInterface>

class CryptoWidgetPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface")

public:
    QString name() const override { return "CryptoWidget"; }
    QString group() const override { return "Crypto Widgets"; }
    QString toolTip() const override { return "Cryptocurrency display widget"; }
    QString whatsThis() const override { return ""; }
    QString includeFile() const override { return "cryptowidget.h"; }
    QIcon icon() const override { return QIcon(); }
    QWidget *createWidget(QWidget *parent) override { return new CryptoWidget(parent); }
    bool isContainer() const override { return false; }
};
```

---

## 参考资源

- [QPainter](https://doc.qt.io/qt-6/qpainter.html)
- [QWidget](https://doc.qt.io/qt-6/qwidget.html)
- [Signals & Slots](https://doc.qt.io/qt-6/signalsandslots.html)
- [Q_PROPERTY](https://doc.qt.io/qt-6/properties.html)
- [QPropertyAnimation](https://doc.qt.io/qt-6/qpropertyanimation.html)
- [Qt Designer Plugin](https://doc.qt.io/qt-6/qdesignercustomwidgetinterface.html)
