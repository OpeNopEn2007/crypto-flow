#include "base64scene.h"
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QFontMetrics>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QtMath>

static const double SW = 600;
static const double SH = 500;
static const double PIPE_Y = 200;  // 管道 Y 坐标
static const double PIPE_X1 = 30;  // 管道起点
static const double PIPE_X2 = 570; // 管道终点
static const QString B64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static QGraphicsDropShadowEffect* glow(QColor c, int b = 15) {
    auto* e = new QGraphicsDropShadowEffect;
    e->setBlurRadius(b);
    e->setColor(c);
    e->setOffset(0);
    return e;
}

Base64Scene::Base64Scene(QObject* parent)
    : QGraphicsScene(parent)
    , animTimer_(new QTimer(this))
    , phaseTimer_(new QTimer(this)) {
    setSceneRect(0, 0, SW, SH);
    connect(animTimer_, &QTimer::timeout, this, &Base64Scene::onAnimFrame);
    connect(phaseTimer_, &QTimer::timeout, this, &Base64Scene::onPhaseDelay);
}

void Base64Scene::setupPipeline() {
    // 标题
    title_ = addCenteredText("Base64 编码", QFont("PingFang SC", 20, QFont::Bold),
                              QColor(0, 210, 255), 15);
    title_->setGraphicsEffect(glow(QColor(0, 210, 255), 20));

    // 管道线（水平轴）
    pipeLine_ = addLine(PIPE_X1, PIPE_Y, PIPE_X2, PIPE_Y,
                         QPen(QColor(0, 180, 255, 60), 3));
    pipeLine_->setZValue(-1);

    // 三个处理站
    addStation(0, "编码器", "文本 → 二进制");
    addStation(1, "分组器", "8bit → 6bit");
    addStation(2, "查表器", "6bit → 字符");
}

void Base64Scene::addStation(int index, const QString& title, const QString& desc) {
    double x = 60 + index * 210;
    double w = 150;
    double h = 60;
    double y = PIPE_Y - h / 2;

    // 站台背景
    auto* bg = addRect(x, y, w, h,
                        QPen(QColor(0, 180, 255, 80), 1.5),
                        QBrush(QColor(10, 20, 35, 200)));
    bg->setZValue(-1);
    stations_[index].bg = bg;

    // 站台标题
    auto* t = addText(title, QFont("PingFang SC", 12, QFont::Bold));
    t->setDefaultTextColor(QColor(0, 210, 255));
    QRectF tr = t->boundingRect();
    t->setPos(x + (w - tr.width()) / 2, y + 8);
    t->setZValue(1);
    stations_[index].title = t;

    // 站台描述
    auto* d = addText(desc, QFont("PingFang SC", 9));
    d->setDefaultTextColor(QColor(120, 160, 180));
    QRectF dr = d->boundingRect();
    d->setPos(x + (w - dr.width()) / 2, y + 32);
    d->setZValue(1);
    stations_[index].desc = d;

    // 入口/出口箭头
    if (index > 0) {
        double arrowX = x - 15;
        addLine(arrowX, PIPE_Y, x, PIPE_Y, QPen(QColor(0, 180, 255, 120), 2));
        QPolygonF tri({QPointF(x, PIPE_Y), QPointF(x - 8, PIPE_Y - 5), QPointF(x - 8, PIPE_Y + 5)});
        addPolygon(tri, QPen(Qt::NoPen), QBrush(QColor(0, 180, 255, 150)));
    }
    if (index < 2) {
        double arrowX = x + w + 5;
        double nextX = x + w + 20;
        addLine(arrowX, PIPE_Y, nextX, PIPE_Y, QPen(QColor(0, 180, 255, 120), 2));
        QPolygonF tri({QPointF(nextX, PIPE_Y), QPointF(nextX - 8, PIPE_Y - 5), QPointF(nextX - 8, PIPE_Y + 5)});
        addPolygon(tri, QPen(Qt::NoPen), QBrush(QColor(0, 180, 255, 150)));
    }
}

void Base64Scene::addDataBlock(double x, double y, const QString& text, QColor color,
                                 const QString& label) {
    QFont font("Menlo", 13, QFont::Bold);
    QFontMetrics fm(font);
    double tw = fm.horizontalAdvance(text) + 20;
    double th = 32;

    DataBlock block;
    block.bg = addRect(x, y, tw, th,
                        QPen(color, 2), QBrush(QColor(15, 25, 40, 220)));
    block.bg->setZValue(3);

    block.text = addText(text, font);
    block.text->setDefaultTextColor(color);
    block.text->setPos(x + 10, y + 5);
    block.text->setZValue(4);

    if (!label.isEmpty()) {
        block.label = addText(label, QFont("PingFang SC", 9));
        block.label->setDefaultTextColor(QColor(150, 150, 160));
        block.label->setPos(x + tw / 2 - 10, y + th + 3);
        block.label->setZValue(4);
    }

    block.targetX = x;
    block.targetY = y;
    block.color = color;
    dataBlocks_.append(block);
}

void Base64Scene::clearDataBlocks() {
    for (auto& b : dataBlocks_) {
        if (b.bg) { removeItem(b.bg); delete b.bg; }
        if (b.text) { removeItem(b.text); delete b.text; }
        if (b.label) { removeItem(b.label); delete b.label; }
    }
    dataBlocks_.clear();
}

void Base64Scene::addFlowArrow(double x1, double y1, double x2, double y2, QColor color) {
    addLine(x1, y1, x2, y2, QPen(color, 2));
    double angle = atan2(y2 - y1, x2 - x1);
    double len = 8;
    QPointF p1(x2, y2);
    QPointF p2(x2 - len * cos(angle - 0.4), y2 - len * sin(angle - 0.4));
    QPointF p3(x2 - len * cos(angle + 0.4), y2 - len * sin(angle + 0.4));
    addPolygon(QPolygonF({p1, p2, p3}), QPen(Qt::NoPen), QBrush(color));
}

QGraphicsTextItem* Base64Scene::addCenteredText(const QString& text, const QFont& font,
                                                  QColor color, double y) {
    auto* t = addText(text, font);
    t->setDefaultTextColor(color);
    QRectF r = t->boundingRect();
    t->setPos((SW - r.width()) / 2, y);
    t->setZValue(5);
    return t;
}

void Base64Scene::addExplanation(const QString& text) {
    if (explanation_) {
        removeItem(explanation_);
        delete explanation_;
    }
    explanation_ = addCenteredText(text, QFont("PingFang SC", 11),
                                    QColor(180, 180, 190), SH - 60);
}

void Base64Scene::startAnimation(const QString& text) {
    reset();
    inputText_ = text;
    inputBytes_ = text.toUtf8();

    // 预计算
    QString bin;
    for (uchar b : inputBytes_) {
        bin += QString("%1").arg(b, 8, 2, QChar('0'));
    }
    binaryString_ = bin;

    QString padded = bin;
    while (padded.size() % 6 != 0) padded += '0';
    for (int i = 0; i < padded.size(); i += 6) {
        sixBitGroups_.append(padded.mid(i, 6));
    }

    for (auto& g : sixBitGroups_) {
        int val = 0;
        for (int j = 0; j < 6; j++) val = val * 2 + (g[j] == '1' ? 1 : 0);
        groupValues_.append(val);
        b64Result_ += B64[val];
    }
    while (b64Result_.size() % 4 != 0) b64Result_ += '=';

    qInfo() << "[Base64] Start:" << text << "->" << b64Result_;

    setupPipeline();

    animationId_++;
    int myId = animationId_;
    currentPhase_ = 0;

    QTimer::singleShot(1500, this, [this, myId]() {
        if (myId != animationId_) return;
        onPhaseDelay();
    });
}

void Base64Scene::onPhaseDelay() {
    clearDataBlocks();

    int myId = animationId_;
    double centerY = PIPE_Y - 16;

    if (currentPhase_ == 0) {
        // Phase 1: 文本进入 → 转为二进制
        addExplanation("原文进入编码器，每个字符转为 8 位二进制");

        // 从左边滑入原文
        addDataBlock(-100, centerY, "\"" + inputText_ + "\"",
                     QColor(0, 230, 255), "明文");

        // 动画：滑到第一个站
        for (auto& b : dataBlocks_) {
            b.targetX = 60 + 75 - b.text->boundingRect().width() / 2 - 10;
        }
        animFrame_ = 0;
        animTimer_->start(30);

        // 延迟后展示二进制结果
        QTimer::singleShot(animSpeed_, this, [this, myId, centerY]() {
            if (myId != animationId_) return;
            clearDataBlocks();

            // 在站 1 输出端显示二进制
            double bx = 240;
            for (int i = 0; i < inputBytes_.size(); i++) {
                QString byteBin = binaryString_.mid(i * 8, 8);
                addDataBlock(bx, centerY - 30, byteBin,
                             QColor(0, 200, 255), QString("B%1").arg(i + 1));
                bx += 90;
            }

            addExplanation(QString("每个字符 → 8 位二进制, 共 %1 字节").arg(inputBytes_.size()));

            currentPhase_++;
            phaseTimer_->start(animSpeed_);
        });

    } else if (currentPhase_ == 1) {
        // Phase 2: 二进制 → 6bit 分组
        addExplanation("二进制流重新切分为 6 位一组");

        // 上排：8bit（从站1滑出）
        double bx1 = 220;
        for (int i = 0; i < inputBytes_.size(); i++) {
            addDataBlock(bx1, centerY - 50, binaryString_.mid(i * 8, 8),
                         QColor(0, 180, 255));
            bx1 += 90;
        }

        // 下排：6bit（从站2输出）
        double bx2 = 250;
        for (int i = 0; i < sixBitGroups_.size(); i++) {
            addDataBlock(bx2, centerY + 25, sixBitGroups_[i],
                         QColor(255, 180, 60), QString("#%1").arg(i + 1));
            bx2 += 75;
        }

        // 切割线动画：在 8bit 组之间画虚线
        addExplanation(QString("拼接后每 6 位切一刀, 得到 %1 组").arg(sixBitGroups_.size()));

        currentPhase_++;
        phaseTimer_->start(animSpeed_);

    } else if (currentPhase_ == 2) {
        // Phase 3: 6bit → 查表 → Base64 字符
        addExplanation("每个 6bit 值 (0-63) 查表得到 Base64 字符");

        // 左边：6bit 值
        double leftX = 60;
        for (int i = 0; i < sixBitGroups_.size(); i++) {
            addDataBlock(leftX, centerY - 25, sixBitGroups_[i],
                         QColor(255, 180, 60), QString("#%1 = %2").arg(i + 1).arg(groupValues_[i]));
            leftX += 75;
        }

        // 查表动画：显示映射关系
        double mapY = centerY + 40;
        double mapX = 60;
        for (int i = 0; i < sixBitGroups_.size(); i++) {
            int val = groupValues_[i];
            QChar ch = B64[val];
            // 箭头
            addFlowArrow(mapX + 25, mapY, mapX + 25, mapY + 25, QColor(0, 230, 120));
            // 字符
            addDataBlock(mapX + 10, mapY + 30, QString(ch),
                         QColor(0, 255, 140));
            mapX += 75;
        }

        currentPhase_++;
        phaseTimer_->start(animSpeed_);

    } else if (currentPhase_ == 3) {
        // Phase 4: 结果汇聚输出
        addExplanation("编码完成！所有字符拼接为最终结果");

        // 结果从右边滑出
        addDataBlock(300, centerY - 16, b64Result_,
                     QColor(0, 255, 140), "Base64 编码结果");

        // 动画：滑到右边
        for (auto& b : dataBlocks_) {
            b.targetX = 570 - b.text->boundingRect().width() - 20;
        }
        animFrame_ = 0;
        animTimer_->start(30);

        // 显示总结
        QTimer::singleShot(animSpeed_, this, [this, myId]() {
            if (myId != animationId_) return;
            addExplanation(QString("完成: \"%1\" → \"%2\" (%3 字节 → %4 字符)")
                               .arg(inputText_).arg(b64Result_)
                               .arg(inputBytes_.size()).arg(b64Result_.size()));
            qInfo() << "[Base64] Complete:" << b64Result_;
            currentPhase_++;
            phaseTimer_->start(1500);
        });
    } else {
        phaseTimer_->stop();
        emit animationComplete();
    }
}

void Base64Scene::onAnimFrame() {
    animFrame_++;
    double t = qMin(static_cast<double>(animFrame_) / ANIM_FRAMES, 1.0);
    QEasingCurve curve(QEasingCurve::OutCubic);
    double eased = curve.valueForProgress(t);

    for (auto& b : dataBlocks_) {
        double curX = b.text->x() + (b.targetX - b.text->x()) * 0.15;
        b.text->setPos(curX, b.text->y());
        if (b.bg) b.bg->setPos(curX - 10, b.bg->y());
        if (b.label) b.label->setPos(curX + 10, b.label->y());
    }

    if (animFrame_ >= ANIM_FRAMES) {
        animTimer_->stop();
        for (auto& b : dataBlocks_) {
            b.text->setPos(b.targetX, b.text->y());
            if (b.bg) b.bg->setPos(b.targetX - 10, b.bg->y());
            if (b.label) b.label->setPos(b.targetX + 10, b.label->y());
        }
    }
}

void Base64Scene::reset() {
    animTimer_->stop();
    phaseTimer_->stop();
    clear();
    dataBlocks_.clear();
    for (auto& s : stations_) { s.bg = nullptr; s.title = nullptr; s.desc = nullptr; }
    explanation_ = nullptr;
    title_ = nullptr;
    pipeLine_ = nullptr;
    currentPhase_ = 0;
    sixBitGroups_.clear();
    groupValues_.clear();
    b64Result_.clear();
    binaryString_.clear();
    animationId_++;
}

void Base64Scene::setSpeed(int ms) {
    animSpeed_ = ms;
    if (phaseTimer_->isActive()) phaseTimer_->setInterval(ms);
}
