# CryptoFlow Design

交互式密码学原理可视化演示系统

## 技术栈

- Qt 6 + C++17 + CMake
- QGraphicsView/QGraphicsScene 做动画
- QSS 深色科技主题（霓虹蓝/绿高亮，深灰背景）

## 架构

```
┌─────────────────────────────────────┐
│            MainWindow               │
│  ┌──────────────────────┬──────────┐│
│  │   Visualization      │ Control  ││
│  │   Canvas (75%)       │ Panel    ││
│  │                      │ (25%)    ││
│  │  - QGraphicsView     │ - 算法选择││
│  │  - 动画场景           │ - 参数输入││
│  │  - 数据流可视化       │ - 播放控制││
│  └──────────────────────┴──────────┘│
└─────────────────────────────────────┘
```

## 模块划分

1. **CryptoEngine** — 纯算法层，无 UI 依赖
   - CaesarCipher: 凯撒密码加解密
   - RSAEngine: RSA 密钥生成、加解密
   - AESEngine: AES 加解密（加分项）

2. **AnimationScene** — QGraphicsScene 子类
   - CaesarScene: 转盘旋转 + 字母替换高亮
   - RSAScene: 数字流 + 密钥生成过程可视化
   - AESScene: S-Box 矩阵动画（加分项）

3. **ControlPanel** — 右侧参数面板
   - 算法选择 ComboBox
   - 参数输入区（滑块/输入框，根据算法动态切换）
   - 播放控制（开始/暂停/重置/速度）

4. **MainWindow** — 主窗口，组装各模块

## MVP 范围（必须完成）

- 凯撒密码：转盘旋转动画 + 字母替换高亮
- RSA：数字流动画展示密钥生成 + 加解密过程
- 深色科技风主题
- 左右分屏布局（左可视化 75%，右控制面板 25%）

## 加分项（时间允许）

- AES S-Box 矩阵动画
- 算法 Tab 切换
- 动画速度控制
- 加密/解密双向演示

## 项目结构

```
crypto-flow/
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── mainwindow.h/cpp
│   ├── controlpanel.h/cpp
│   ├── crypto/
│   │   ├── caesar.h/cpp
│   │   └── rsa.h/cpp
│   └── scenes/
│       ├── caesarscene.h/cpp
│       └── rsascene.h/cpp
├── resources/
│   └── style.qss
└── tests/
    └── test_crypto.cpp
```

## 调试与日志

- 使用 Qt 的 qDebug/qWarning/qCritical 分级日志
- 自定义日志 handler，输出到文件 + 控制台
- 算法层独立可测试，单元测试覆盖核心逻辑
