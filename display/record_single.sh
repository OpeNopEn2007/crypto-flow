#!/bin/bash
# 录制单个视频片段
# 用法: ./record_single.sh <片段名称> <时长秒数>

set -e

FRAGMENTS_DIR="$(dirname "$0")/fragments"
mkdir -p "$FRAGMENTS_DIR"

NAME="${1:?用法: $0 <片段名称> <时长秒数>}"
DURATION="${2:?用法: $0 <片段名称> <时长秒数>}"
OUTPUT="$FRAGMENTS_DIR/${NAME}.mov"

echo "开始录制: $NAME (${DURATION}秒)"
echo "输出文件: $OUTPUT"
echo ""
echo "操作步骤："
echo "1. QuickTime Player → 文件 → 新建屏幕录制"
echo "2. 选择 CryptoFlow 窗口区域"
echo "3. 点击录制按钮"
echo "4. 按脚本操作演示"
echo "5. ${DURATION}秒后停止录制"
echo "6. 导出为 MOV 格式到: $OUTPUT"
echo ""
echo "按 Enter 开始计时..."
read

# 倒计时
for ((i=DURATION; i>0; i--)); do
    echo -ne "\r剩余时间: ${i}秒 "
    sleep 1
done
echo -ne "\r录制结束！              "
echo ""

echo "请将录制的视频保存到: $OUTPUT"
