#!/bin/bash
# 录制所有视频片段
# 用法: ./record.sh

set -e

SCRIPT_DIR="$(dirname "$0")"
cd "$SCRIPT_DIR"

echo "=========================================="
echo "CryptoFlow 视频录制脚本"
echo "=========================================="
echo ""
echo "将按照以下顺序录制 9 个片段："
echo ""
echo "1. 01_title      - 开场标题 (静态图片, 15秒)"
echo "2. 02_interface  - 界面展示 (30秒)"
echo "3. 03_caesar     - 凯撒密码 (60秒)"
echo "4. 04_vigenere   - 维吉尼亚密码 (60秒)"
echo "5. 05_rsa        - RSA (90秒)"
echo "6. 06_base64     - Base64 (40秒)"
echo "7. 07_xor        - XOR (40秒)"
echo "8. 08_cli        - CLI 演示 (30秒)"
echo "9. 09_ending     - 结尾 (静态图片, 15秒)"
echo ""
echo "总时长: 约 6分20秒"
echo ""
echo "按 Enter 开始录制..."
read

# 创建目录
mkdir -p fragments

# 录制动态片段
echo ""
echo "=========================================="
echo "[2/8] 录制界面展示 (30秒)"
echo "=========================================="
./record_single.sh 02_interface 30

echo ""
echo "=========================================="
echo "[3/8] 录制凯撒密码 (60秒)"
echo "=========================================="
echo "操作: 输入 HELLO → 偏移 3 → 点击加密 → 等待动画完成"
./record_single.sh 03_caesar 60

echo ""
echo "=========================================="
echo "[4/8] 录制维吉尼亚密码 (60秒)"
echo "=========================================="
echo "操作: 输入 ATTACK → 密钥 LEMON → 点击加密 → 等待动画完成"
./record_single.sh 04_vigenere 60

echo ""
echo "=========================================="
echo "[5/8] 录制 RSA (90秒)"
echo "=========================================="
echo "操作: p=61, q=53 → 生成密钥 → 等待7步动画 → 加密 65 → 解密"
./record_single.sh 05_rsa 90

echo ""
echo "=========================================="
echo "[6/8] 录制 Base64 (40秒)"
echo "=========================================="
echo "操作: 输入 Hello → 点击编码 → 等待动画完成"
./record_single.sh 06_base64 40

echo ""
echo "=========================================="
echo "[7/8] 录制 XOR (40秒)"
echo "=========================================="
echo "操作: 输入 HELLO → 密钥 KEY → 点击加密 → 等待动画完成"
./record_single.sh 07_xor 40

echo ""
echo "=========================================="
echo "[8/8] 录制 CLI 演示 (30秒)"
echo "=========================================="
echo "操作: 终端执行 CLI 命令"
./record_single.sh 08_cli 30

echo ""
echo "=========================================="
echo "录制完成！"
echo "=========================================="
echo ""
echo "请确保以下文件已保存到 fragments/ 目录："
echo "  - 01_title.png (需手动创建)"
echo "  - 02_interface.mov"
echo "  - 03_caesar.mov"
echo "  - 04_vigenere.mov"
echo "  - 05_rsa.mov"
echo "  - 06_base64.mov"
echo "  - 07_xor.mov"
echo "  - 08_cli.mov"
echo "  - 09_ending.png (需手动创建)"
echo ""
echo "下一步: 运行 ./compose.sh 合成最终视频"
