#include "base64scene.h"
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QtMath>

static const double SW = 600;
static const double SH = 500;
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
    , phaseTimer_(new QTimer(this))
    , pulseTimer_(new QTimer(this)) {
    setSceneRect(0, 0, SW, SH);
    connect(phaseTimer_, &QTimer::timeout, this, &Base64Scene::onPhaseTick);
    connect(pulseTimer_, &QTimer::timeout, this, &Base64Scene::animatePulse);
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

QGraphicsRectItem* Base64Scene::addDataBox(double x, double y, double w, double h,
                                             QColor borderColor, QColor bgColor) {
    auto* box = addRect(x, y, w, h,
                         QPen(borderColor, 2),
                         QBrush(bgColor));
    box->setZValue(0);
    return box;
}

void Base64Scene::addFlowArrow(double x1, double y1, double x2, double y2, QColor color) {
    addLine(x1, y1, x2, y2, QPen(color, 2));
    // 箭头三角
    double angle = atan2(y2 - y1, x2 - x1);
    double len = 8;
    QPointF p1(x2, y2);
    QPointF p2(x2 - len * cos(angle - 0.4), y2 - len * sin(angle - 0.4));
    QPointF p3(x2 - len * cos(angle + 0.4), y2 - len * sin(angle + 0.4));
    addPolygon(QPolygonF({p1, p2, p3}), QPen(Qt::NoPen), QBrush(color));
}

QGraphicsTextItem* Base64Scene::addLabel(double x, double y, const QString& text,
                                           QFont font, QColor color) {
    auto* t = addText(text, font);
    t->setDefaultTextColor(color);
    t->setPos(x, y);
    t->setZValue(5);
    return t;
}

void Base64Scene::setupTitle() {
    auto* title = addCenteredText("Base64 编码", QFont("PingFang SC", 20, QFont::Bold),
                                   QColor(0, 210, 255), 15);
    title->setGraphicsEffect(glow(QColor(0, 210, 255), 20));
    addLine(SW / 2 - 80, 48, SW / 2 + 80, 48, QPen(QColor(0, 210, 255, 80), 1));
}

void Base64Scene::clearPhase() {
    for (auto* item : phaseItems_) {
        removeItem(item);
        delete item;
    }
    phaseItems_.clear();
}

void Base64Scene::startAnimation(const QString& text) {
    reset();
    inputText_ = text;
    inputBytes_ = text.toUtf8();

    // 预计算所有数据
    QString bin;
    for (uchar b : inputBytes_) {
        bin += QString("%1").arg(b, 8, 2, QChar('0'));
    }
    binaryString_ = bin;

    // 6bit 分组
    QString padded = bin;
    while (padded.size() % 6 != 0) padded += '0';
    for (int i = 0; i < padded.size(); i += 6) {
        sixBitGroups_.append(padded.mid(i, 6));
    }

    // 查表
    for (auto& g : sixBitGroups_) {
        int val = 0;
        for (int j = 0; j < 6; j++) val = val * 2 + (g[j] == '1' ? 1 : 0);
        groupValues_.append(val);
        b64Result_ += B64[val];
    }
    while (b64Result_.size() % 4 != 0) b64Result_ += '=';

    qInfo() << "[Base64] Start:" << text << "->" << b64Result_;

    setupTitle();
    totalPhases_ = 5;
    currentPhase_ = 0;

    pulsePhase_ = 0;
    pulseTimer_->start(50);

    animationId_++;
    int myId = animationId_;
    QTimer::singleShot(1500, this, [this, myId]() {
        if (myId != animationId_) return;
        onPhaseTick();
    });
}

void Base64Scene::onPhaseTick() {
    clearPhase();

    if (currentPhase_ >= totalPhases_) {
        phaseTimer_->stop();
        pulseTimer_->stop();
        qInfo() << "[Base64] Complete:" << b64Result_;
        emit animationComplete();
        return;
    }

    double contentY = 55;
    double centerX = SW / 2;

    if (currentPhase_ == 0) {
        // Phase 1: 原始文本 → 字节值
        addCenteredText("第 1 步: 原始文本 → 字节", QFont("PingFang SC", 14, QFont::Bold),
                         QColor(255, 200, 50), contentY);

        // 原始文本大字显示
        double textY = contentY + 45;
        auto* textBox = addDataBox(centerX - 200, textY, 400, 55,
                                    QColor(0, 180, 255), QColor(15, 25, 40, 220));
        phaseItems_.append(textBox);
        auto* textLabel = addCenteredText("\"" + inputText_ + "\"",
                                           QFont("Menlo", 22, QFont::Bold),
                                           QColor(0, 230, 255), textY + 10);
        phaseItems_.append(textLabel);

        // 流向箭头
        addFlowArrow(centerX, textY + 55, centerX, textY + 80, QColor(0, 180, 255));

        // 字节值显示
        double byteY = textY + 90;
        addLabel(centerX - 200, byteY, "UTF-8 字节:", QFont("PingFang SC", 11),
                 QColor(150, 150, 160));
        phaseItems_.append(phaseItems_.last());

        double bx = centerX - 200;
        for (int i = 0; i < inputBytes_.size(); i++) {
            uchar b = inputBytes_[i];
            // 十六进制
            auto* hex = addLabel(bx, byteY + 22, QString("0x%1").arg(b, 2, 16, QChar('0')),
                                  QFont("Menlo", 13, QFont::Bold), QColor(0, 255, 140));
            phaseItems_.append(hex);
            // ASCII 字符
            QChar ch = (b >= 32 && b < 127) ? QChar(b) : QChar('?');
            auto* chText = addLabel(bx + 5, byteY + 44, QString(ch),
                                     QFont("Menlo", 11), QColor(200, 200, 200));
            phaseItems_.append(chText);
            bx += 75;
        }

        // 说明
        addCenteredText(QString("每个字符转换为 8 位二进制, 共 %1 字节")
                           .arg(inputBytes_.size()),
                         QFont("PingFang SC", 11), QColor(150, 150, 160), byteY + 75);

    } else if (currentPhase_ == 1) {
        // Phase 2: 二进制展示
        addCenteredText("第 2 步: 转为二进制 (8bit/字节)", QFont("PingFang SC", 14, QFont::Bold),
                         QColor(255, 200, 50), contentY);

        double binY = contentY + 45;
        // 按字节分组显示二进制
        double bx = centerX - 220;
        for (int i = 0; i < inputBytes_.size(); i++) {
            QString byteBin = binaryString_.mid(i * 8, 8);
            auto* box = addDataBox(bx, binY, 85, 40,
                                    QColor(0, 180, 255), QColor(15, 25, 40, 220));
            phaseItems_.append(box);
            auto* binText = addLabel(bx + 3, binY + 10, byteBin,
                                      QFont("Menlo", 14, QFont::Bold), QColor(0, 230, 255));
            phaseItems_.append(binText);
            // 字节标签
            auto* lbl = addLabel(bx + 15, binY + 44, QString("B%1").arg(i + 1),
                                  QFont("PingFang SC", 9), QColor(120, 120, 130));
            phaseItems_.append(lbl);
            bx += 95;
        }

        // 说明
        addCenteredText("每个字节用 8 位二进制表示",
                         QFont("PingFang SC", 11), QColor(150, 150, 160), binY + 75);

    } else if (currentPhase_ == 2) {
        // Phase 3: 重新分组 8bit → 6bit
        addCenteredText("第 3 步: 重新分组 8bit → 6bit", QFont("PingFang SC", 14, QFont::Bold),
                         QColor(255, 160, 50), contentY);

        double groupY = contentY + 45;
        // 上方：原始 8bit 分组
        addLabel(centerX - 240, groupY, "原始 8bit:", QFont("PingFang SC", 10),
                 QColor(120, 120, 130));
        double bx = centerX - 240;
        for (int i = 0; i < inputBytes_.size(); i++) {
            auto* box = addDataBox(bx, groupY + 18, 85, 30,
                                    QColor(0, 180, 255, 150), QColor(15, 25, 40, 180));
            phaseItems_.append(box);
            auto* t = addLabel(bx + 3, groupY + 23, binaryString_.mid(i * 8, 8),
                                QFont("Menlo", 12), QColor(0, 200, 255));
            phaseItems_.append(t);
            bx += 95;
        }

        // 流向箭头
        addFlowArrow(centerX, groupY + 55, centerX, groupY + 80, QColor(255, 160, 50));

        // 下方：6bit 分组
        addLabel(centerX - 240, groupY + 85, "重组 6bit:", QFont("PingFang SC", 10),
                 QColor(120, 120, 130));
        bx = centerX - 200;
        for (int i = 0; i < sixBitGroups_.size(); i++) {
            auto* box = addDataBox(bx, groupY + 103, 65, 30,
                                    QColor(255, 160, 50), QColor(30, 25, 15, 220));
            phaseItems_.append(box);
            auto* t = addLabel(bx + 3, groupY + 108, sixBitGroups_[i],
                                QFont("Menlo", 12, QFont::Bold), QColor(255, 200, 80));
            phaseItems_.append(t);
            // 编号
            auto* num = addLabel(bx + 18, groupY + 137, QString("#%1").arg(i + 1),
                                  QFont("Menlo", 9), QColor(150, 150, 150));
            phaseItems_.append(num);
            bx += 75;
        }

        addCenteredText(QString("将 %1 个字节的二进制拼接后, 每 6 位分为一组, 共 %2 组")
                           .arg(inputBytes_.size()).arg(sixBitGroups_.size()),
                         QFont("PingFang SC", 11), QColor(150, 150, 160), groupY + 155);

    } else if (currentPhase_ == 3) {
        // Phase 4: 查表映射
        addCenteredText("第 4 步: Base64 字母表查表", QFont("PingFang SC", 14, QFont::Bold),
                         QColor(0, 230, 120), contentY);

        // 显示 Base64 字母表参考
        double tableY = contentY + 40;
        addLabel(centerX - 250, tableY, "字母表: A B C D ... X Y Z a b c ... z 0 1 ... 9 + /",
                 QFont("Menlo", 9), QColor(100, 100, 110));

        // 映射过程
        double mapY = tableY + 25;
        double bx = centerX - 220;
        for (int i = 0; i < sixBitGroups_.size(); i++) {
            int val = groupValues_[i];
            QChar ch = B64[val];

            // 6bit 值
            auto* box1 = addDataBox(bx, mapY, 55, 35,
                                     QColor(255, 160, 50), QColor(30, 25, 15, 220));
            phaseItems_.append(box1);
            auto* t1 = addLabel(bx + 3, mapY + 8, sixBitGroups_[i],
                                 QFont("Menlo", 11), QColor(255, 200, 80));
            phaseItems_.append(t1);

            // 数值
            auto* numLabel = addLabel(bx + 12, mapY + 40, QString::number(val),
                                       QFont("Menlo", 10), QColor(180, 180, 180));
            phaseItems_.append(numLabel);

            // 箭头
            addFlowArrow(bx + 55, mapY + 17, bx + 75, mapY + 17, QColor(0, 230, 120));

            // 映射结果字符
            auto* box2 = addDataBox(bx + 75, mapY, 35, 35,
                                     QColor(0, 230, 120), QColor(15, 35, 25, 220));
            phaseItems_.append(box2);
            auto* t2 = addLabel(bx + 83, mapY + 6, QString(ch),
                                 QFont("Menlo", 16, QFont::Bold), QColor(0, 255, 140));
            phaseItems_.append(t2);

            bx += 125;
        }

        addCenteredText("每个 6bit 值 (0-63) 对应字母表中的一个字符",
                         QFont("PingFang SC", 11), QColor(150, 150, 160), mapY + 70);

    } else if (currentPhase_ == 4) {
        // Phase 5: 最终结果
        addCenteredText("第 5 步: 编码结果", QFont("PingFang SC", 14, QFont::Bold),
                         QColor(0, 255, 150), contentY);

        double resY = contentY + 50;

        // 原文回顾
        auto* origBox = addDataBox(centerX - 220, resY, 440, 50,
                                    QColor(0, 180, 255), QColor(15, 25, 40, 220));
        phaseItems_.append(origBox);
        addLabel(centerX - 200, resY + 5, "明文:", QFont("PingFang SC", 11),
                 QColor(150, 150, 160));
        auto* origText = addLabel(centerX - 130, resY + 12, "\"" + inputText_ + "\"",
                                   QFont("Menlo", 18, QFont::Bold), QColor(0, 230, 255));
        phaseItems_.append(origText);

        // 流向箭头
        addFlowArrow(centerX, resY + 50, centerX, resY + 75, QColor(0, 255, 150));

        // 结果
        auto* resBox = addDataBox(centerX - 220, resY + 80, 440, 60,
                                   QColor(0, 255, 150), QColor(15, 35, 25, 220));
        phaseItems_.append(resBox);
        resBox->setGraphicsEffect(glow(QColor(0, 255, 150), 20));
        addLabel(centerX - 200, resY + 85, "Base64:", QFont("PingFang SC", 11),
                 QColor(150, 150, 160));
        auto* resText = addLabel(centerX - 120, resY + 92, b64Result_,
                                  QFont("Menlo", 24, QFont::Bold), QColor(0, 255, 140));
        phaseItems_.append(resText);
        resText->setGraphicsEffect(glow(QColor(0, 255, 140), 20));

        // 说明
        addCenteredText(QString("编码完成: %1 字节 → %2 个 Base64 字符")
                           .arg(inputBytes_.size()).arg(b64Result_.size()),
                         QFont("PingFang SC", 12), QColor(200, 200, 200), resY + 155);
    }

    currentPhase_++;
    phaseTimer_->start(animSpeed_);
}

void Base64Scene::animatePulse() {
    pulsePhase_ += 0.06;
    // 轻微的背景脉冲效果 (通过场景边框)
}

void Base64Scene::reset() {
    phaseTimer_->stop();
    pulseTimer_->stop();
    clear();
    phaseItems_.clear();
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
