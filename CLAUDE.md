# CLAUDE.md

## Project Info

CryptoFlow — 交互式密码学原理可视化演示系统，南开大学密码学课程期末作业。

### 技术栈
- Qt 6 (Widgets) + C++20 + CMake 3.20
- QGraphicsView/QGraphicsScene 做动画
- QSS 深色科技主题（#0d1117 背景，霓虹蓝/绿高亮）
- CMAKE_AUTOMOC/AUTORCC/AUTOUIC 全开

### 构建与运行
```bash
cmake -B build -DCMAKE_PREFIX_PATH=/opt/homebrew/opt/qt@6
cmake --build build
./build/CryptoFlow
# 自动演示
./build/CryptoFlow --caesar "HELLO" --shift 3
./build/CryptoFlow --rsa
```

### 项目结构
```
src/
├── main.cpp              # CLI 解析 (--caesar, --shift, --rsa)
├── mainwindow.h/cpp      # 主窗口，组装视图+控制面板，日志，截图
├── controlpanel.h/cpp    # 右侧控制面板（算法选择、参数、速度）
├── crypto/
│   ├── caesar.h/cpp      # 凯撒密码算法层
│   ├── rsa.h/cpp         # RSA 算法层（密钥生成、加解密）
│   ├── vigenere.h/cpp    # 维吉尼亚密码算法层
│   └── (base64/xor 内嵌在 scene 中)
└── scenes/
    ├── caesarscene.h/cpp  # 凯撒转盘动画场景
    ├── rsascene.h/cpp     # RSA 密钥生成动画场景
    ├── vigenerecene.h/cpp # 维吉尼亚转盘动画场景
    ├── base64scene.h/cpp  # Base64 编码可视化场景
    └── xorscene.h/cpp     # XOR 加密可视化场景
resources/
└── style.qss             # 深色主题样式
```

### 架构要点
- **MainWindow**: QSplitter 左(3)右(1)布局，自定义 messageHandler 双日志（stderr + 文件），动画完成后自动截图
- **CaesarScene**: 外圈固定=明文，内圈旋转=密文，逆时针旋转对齐；离散步进旋转（每步 300ms 暂停 + 12帧 OutCubic 缓动）；连接线+脉冲发光；打字机效果显示结果
- **RSAScene**: 7步密钥生成动画，居中卡片布局，圆形步骤编号，绿色高亮数值，流程箭头
- **VigenereScene**: 双圈转盘，根据关键词每字母切换偏移量，黄色关键词显示
- **Base64Scene**: 5步动画：原文→二进制(8bit)→6bit分组→查表→结果，彩色卡片
- **XORScene**: 4步动画：明文→密钥循环→逐位XOR→十六进制结果
- **ControlPanel**: QStackedWidget 切换5种算法控件，速度滑块 100-1500ms，默认 800ms

### 开发约定
- 信号槽只用新式语法（`&Class::method`），禁止旧式 `SIGNAL/SLOT`
- 动画用 QTimer + QEasingCurve，不用 QPropertyAnimation（QGraphicsItem 不支持）
- 内圈字母不使用 QGraphicsItemGroup（旋转会导致字母倾斜），手动 cos/sin 重算位置
- 内圈旋转方向为逆时针（targetRotation_ 为负值）

### 调试
- 日志输出到 `cryptoflow.log` + stderr
- 动画完成后自动截图 `caesar_screenshot.png` / `rsa_screenshot.png`
- CLI 参数支持自动启动演示