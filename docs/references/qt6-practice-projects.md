# Qt 6 实际项目案例与开源项目参考

> 来源: GitHub Topics (github.com/topics/qt6), Qt 官方示例 (code.qt.io), 各项目官方仓库
> 获取日期: 2026-05-12

---

## 一、Qt 官方示例仓库

### 1. qtbase (官方核心示例)
- **仓库**: https://github.com/qt/qtbase
- **说明**: Qt 核心模块源码，包含大量 Widgets、Core、Network 等模块的官方示例
- **示例路径**: `examples/` 目录下按模块分类
- **关键示例**:
  - `examples/widgets/` - 传统 Widget 应用示例
  - `examples/network/` - 网络编程示例 (Fortune Client/Server)
  - `examples/sql/` - 数据库操作示例

### 2. qtdeclarative (QML/Qt Quick 示例)
- **仓库**: https://github.com/qt/qtdeclarative
- **说明**: QML 引擎和 Qt Quick 模块源码
- **示例路径**: `examples/` 目录
- **关键示例**:
  - `examples/quick/` - Qt Quick 基础示例
  - `examples/qml/` - QML 语言示例

### 3. qtquickcontrols2
- **仓库**: https://github.com/qt/qtquickcontrols2
- **说明**: Qt Quick Controls 2 样式和控件示例

---

## 二、知名 Qt 6 开源项目案例

### 1. OBS Studio
- **仓库**: https://github.com/obsproject/obs-studio
- **Stars**: 65k+
- **说明**: 跨平台视频录制和直播软件，新版本已迁移到 Qt 6
- **技术栈**: Qt 6 + CMake + C++17
- **学习价值**: 大型 C++ 桌面应用的 Qt 6 迁移实践、CMake 构建系统集成

### 2. MuseScore
- **仓库**: https://github.com/musescore/MuseScore
- **Stars**: 17k+
- **说明**: 开源乐谱编辑器，已迁移到 Qt 6
- **技术栈**: Qt 6 + CMake
- **学习价值**: 音频/MIDI 处理与 Qt UI 集成、自定义控件开发

### 3. FreeCAD
- **仓库**: https://github.com/FreeCAD/FreeCAD
- **Stars**: 20k+
- **说明**: 参数化 3D CAD 模型器，正在迁移到 Qt 6
- **技术栈**: Qt 6 + CMake + Python
- **学习价值**: 3D 渲染与 Qt 集成、Python/C++ 混合编程

### 4. Tiled Map Editor
- **仓库**: https://github.com/mapeditor/tiled
- **Stars**: 11k+
- **说明**: 通用地图编辑器，用于 2D 游戏开发
- **技术栈**: Qt 6 + CMake
- **学习价值**: 小型到中型 Qt 6 项目的最佳实践、QML 与 Widgets 混用

### 5. Nextcloud Desktop Client
- **仓库**: https://github.com/nextcloud/desktop
- **Stars**: 2k+
- **说明**: Nextcloud 桌面同步客户端，已迁移到 Qt 6
- **技术栈**: Qt 6 + CMake + C++17
- **学习价值**: 云存储同步、网络编程、系统托盘集成

### 6. OpenToonz
- **仓库**: https://github.com/opentoonz/opentoonz
- **Stars**: 4k+
- **说明**: 吉卜力工作室使用的动画制作软件
- **技术栈**: Qt 6 + CMake
- **学习价值**: 多媒体处理、自定义渲染管线

---

## 三、Qt 6 模板与入门项目

### 1. qt6-cmake-template
- **搜索**: GitHub 搜索 `qt6 cmake template`
- **说明**: 社区维护的 Qt 6 + CMake 项目模板
- **包含**: 基本目录结构、CMake 配置、QML 集成

### 2. Qt 官方 Example 索引
- **地址**: https://doc.qt.io/qt-6/examples.html
- **说明**: Qt 6 官方示例索引页面，按模块分类
- **建议**: 重点关注 `sql/`、`network/`、`quick/` 分类下的示例

---

## 四、GitHub 搜索推荐关键词

| 搜索词 | 用途 |
|--------|------|
| `qt6 example` | 入门级项目和演示 |
| `qt6 qml app` | QML 现代 UI 项目 |
| `qt6 cmake template` | 项目启动模板 |
| `qt6 widgets` | 传统桌面 Widget 应用 |
| `qt6 desktop app` | 完整桌面应用 |
| `qt6 database` | 数据库相关项目 |
| `qt6 websocket` | WebSocket 应用 |

---

## 五、项目结构参考

推荐的 Qt 6 项目目录结构（基于官方文档）：

```
<project root>
├── CMakeLists.txt          # 顶层 CMake 配置
├── src/
│   ├── app/
│   │   ├── CMakeLists.txt  # 应用程序构建配置
│   │   ├── main.cpp
│   │   ├── mainwindow.cpp
│   │   ├── mainwindow.h
│   │   └── mainwindow.ui
│   ├── businesslogic/
│   │   ├── CMakeLists.txt  # 业务逻辑库
│   │   ├── businesslogic.cpp
│   │   └── businesslogic.h
│   └── models/
│       ├── CMakeLists.txt  # 数据模型库
│       └── ...
├── qml/
│   └── Main.qml            # QML 文件（如果使用 QML）
├── resources/
│   ├── images/
│   └── translations/       # 国际化文件
└── tests/
    └── CMakeLists.txt      # 测试配置
```
