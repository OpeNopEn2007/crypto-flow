#include "xorscene.h"
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QFontMetrics>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QtMath>

static const double SW = 600;
static const double SH = 500;
static const double PLAIN_Y = 160;  // 明文轨道
static const double KEY_Y = 280;    // 密钥轨道
static const double MERGE_X = 300;  // XOR 运算位置
static const double RESULT_Y = 220; // 结果轨道

static QGraphicsDropShadowEffect* glow(QColor c, int b = 15) {
    auto* e = new QGraphicsDropShadowEffect;
    e->setBlurRadius(b);
    e->setColor(c);
    e->setOffset(0);
    return e;
}

XORScene::XORScene(QObject* parent)
    : QGraphicsScene(parent)
    , animTimer_(new QTimer(this))
    , phaseTimer_(new QTimer(this)) {
    setSceneRect(0, 0, SW, SH);
    connect(animTimer_, &QTimer::timeout, this, &XORScene::onAnimFrame);
    connect(phaseTimer_, &QTimer::timeout, this, &XORScene::onPhaseDelay);
}

void XORScene::setupPipeline() {
    title_ = addCenteredText("XOR 加密", QFont("PingFang SC", 20, QFont::Bold),
                              QColor(255, 100, 200), 15);
    title_->setGraphicsEffect(glow(QColor(255, 100, 200), 20));

    // 上轨道（明文）
    addLine(30, PLAIN_Y, MERGE_X - 30, PLAIN_Y,
            QPen(QColor(0, 180, 255, 60), 2));
    addText("明文", QFont("PingFang SC", 10))->setDefaultTextColor(QColor(0, 180, 255, 150));
    auto* pt = items().last();
    pt->setPos(10, PLAIN_Y - 25);

    // 下轨道（密钥）
    addLine(30, KEY_Y, MERGE_X - 30, KEY_Y,
            QPen(QColor(255, 100, 200, 60), 2));
    auto* kt = addText("密钥", QFont("PingFang SC", 10));
    kt->setDefaultTextColor(QColor(255, 100, 200, 150));
    kt->setPos(10, KEY_Y + 10);

    // XOR 运算器（菱形）
    QPolygonF diamond({
        QPointF(MERGE_X, PLAIN_Y + 15),
        QPointF(MERGE_X + 50, RESULT_Y),
        QPointF(MERGE_X, KEY_Y - 15),
        QPointF(MERGE_X - 50, RESULT_Y)
    });
    auto* xorBg = addPolygon(diamond,
                              QPen(QColor(255, 200, 80), 2),
                              QBrush(QColor(30, 25, 15, 220)));
    xorBg->setZValue(-1);
    auto* xorLabel = addText("XOR", QFont("Menlo", 14, QFont::Bold));
    xorLabel->setDefaultTextColor(QColor(255, 200, 80));
    QRectF xlr = xorLabel->boundingRect();
    xorLabel->setPos(MERGE_X - xlr.width() / 2, RESULT_Y - xlr.height() / 2);
    xorLabel->setZValue(1);

    // 输出管道
    addLine(MERGE_X + 50, RESULT_Y, 570, RESULT_Y,
            QPen(QColor(0, 255, 150, 60), 2));
    auto* ot = addText("密文", QFont("PingFang SC", 10));
    ot->setDefaultTextColor(QColor(0, 255, 150, 150));
    ot->setPos(560, RESULT_Y - 25);

    // 汇入箭头
    addFlowArrow(MERGE_X - 30, PLAIN_Y, MERGE_X - 50, RESULT_Y, QColor(0, 180, 255, 150));
    addFlowArrow(MERGE_X - 30, KEY_Y, MERGE_X - 50, RESULT_Y, QColor(255, 100, 200, 150));
    addFlowArrow(MERGE_X + 50, RESULT_Y, MERGE_X + 70, RESULT_Y, QColor(0, 255, 150, 150));
}

void XORScene::addDataBlock(double x, double y, const QString& text, QColor color,
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
        block.label->setPos(x + 10, y + th + 3);
        block.label->setZValue(4);
    }

    block.targetX = x;
    block.targetY = y;
    block.color = color;
    dataBlocks_.append(block);
}

void XORScene::clearDataBlocks() {
    for (auto& b : dataBlocks_) {
        if (b.bg) { removeItem(b.bg); delete b.bg; }
        if (b.text) { removeItem(b.text); delete b.text; }
        if (b.label) { removeItem(b.label); delete b.label; }
    }
    dataBlocks_.clear();
}

void XORScene::addFlowArrow(double x1, double y1, double x2, double y2, QColor color) {
    addLine(x1, y1, x2, y2, QPen(color, 2));
    double angle = atan2(y2 - y1, x2 - x1);
    double len = 8;
    QPointF p1(x2, y2);
    QPointF p2(x2 - len * cos(angle - 0.4), y2 - len * sin(angle - 0.4));
    QPointF p3(x2 - len * cos(angle + 0.4), y2 - len * sin(angle + 0.4));
    addPolygon(QPolygonF({p1, p2, p3}), QPen(Qt::NoPen), QBrush(color));
}

QGraphicsTextItem* XORScene::addCenteredText(const QString& text, const QFont& font,
                                               QColor color, double y) {
    auto* t = addText(text, font);
    t->setDefaultTextColor(color);
    QRectF r = t->boundingRect();
    t->setPos((SW - r.width()) / 2, y);
    t->setZValue(5);
    return t;
}

void XORScene::addExplanation(const QString& text) {
    if (explanation_) {
        removeItem(explanation_);
        delete explanation_;
    }
    explanation_ = addCenteredText(text, QFont("PingFang SC", 11),
                                    QColor(180, 180, 190), SH - 60);
}

void XORScene::startAnimation(const QString& text, const QString& key) {
    reset();
    inputText_ = text;
    keyText_ = key;
    inputBytes_ = text.toUtf8();
    keyBytes_ = key.toUtf8();

    resultBytes_.resize(inputBytes_.size());
    for (int i = 0; i < inputBytes_.size(); i++) {
        resultBytes_[i] = inputBytes_[i] ^ keyBytes_[i % keyBytes_.size()];
    }

    qInfo() << "[XOR] Start:" << text << "key:" << key;

    setupPipeline();

    animationId_++;
    int myId = animationId_;
    currentPhase_ = 0;

    QTimer::singleShot(1500, this, [this, myId]() {
        if (myId != animationId_) return;
        onPhaseDelay();
    });
}

void XORScene::onPhaseDelay() {
    clearDataBlocks();
    int myId = animationId_;

    if (currentPhase_ == 0) {
        // Phase 1: 明文和密钥进入
        addExplanation("明文和密钥分别进入 XOR 运算器");

        // 明文从左边滑入（上轨道）
        addDataBlock(-200, PLAIN_Y - 16, "\"" + inputText_ + "\"",
                     QColor(0, 230, 255), "明文");
        for (auto& b : dataBlocks_) {
            b.targetX = 60;
        }

        animFrame_ = 0;
        animTimer_->start(30);

        QTimer::singleShot(animSpeed_, this, [this, myId]() {
            if (myId != animationId_) return;
            clearDataBlocks();

            // 明文停在上轨道
            addDataBlock(60, PLAIN_Y - 16, "\"" + inputText_ + "\"",
                         QColor(0, 230, 255), "明文");

            // 密钥从左边滑入（下轨道）
            addDataBlock(-200, KEY_Y - 16, "\"" + keyText_ + "\"",
                         QColor(255, 150, 220), "密钥");
            for (auto& b : dataBlocks_) {
                if (b.targetY == KEY_Y - 16) b.targetX = 60;
            }

            animFrame_ = 0;
            animTimer_->start(30);

            currentPhase_++;
            phaseTimer_->start(animSpeed_);
        });

    } else if (currentPhase_ == 1) {
        // Phase 2: 二进制对比
        addExplanation("将明文和密钥转为二进制，逐位对比");

        // 明文二进制（上轨道）
        double px = 40;
        for (int i = 0; i < qMin(inputBytes_.size(), (qsizetype)5); i++) {
            uchar p = (uchar)inputBytes_[i];
            addDataBlock(px, PLAIN_Y - 20, QString("%1").arg(p, 8, 2, QChar('0')),
                         QColor(0, 200, 255), QString("B%1").arg(i + 1));
            px += 95;
        }

        // 密钥二进制（下轨道）
        double kx = 40;
        for (int i = 0; i < qMin(inputBytes_.size(), (qsizetype)5); i++) {
            uchar k = (uchar)keyBytes_[i % keyBytes_.size()];
            addDataBlock(kx, KEY_Y - 12, QString("%1").arg(k, 8, 2, QChar('0')),
                         QColor(255, 150, 220));
            kx += 95;
        }

        if (inputBytes_.size() > 5) {
            addDataBlock(px, PLAIN_Y - 20, "...", QColor(120, 120, 130));
        }

        currentPhase_++;
        phaseTimer_->start(animSpeed_);

    } else if (currentPhase_ == 2) {
        // Phase 3: XOR 运算 — 逐位碰撞
        addExplanation("XOR 规则: 相同→0, 不同→1");

        // 明文位（上轨道向 XOR 靠拢）
        double px = 50;
        for (int i = 0; i < qMin(inputBytes_.size(), (qsizetype)4); i++) {
            uchar p = (uchar)inputBytes_[i];
            uchar k = (uchar)keyBytes_[i % keyBytes_.size()];
            uchar r = p ^ k;

            double rowY = 100 + i * 80;

            // 明文
            addDataBlock(px, rowY, QString("%1").arg(p, 8, 2, QChar('0')),
                         QColor(0, 200, 255));

            // XOR 符号
            auto* xorSign = addText("XOR", QFont("Menlo", 11, QFont::Bold));
            xorSign->setDefaultTextColor(QColor(255, 200, 80));
            xorSign->setPos(160, rowY + 5);
            xorSign->setZValue(5);

            // 密钥
            addDataBlock(210, rowY, QString("%1").arg(k, 8, 2, QChar('0')),
                         QColor(255, 150, 220));

            // 等号
            addFlowArrow(320, rowY + 16, 350, rowY + 16, QColor(0, 230, 120));

            // 结果
            addDataBlock(355, rowY, QString("%1").arg(r, 8, 2, QChar('0')),
                         QColor(0, 255, 140));
        }

        if (inputBytes_.size() > 4) {
            auto* dots = addCenteredText("... 共 " + QString::number(inputBytes_.size()) + " 字节",
                                          QFont("PingFang SC", 10), QColor(120, 120, 130),
                                          100 + 4 * 80);
        }

        currentPhase_++;
        phaseTimer_->start(animSpeed_);

    } else if (currentPhase_ == 3) {
        // Phase 4: 结果输出
        addExplanation("XOR 结果即为密文，再次 XOR 同一密钥可解密");

        // 密文从 XOR 运算器滑出
        QString hexResult;
        for (int i = 0; i < resultBytes_.size(); i++) {
            if (i > 0) hexResult += " ";
            hexResult += QString("%1").arg((uchar)resultBytes_[i], 2, 16, QChar('0')).toUpper();
        }

        addDataBlock(MERGE_X + 60, RESULT_Y - 16, hexResult,
                     QColor(0, 255, 140), "密文 (Hex)");

        for (auto& b : dataBlocks_) {
            b.targetX = 570 - b.text->boundingRect().width() - 20;
        }
        animFrame_ = 0;
        animTimer_->start(30);

        QTimer::singleShot(animSpeed_, this, [this, myId]() {
            if (myId != animationId_) return;

            QString hexResult;
            for (int i = 0; i < resultBytes_.size(); i++) {
                if (i > 0) hexResult += " ";
                hexResult += QString("%1").arg((uchar)resultBytes_[i], 2, 16, QChar('0')).toUpper();
            }

            addExplanation(QString("完成: \"%1\" XOR \"%2\" → %3")
                               .arg(inputText_).arg(keyText_).arg(hexResult));

            qInfo() << "[XOR] Complete";
            currentPhase_++;
            phaseTimer_->start(1500);
        });
    } else {
        phaseTimer_->stop();
        emit animationComplete();
    }
}

void XORScene::onAnimFrame() {
    animFrame_++;
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

void XORScene::reset() {
    animTimer_->stop();
    phaseTimer_->stop();
    clear();
    dataBlocks_.clear();
    explanation_ = nullptr;
    title_ = nullptr;
    currentPhase_ = 0;
    resultBytes_.clear();
    animationId_++;
}

void XORScene::setSpeed(int ms) {
    animSpeed_ = ms;
    if (phaseTimer_->isActive()) phaseTimer_->setInterval(ms);
}
