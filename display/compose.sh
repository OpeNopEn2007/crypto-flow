#!/bin/bash
# 合成最终视频
# 用法: ./compose.sh

set -e

SCRIPT_DIR="$(dirname "$0")"
cd "$SCRIPT_DIR"

FRAGMENTS_DIR="fragments"
OUTPUT_DIR="output"
OUTPUT_FILE="$OUTPUT_DIR/CryptoFlow_demo.mp4"

echo "=========================================="
echo "CryptoFlow 视频合成脚本"
echo "=========================================="
echo ""

# 检查 ffmpeg
if ! command -v ffmpeg &> /dev/null; then
    echo "错误: 未找到 ffmpeg"
    echo "请先安装: brew install ffmpeg"
    exit 1
fi

# 检查片段目录
if [ ! -d "$FRAGMENTS_DIR" ]; then
    echo "错误: 未找到 fragments/ 目录"
    echo "请先运行录制脚本: ./record.sh"
    exit 1
fi

# 检查必要文件
echo "检查片段文件..."
REQUIRED_FILES=(
    "01_title.png"
    "02_interface.mov"
    "03_caesar.mov"
    "04_vigenere.mov"
    "05_rsa.mov"
    "06_base64.mov"
    "07_xor.mov"
    "08_cli.mov"
    "09_ending.png"
)

MISSING=0
for file in "${REQUIRED_FILES[@]}"; do
    if [ -f "$FRAGMENTS_DIR/$file" ]; then
        echo "  ✓ $file"
    else
        echo "  ✗ $file (缺失)"
        MISSING=1
    fi
done

if [ $MISSING -eq 1 ]; then
    echo ""
    echo "错误: 有片段缺失，请先完成录制"
    exit 1
fi

echo ""
echo "所有片段检查通过！"
echo ""

# 创建输出目录
mkdir -p "$OUTPUT_DIR"

# 创建 concat 列表
echo "生成拼接列表..."
CONCAT_FILE="$OUTPUT_DIR/concat.txt"
> "$CONCAT_FILE"

for file in "${REQUIRED_FILES[@]}"; do
    echo "file '../fragments/$file'" >> "$CONCAT_FILE"
done

echo "拼接列表内容:"
cat "$CONCAT_FILE"
echo ""

# 第一步：拼接片段（无字幕）
echo "=========================================="
echo "步骤 1: 拼接视频片段"
echo "=========================================="
TEMP_VIDEO="$OUTPUT_DIR/temp_no_sub.mp4"
ffmpeg -y -f concat -safe 0 -i "$CONCAT_FILE" \
    -c:v libx264 -pix_fmt yuv420p \
    -vf "scale=1280:720:force_original_aspect_ratio=decrease,pad=1280:720:(ow-iw)/2:(oh-ih)/2" \
    "$TEMP_VIDEO"

echo ""
echo "=========================================="
echo "步骤 2: 添加字幕"
echo "=========================================="
ffmpeg -y -i "$TEMP_VIDEO" \
    -vf "subtitles=subtitles.srt:force_style='FontName=PingFang SC,FontSize=24,PrimaryColour=&H00FFFFFF,OutlineColour=&H00000000,Outline=2'" \
    -c:v libx264 -pix_fmt yuv420p \
    "$OUTPUT_FILE"

# 清理临时文件
rm -f "$TEMP_VIDEO"
rm -f "$CONCAT_FILE"

echo ""
echo "=========================================="
echo "合成完成！"
echo "=========================================="
echo ""
echo "输出文件: $OUTPUT_FILE"
echo ""

# 显示视频信息
echo "视频信息:"
ffprobe -v quiet -show_format -show_streams "$OUTPUT_FILE" 2>/dev/null | grep -E "(duration|width|height|codec_name)" | head -10

echo ""
echo "下一步:"
echo "1. 检查视频: open $OUTPUT_FILE"
echo "2. 上传到 Bilibili"
echo "3. 标题: 【南开大学26C++】CryptoFlow"
