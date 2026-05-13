# Qt 6 样式表 (QSS) 与主题定制

> 来源: https://doc.qt.io/qt-6/stylesheet-syntax.html, https://doc.qt.io/qt-6/stylesheet-reference.html, https://doc.qt.io/qt-6/stylesheet-examples.html
> 获取日期: 2026-05-12

---

## 1. QSS 基础

Qt Style Sheets (QSS) 是类 CSS 的样式机制，用于自定义 Qt Widget 的外观。

### 1.1 应用样式表

```cpp
// 方式 1: 应用到整个应用
QFile styleFile(":/styles/dark.qss");
styleFile.open(QFile::ReadOnly | QFile::Text);
QString styleSheet = QLatin1String(styleFile.readAll());
qApp->setStyleSheet(styleSheet);

// 方式 2: 应用到单个控件
button->setStyleSheet("background-color: blue; color: white;");

// 方式 3: 通过 CMake 资源系统
// 在 .qrc 文件中添加 .qss 文件
```

### 1.2 基本语法

```css
/* 样式规则: 选择器 { 属性: 值; } */
QPushButton {
    background-color: #3c3f41;
    color: #f0f0f0;
    border: 1px solid #555;
    border-radius: 4px;
    padding: 6px 16px;
}

/* 多选择器 */
QPushButton, QToolButton {
    background-color: #3c3f41;
}

/* 伪状态 */
QPushButton:hover {
    background-color: #4a6fa5;
}

QPushButton:pressed {
    background-color: #3a5f95;
}

QPushButton:disabled {
    background-color: #2b2b2b;
    color: #666;
}
```

---

## 2. 选择器类型

### 2.1 通用选择器

```css
/* 匹配所有控件 */
* {
    font-family: "Segoe UI", "Microsoft YaHei";
    font-size: 14px;
}
```

### 2.2 类型选择器

```css
/* 匹配 QPushButton 及其所有子类 */
QPushButton {
    background-color: #4a6fa5;
}

/* 不匹配子类 - 使用 . 类选择器 */
.QPushButton {
    background-color: #4a6fa5;  /* 仅精确匹配 QPushButton */
}
```

### 2.3 ID 选择器

```css
/* 匹配 objectName 为 "okButton" 的控件 */
QPushButton#okButton {
    background-color: green;
    color: white;
}

/* 设置 objectName */
button->setObjectName("okButton");
```

### 2.4 属性选择器

```css
/* 匹配 flat 属性为 "true" 的按钮 */
QPushButton[flat="true"] {
    border: none;
}

/* 匹配自定义动态属性 */
QPushButton[dangerous="true"] {
    background-color: #d32f2f;
    color: white;
}

/* 使用 ~= 匹配 QStringList */
QCheckBox[features~="bold"] {
    font-weight: bold;
}
```

```cpp
// 设置动态属性供 QSS 使用
button->setProperty("dangerous", true);
button->style()->unpolish(button);   // 强制重新计算样式
button->style()->polish(button);
button->update();
```

### 2.5 后代选择器

```css
/* 匹配 QDialog 内部所有 QPushButton (任意深度) */
QDialog QPushButton {
    background-color: white;
}
```

### 2.6 子选择器

```css
/* 仅匹配 QDialog 的直接子 QPushButton */
QDialog > QPushButton {
    background-color: white;
}
```

### 2.7 选择器组合

```css
/* 逗号分隔 = 或 */
QPushButton, QComboBox {
    background-color: #3c3f41;
}

/* 组合: 满足所有条件 */
QPushButton[flat="true"]:hover {
    background-color: #4a6fa5;
}
```

---

## 3. 伪状态

| 伪状态 | 说明 | 适用控件 |
|--------|------|----------|
| `:hover` | 鼠标悬停 | 所有控件 |
| `:pressed` | 按下状态 | QPushButton, QToolButton |
| `:checked` | 选中状态 | QCheckBox, QRadioButton, QToolButton |
| `:unchecked` | 未选中状态 | QCheckBox, QRadioButton |
| `:disabled` | 禁用状态 | 所有控件 |
| `:enabled` | 启用状态 | 所有控件 |
| `:focus` | 获得焦点 | 所有控件 |
| `:editable` | 可编辑 | QComboBox, QLineEdit |
| `:on` | 打开状态 | QComboBox, QMenu |
| `:off` | 关闭状态 | QComboBox, QMenu |

```css
/* 链式伪状态 */
QPushButton:enabled:hover {
    background-color: #5a7fb5;
}

QPushButton:enabled:pressed {
    background-color: #3a5f95;
}

/* 否定伪状态 */
QPushButton:!flat {
    border: 1px solid #8f8f91;
}

/* 选中且悬停 */
QCheckBox:checked:hover {
    color: green;
}
```

---

## 4. 子控件 (Sub-Controls)

### 4.1 常用子控件

```css
/* QComboBox 下拉按钮 */
QComboBox::drop-down {
    subcontrol-origin: padding;
    subcontrol-position: top right;
    width: 15px;
    border-left: 1px solid darkgray;
}

/* QComboBox 下拉箭头 */
QComboBox::down-arrow {
    image: url(:/icons/down-arrow.png);
    width: 12px;
    height: 12px;
}

/* QScrollBar 滑块 */
QScrollBar::handle:vertical {
    background: #5a5a5a;
    min-height: 20px;
    border-radius: 4px;
}

/* QScrollBar 轨道 */
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0px;  /* 隐藏上下箭头 */
}

/* QProgressBar 块 */
QProgressBar::chunk {
    background-color: #4a6fa5;
    border-radius: 3px;
}

/* QTabWidget 标签页 */
QTabBar::tab:selected {
    background-color: #4a6fa5;
    color: white;
}

/* QMenu 选中项 */
QMenu::item:selected {
    background-color: #4a6fa5;
}
```

---

## 5. 盒模型

### 5.1 边框宽度

```css
/* 四边相同 */
QLabel { border-width: 1px; }

/* 上下 1px, 左右 2px */
QLabel { border-width: 1px 2px; }

/* 上 1px, 左右 2px, 下 3px */
QLabel { border-width: 1px 2px 3px; }

/* 上 右 下 左 */
QLabel { border-width: 1px 2px 3px 4px; }
```

### 5.2 内边距与外边距

```css
QPushButton {
    padding: 6px 16px;           /* 上下 6px, 左右 16px */
    margin: 4px;                  /* 外边距 */
}

QLineEdit {
    padding: 2px 8px;             /* 内边距 */
}
```

---

## 6. 背景与边框

### 6.1 渐变背景

```css
QPushButton {
    /* 线性渐变 */
    background-color: qlineargradient(
        x1:0, y1:0, x2:0, y2:1,
        stop:0 #f6f7fa,
        stop:1 #dadbde
    );
}

QProgressBar {
    /* 径向渐变 */
    background-color: qradialgradient(
        cx:0.5, cy:0.5, radius:0.5,
        fx:0.5, fy:0.3,
        stop:0 #fff,
        stop:1 #ddd
    );
}
```

### 6.2 边框样式

```css
QPushButton {
    border: 2px solid #8f8f91;       /* 宽度 样式 颜色 */
    border-radius: 6px;               /* 圆角 */
    border-top-left-radius: 0px;      /* 单独设置某个角 */
}
```

---

## 7. 实战: 深色主题

```css
/* ========== 全局 ========== */
QWidget {
    background-color: #2b2b2b;
    color: #f0f0f0;
    font-family: "Segoe UI", "Microsoft YaHei", sans-serif;
    font-size: 13px;
}

/* ========== 按钮 ========== */
QPushButton {
    background-color: #3c3f41;
    border: 1px solid #555;
    border-radius: 4px;
    padding: 6px 16px;
    min-width: 80px;
}

QPushButton:hover {
    background-color: #4a6fa5;
    border-color: #5a8fc5;
}

QPushButton:pressed {
    background-color: #3a5f95;
}

QPushButton:disabled {
    background-color: #2b2b2b;
    color: #666;
    border-color: #444;
}

/* ========== 输入框 ========== */
QLineEdit, QTextEdit, QSpinBox {
    background-color: #1e1e1e;
    border: 1px solid #555;
    border-radius: 3px;
    padding: 4px 8px;
    color: #f0f0f0;
    selection-background-color: #4a6fa5;
}

QLineEdit:focus, QTextEdit:focus {
    border-color: #4a6fa5;
}

/* ========== 表格 ========== */
QTableView {
    background-color: #1e1e1e;
    alternate-background-color: #252525;
    gridline-color: #3c3f41;
    border: 1px solid #555;
    selection-background-color: #4a6fa5;
    selection-color: white;
}

QHeaderView::section {
    background-color: #3c3f41;
    border: 1px solid #555;
    padding: 4px 8px;
    font-weight: bold;
}

/* ========== 滚动条 ========== */
QScrollBar:vertical {
    background: #2b2b2b;
    width: 12px;
    border-radius: 6px;
}

QScrollBar::handle:vertical {
    background: #5a5a5a;
    min-height: 20px;
    border-radius: 6px;
}

QScrollBar::handle:vertical:hover {
    background: #7a7a7a;
}

QScrollBar::add-line:vertical,
QScrollBar::sub-line:vertical {
    height: 0px;
}

/* ========== 标签页 ========== */
QTabWidget::pane {
    border: 1px solid #555;
}

QTabBar::tab {
    background-color: #3c3f41;
    border: 1px solid #555;
    padding: 6px 16px;
    border-top-left-radius: 4px;
    border-top-right-radius: 4px;
}

QTabBar::tab:selected {
    background-color: #4a6fa5;
    color: white;
}

/* ========== 工具提示 ========== */
QToolTip {
    background-color: #3c3f41;
    border: 1px solid #555;
    color: #f0f0f0;
    padding: 4px;
}
```

---

## 8. 动态主题切换

```cpp
class ThemeManager : public QObject {
    Q_OBJECT
public:
    enum Theme { Light, Dark };
    
    static ThemeManager* instance() {
        static ThemeManager mgr;
        return &mgr;
    }
    
    void setTheme(Theme theme) {
        QFile file(theme == Dark 
            ? ":/styles/dark.qss" 
            : ":/styles/light.qss");
        file.open(QFile::ReadOnly | QFile::Text);
        qApp->setStyleSheet(QLatin1String(file.readAll()));
        
        emit themeChanged(theme);
    }

signals:
    void themeChanged(Theme theme);
};

// 使用
ThemeManager::instance()->setTheme(ThemeManager::Dark);
```

---

## 9. 性能注意事项

1. **避免过度使用 QSS**: 复杂选择器会影响渲染性能
2. **使用 objectName 而非复杂选择器**: ID 选择器最快
3. **属性变更后刷新**: 修改动态属性后调用 `unpolish()` + `polish()` + `update()`
4. **考虑使用 QPalette**: 对于简单主题，QPalette 性能优于 QSS
5. **资源文件**: 将 QSS 打包到 `.qrc` 中，避免运行时文件 I/O

```cpp
// 安全地更新样式
widget->setProperty("myProp", newValue);
widget->style()->unpolish(widget);
widget->style()->polish(widget);
widget->update();
```

---

## 参考资源

- [Qt Style Sheets Reference](https://doc.qt.io/qt-6/stylesheet-reference.html)
- [Qt Style Sheets Syntax](https://doc.qt.io/qt-6/stylesheet-syntax.html)
- [Qt Style Sheets Examples](https://doc.qt.io/qt-6/stylesheet-examples.html)
- [QDarkStyleSheet](https://github.com/ColinDuquesnoy/QDarkStyleSheet)
