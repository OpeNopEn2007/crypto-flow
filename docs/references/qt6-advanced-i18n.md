# Qt 6 国际化 (i18n) 与本地化

> 来源: https://doc.qt.io/qt-6/i18n-source-translation.html, https://doc.qt.io/qt-6/qtlinguist-trollprint-example.html
> 获取日期: 2026-05-12

---

## 1. 国际化工作流程概览

```
源代码 -> tr() 标记 -> lupdate 提取 -> Qt Linguist 翻译 -> lrelease 编译 -> 运行时加载
```

步骤:
1. **标记**: 用 `tr()` (C++) 或 `qsTr()` (QML) 标记所有用户可见字符串
2. **提取**: 运行 `lupdate` 从源代码提取可翻译字符串，生成 `.ts` 文件
3. **翻译**: 使用 Qt Linguist GUI 工具或直接编辑 `.ts` XML 文件
4. **编译**: 运行 `lrelease` 将 `.ts` 编译为 `.qm` 二进制文件
5. **加载**: 运行时通过 `QTranslator` 加载 `.qm` 文件

---

## 2. C++ 中标记可翻译字符串

### 2.1 基本 tr() 用法

```cpp
#include <QObject>
#include <QPushButton>

class LoginDialog : public QWidget
{
    Q_OBJECT

public:
    void setupUI()
    {
        auto label = new QLabel(tr("Username:"));
        auto passwordLabel = new QLabel(tr("Password:"));
        auto loginButton = new QPushButton(tr("Login"));
        auto cancelButton = new QPushButton(tr("Cancel"));
    }
};
```

### 2.2 消除歧义

当同一字符串在不同上下文有不同含义时:

```cpp
// "File" 作为菜单项 vs 文件类型
QString menuFile = tr("File", "Menu item");
QString fileType = tr("File", "Document type");

// 不加消歧参数，两者会合并为同一翻译
```

### 2.3 复数形式

```cpp
int count = 5;
QString message = tr("%n message(s) received", "", count);
// 英语: "5 messages received"
// 俄语: 根据俄语复数规则选择正确形式
```

### 2.4 全局函数中的翻译

```cpp
// 方式 1: 使用某个类的 tr()
void some_global_function(LoginWidget *logwid)
{
    QLabel *label = new QLabel(LoginWidget::tr("Password:"), logwid);
}

// 方式 2: 使用 QCoreApplication::translate()
void same_global_function(LoginWidget *logwid)
{
    QLabel *label = new QLabel(
        QCoreApplication::translate("LoginWidget", "Password:"), logwid);
}
```

### 2.5 动态字符串

```cpp
// QT_TR_NOOP: 标记但不立即翻译
static const char *colors[] = {
    QT_TR_NOOP("Red"),
    QT_TR_NOOP("Green"),
    QT_TR_NOOP("Blue")
};

// 使用时翻译
QString red = tr(colors[0]);

// QT_TRANSLATE_N_NOOP: 带上下文
static const char *items[] = {
    QT_TRANSLATE_N_NOOP("Colors", "Red"),
    QT_TRANSLATE_N_NOOP("Colors", "Green"),
};
```

---

## 3. QML 中的国际化

### 3.1 qsTr() 用法

```qml
import QtQuick

Item {
    Text {
        text: qsTr("Hello, World!")
    }
    
    Button {
        text: qsTr("Click me")
        onClicked: {
            label.text = qsTr("Button clicked %1 times").arg(count)
        }
    }
}
```

### 3.2 QML 中的复数

```qml
Text {
    // Qt 6 中使用 qsTrId
    text: qsTrId("n-messages").arg(count)
}
```

### 3.3 CMake 中配置 QML 翻译

```cmake
find_package(Qt6 REQUIRED COMPONENTS Core Linguist Qml Quick)

qt_standard_project_setup(REQUIRES 6.8
    I18N_TRANSLATED_LANGUAGES de ar ko zh ja fr it es pt en)
```

---

## 4. lupdate 提取字符串

### 4.1 命令行用法

```bash
# 从源代码提取翻译字符串
lupdate src/ -ts translations/app_zh_CN.ts

# 从多个目录提取
lupdate src/ ui/ -ts translations/app_zh_CN.ts translations/app_en.ts

# 更新现有翻译文件 (保留已有翻译)
lupdate -no-obsolete src/ -ts translations/app_zh_CN.ts
```

### 4.2 CMake 自动提取

```cmake
find_package(Qt6 REQUIRED COMPONENTS Linguist)

# 声明翻译文件
qt_add_lupdate(TARGET_NAME
    SOURCES
        src/main.cpp
        src/mainwindow.cpp
        ui/mainwindow.ui
    TS_FILES
        translations/app_zh_CN.ts
        translations/app_ja.ts
    LUPDATE_OPTIONS -no-obsolete
)
```

### 4.3 .ts 文件格式

```xml
<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN">
<context>
    <name>LoginDialog</name>
    <message>
        <location filename="../src/logindialog.cpp" line="42"/>
        <source>Username:</source>
        <translation>用户名：</translation>
    </message>
    <message>
        <location filename="../src/logindialog.cpp" line="43"/>
        <source>Password:</source>
        <translation>密码：</translation>
    </message>
</context>
</TS>
```

---

## 5. Qt Linguist 翻译

### 5.1 基本操作

1. 打开 `.ts` 文件
2. 逐条翻译，标记为:
   - **已翻译** (Translated)
   - **需要审阅** (Needs Review)
   - **未翻译** (Untranslated)
3. 保存

### 5.2 上下文与注释

在源代码中添加翻译注释:

```cpp
// qt_tr_noop: 为翻译者添加注释
// 上下文: 菜单
// 需要: 保持简洁
menu->addAction(tr("Open"));
```

### 5.3 使用 Qt Linguist 管理翻译

- **短语书**: 存储常用翻译，提高一致性
- **词汇表**: 标准化术语翻译
- **批量操作**: 一次翻译多个相同源文本

---

## 6. lrelease 编译翻译

```bash
# 编译 .ts 为 .qm
lrelease translations/app_zh_CN.ts

# 编译所有翻译文件
lrelease translations/*.ts
```

```cmake
# CMake 中自动编译
qt_add_lrelease(TARGET_NAME
    TS_FILES
        translations/app_zh_CN.ts
        translations/app_ja.ts
    QM_FILES_OUTPUT_VARIABLE QM_FILES
)
```

---

## 7. 运行时加载翻译

### 7.1 基本加载

```cpp
#include <QApplication>
#include <QTranslator>
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 根据系统语言自动加载
    QLocale locale;
    QTranslator translator;
    
    // 方式 1: 从资源系统加载
    if (translator.load(locale, "app", "_", ":/translations")) {
        app.installTranslator(&translator);
    }
    
    // 方式 2: 从文件系统加载
    if (translator.load("translations/app_zh_CN.qm")) {
        app.installTranslator(&translator);
    }
    
    MainWindow window;
    window.show();
    return app.exec();
}
```

### 7.2 动态切换语言

```cpp
class TranslationManager : public QObject {
    Q_OBJECT
public:
    void switchLanguage(const QLocale &locale) {
        // 移除旧翻译
        if (m_translator) {
            qApp->removeTranslator(m_translator);
            delete m_translator;
        }
        
        m_translator = new QTranslator(this);
        if (m_translator->load(locale, "app", "_", ":/translations")) {
            qApp->installTranslator(m_translator);
        }
        
        // 触发 UI 重新翻译
        emit languageChanged();
    }
    
signals:
    void languageChanged();

private:
    QTranslator *m_translator = nullptr;
};

// 在 UI 中监听语言变化
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow() {
        connect(TranslationManager::instance(), 
                &TranslationManager::languageChanged,
                this, &MainWindow::retranslateUI);
    }
    
private slots:
    void retranslateUI() {
        // 重新设置所有可翻译字符串
        m_fileMenu->setTitle(tr("&File"));
        m_exitAction->setText(tr("E&xit"));
        m_statusBar->showMessage(tr("Ready"));
    }
};
```

### 7.3 CMake 资源配置

```cmake
# 将 .qm 文件打包到资源系统
qt_add_resources(TARGET_NAME "translations"
    PREFIX "/translations"
    FILES
        translations/app_zh_CN.qm
        translations/app_ja.qm
        translations/app_de.qm
)
```

---

## 8. QLocale 数字与日期格式化

### 8.1 数字格式化

```cpp
QLocale locale(QLocale::Chinese);

// 数字格式化
double value = 1234567.89;
QString formatted = locale.toString(value, 'f', 2);
// 中文: "1,234,567.89"
// 德语: "1.234.567,89"

// 货币格式化
QString currency = locale.toCurrencyString(1234.56);
// 中文: "￥1,234.56"
// 美国: "$1,234.56"

// 整数格式化
QString intStr = locale.toString(42);
// 结果: "42"

// 解析本地化数字
double parsed = locale.toDouble("1.234,56");  // 德语格式
```

### 8.2 日期和时间格式化

```cpp
QLocale locale(QLocale::Chinese);

// 日期格式化
QDate date(2026, 5, 12);
QString dateStr = locale.toString(date, QLocale::ShortFormat);
// 中文: "2026/5/12"
// 美国: "5/12/2026"

// 时间格式化
QTime time(14, 30, 0);
QString timeStr = locale.toString(time, QLocale::ShortFormat);
// 中文: "下午2:30"
// 24小时制: "14:30"

// 日期时间格式化
QDateTime dateTime(date, time);
QString dtStr = locale.toString(dateTime);

// 自定义格式
QString custom = locale.toString(dateTime, "yyyy-MM-dd hh:mm:ss");
```

### 8.3 在 QML 中使用

```qml
import QtQuick

Text {
    // 使用系统区域设置格式化日期
    text: qsTr("Date %1").arg(Qt.formatDateTime(new Date(), locale.dateFormat(QLocale.ShortFormat)))
}
```

---

## 9. 最佳实践

### 9.1 字符串处理

```cpp
// 不推荐: 硬编码拼接
label->setText("Price: $" + QString::number(price));  // 翻译者无法处理

// 推荐: 使用占位符
label->setText(tr("Price: $%1").arg(price));  // 翻译者可以调整语序

// 不推荐: 字符串拼接
statusBar->showMessage(tr("File ") + filename + tr(" loaded"));

// 推荐: 单一翻译字符串
statusBar->showMessage(tr("File %1 loaded").arg(filename));
```

### 9.2 图片中的文字

```cpp
// 不推荐: 图片中包含文字
QPixmap(":/images/logo-with-text.png");

// 推荐: 文字叠加
QPixmap(":/images/logo.png");
painter.drawText(rect, Qt::AlignCenter, tr("Welcome"));
```

### 9.3 动态布局

```cpp
// 考虑不同语言文本长度差异
// 德语/俄语文本通常比英语长 30-50%
// 预留足够空间

auto layout = new QHBoxLayout();
label->setMinimumWidth(200);  // 给翻译预留空间
button->setMinimumWidth(120);
```

### 9.4 CMake 完整配置

```cmake
cmake_minimum_required(VERSION 3.16)
project(MyApp LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Linguist)

qt_standard_project_setup(REQUIRES 6.8
    I18N_TRANSLATED_LANGUAGES zh_CN ja de fr)

add_executable(MyApp
    src/main.cpp
    src/mainwindow.cpp
)

target_link_libraries(MyApp PRIVATE Qt6::Core Qt6::Widgets)

# 添加翻译支持
qt_add_lupdate(MyApp
    SOURCES src/main.cpp src/mainwindow.cpp
    TS_FILES translations/app_zh_CN.ts translations/app_ja.ts
)

qt_add_lrelease(MyApp
    TS_FILES translations/app_zh_CN.ts translations/app_ja.ts
    QM_FILES_OUTPUT_VARIABLE QM_FILES
)

qt_add_resources(MyApp "translations"
    PREFIX "/translations"
    FILES ${QM_FILES}
)
```

---

## 10. 常见陷阱

| 陷阱 | 说明 | 解决方案 |
|------|------|----------|
| 遗漏 tr() | 字符串未标记，不会被提取 | 使用 `QT_TR_NOOP` 标记静态数组 |
| 硬编码数字 | 数字在不同语言中格式不同 | 使用 `QLocale::toString()` |
| 字符串拼接 | 不同语言语序不同 | 使用 `%1` 占位符 |
| 图片内嵌文字 | 无法翻译 | 文字叠加在图片上 |
| 固定宽度布局 | 翻译后文本溢出 | 使用弹性布局 + `setMinimumWidth` |
| 日期/时间格式 | 各地区格式不同 | 使用 `QLocale` 格式化 |
| 编码问题 | 中文等非 ASCII 字符 | 确保文件保存为 UTF-8 |

---

## 参考资源

- [Qt 6 i18n Guide](https://doc.qt.io/qt-6/i18n-source-translation.html)
- [Qt Linguist Manual](https://doc.qt.io/qt-6/linguist-manager.html)
- [QLocale](https://doc.qt.io/qt-6/qlocale.html)
- [tr() 函数](https://doc.qt.io/qt-6/qobject.html#tr)
- [Qt Linguist 示例](https://doc.qt.io/qt-6/qtlinguist-trollprint-example.html)
