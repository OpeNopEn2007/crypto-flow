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
./build/CryptoFlow --base64 "Hello"
./build/CryptoFlow --xor "HELLO" --xor-key "KEY"
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
- **CaesarScene**: 外圈固定=明文，内圈旋转=密文，逆时针旋转对齐；离散步进旋转（每步 500ms 暂停 + 12帧 OutCubic 缓动）；旋转完成后匀速回转初始位置（30帧线性）→ 停顿 500ms → 逐字母高亮；连接线+脉冲发光；打字机效果显示结果；**切换位移时先回转复位再启动新动画**（仅当 `currentRotation_ > 0.1` 时触发 rewind）
- **RSAScene**: 7步密钥生成动画，居中卡片布局，圆形步骤编号，绿色高亮数值，流程箭头；加密后自动将密文存入 lastCipher_，切换到解密模式时自动填充
- **VigenereScene**: 双圈转盘，根据关键词每字母切换偏移量，黄色关键词显示；密钥索引用 ki 计数器只对字母递增（非字母不消耗密钥位）；每个字母处理完后回转复位再处理下一个；切换密钥时若内圈未归零则先 rewind
- **Base64Scene**: 5步卡片流程（与 RSA 同风格）：明文→二进制→6bit分组→查表→结果，圆形编号+流程箭头
- **XORScene**: 4步卡片流程（与 RSA 同风格）：明文→密钥→逐字节XOR→加密结果，圆形编号+流程箭头
- **ControlPanel**: QStackedWidget 切换5种算法控件，速度滑块 200-2000ms，默认 1000ms；所有算法启动前做输入校验（空值、范围、素数）

### 输入校验规则（ControlPanel）
- RSA 密钥生成：p,q 必须为素数、p≠q、p,q >= 3
- RSA 加密：M 必须满足 0 <= M < n，e > 0，n > 0（先生成密钥）
- RSA 解密：C 必须满足 0 <= C < n，d > 0，n > 0
- 维吉尼亚：密钥非空、输入非空
- XOR：密钥非空、输入非空
- Base64：输入非空
- CLI --shift：越界时默认回退到 3

### 开发约定
- 信号槽只用新式语法（`&Class::method`），禁止旧式 `SIGNAL/SLOT`
- 动画用 QTimer + QEasingCurve，不用 QPropertyAnimation（QGraphicsItem 不支持）
- 内圈字母不使用 QGraphicsItemGroup（旋转会导致字母倾斜），手动 cos/sin 重算位置
- 内圈旋转方向为逆时针（targetRotation_ 为负值）
- `animationId_` 守卫：`startAnimation()` 入口 `animationId_++`，所有 `singleShot` 回调捕获当时的 `animationId_`，执行前校验 `myId != animationId_` 则 return；`reset()` 中**禁止** `animationId_++`（会导致 `launchAnimation()` 的 singleShot 捕获过期 ID）
- rewind 完成后必须先 `reset()` 再 `launchAnimation()`，否则旧场景元素残留导致字母重叠
- RSA modPow 用 `__int128` 中间变量防 int64_t 乘法溢出
- `isPrime` 用 `i <= n / i` 代替 `i * i <= n` 防整数溢出
- 卡片流程场景（RSA/Base64/XOR）：CARD_W=520, CARD_H=60~80, CARD_SPACING=72~95, START_Y=58~65

### 调试
- 日志输出到 `cryptoflow.log` + stderr
- 动画完成后自动截图 `caesar_screenshot.png` / `rsa_screenshot.png` / `vigenere_screenshot.png` / `xor_screenshot.png`
- CLI 参数支持自动启动演示