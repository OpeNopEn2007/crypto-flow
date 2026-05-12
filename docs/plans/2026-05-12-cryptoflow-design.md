# CryptoFlow Design

交互式密码学原理可视化演示系统

## 技术栈

- Qt 6 (Widgets) + C++20 + CMake 3.20
- QGraphicsView/QGraphicsScene 做动画
- QSS 深色科技主题（#0d1117 背景，霓虹蓝/绿高亮）

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
   - VigenereCipher: 维吉尼亚密码加解密（多表替换，关键词驱动）

2. **AnimationScene** — QGraphicsScene 子类
   - CaesarScene: 双圈转盘 + 离散步进旋转 + 字母替换高亮 + 连接线 + 脉冲发光 + 打字机效果
   - RSAScene: 7步密钥生成过程动画，居中卡片布局
   - VigenereScene: 双圈转盘，根据关键词每字母切换偏移量
   - Base64Scene: 5步编码可视化（原文→二进制→6bit分组→查表→结果）
   - XORScene: 4步加密可视化（明文→密钥循环→逐位XOR→十六进制）

3. **ControlPanel** — 右侧参数面板
   - 算法选择 ComboBox（5种算法）
   - 参数输入区（根据算法动态切换）
   - 速度控制滑块 (100-1500ms)，默认 800ms
   - 开始/重置按钮

4. **MainWindow** — 主窗口，组装各模块
   - QSplitter 左(3)右(1)布局
   - 自定义 messageHandler 双日志（stderr + 文件）
   - 动画完成后自动截图
   - CLI 参数支持自动演示

## 凯撒密码转盘动画

### 布局
- 外圈：固定，26个大写字母（蓝色），代表明文字母表
- 内圈：可旋转，26个大写字母（橙色），代表密文字母表
- 中心：结果显示区

### 动画流程
1. **旋转阶段**：内圈逆时针旋转 `shift × (360/26)` 度，使用 OutCubic 缓动曲线
2. **高亮阶段**：逐字母高亮（黄色发光），画连接线连接明文→密文
3. **显示结果**：加密文本在中心绿色显示

### 技术要点
- 内圈字母不使用 QGraphicsItemGroup（旋转会导致字母倾斜），手动 cos/sin 重算位置
- 两阶段高亮节奏：亮(500ms) → 灭 → 停顿(150ms) → 下一个
- 脉冲动画：圆环发光强度随正弦波变化

## RSA 密钥生成动画

### 7步流程
1. 选择素数 p, q → 计算 n = p × q
2. 计算欧拉函数 φ(n) = (p-1)(q-1)
3. 选择公钥 e（与 φ(n) 互素）
4. 计算私钥 d = e⁻¹ mod φ(n)
5. 公钥 (e, n)
6. 私钥 (d, n)

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
│   │   ├── rsa.h/cpp
│   │   └── vigenere.h/cpp
│   └── scenes/
│       ├── caesarscene.h/cpp
│       ├── rsascene.h/cpp
│       ├── vigenerecene.h/cpp
│       ├── base64scene.h/cpp
│       └── xorscene.h/cpp
├── resources/
│   └── style.qss
└── docs/
    └── referances/  (Qt 6 参考文档)
```

## 调试与日志

- 自定义 messageHandler：stderr + 文件双输出
- 动画完成后自动截图
- CLI 参数：`--caesar "TEXT" --shift N` / `--rsa`

---

## 新增算法

### 维吉尼亚密码（Vigenère Cipher）

**原理**：多表替换密码，使用关键词决定每个字母的移位量。例如关键词 "KEY"，则 K=10, E=4, Y=24，每个明文字母按对应关键词字母的偏移量进行凯撒替换。

**算法层**：`VigenereCipher::encrypt(text, key)` 返回 `VigenereResult{result, shifts}`，其中 shifts 记录每个字母的偏移量。

**动画场景**：VigenereScene — 与 CaesarScene 共用双圈转盘结构，但根据关键词逐字母旋转到不同位置。黄色关键词显示 "KEY: KEY"。

### Base64 编码

**原理**：将二进制数据编码为 ASCII 字符串。UTF-8 → 二进制 → 每 6bit 一组 → 查 Base64 字母表 → 结果。

**动画场景**：Base64Scene — 5步卡片动画：
1. 原文文本
2. 转为二进制（8bit 一组）
3. 重新分组为 6bit
4. 查 Base64 字母表
5. 显示编码结果

### XOR 加密

**原理**：逐位异或运算。明文与密钥（循环使用）逐位 XOR，结果可逆（再 XOR 同一密钥即解密）。

**动画场景**：XORScene — 4步卡片动画：
1. 明文（二进制）
2. 密钥（循环展开）
3. XOR 运算过程（逐位对比）
4. 十六进制结果
