#include "xorscene.h"
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QDebug>
#include <QGraphicsDropShadowEffect>

static const double SW = 600;
static const double SH = 500;

static QGraphicsDropShadowEffect* glow(QColor c, int b = 15) {
    auto* e = new QGraphicsDropShadowEffect;
    e->setBlurRadius(b);
    e->setColor(c);
    e->setOffset(0);
    return e;
}

XORScene::XORScene(QObject* parent)
    : QGraphicsScene(parent), phaseTimer_(new QTimer(this)) {
    setSceneRect(0, 0, SW, SH);
    connect(phaseTimer_, &QTimer::timeout, this, &XORScene::onPhaseTick);
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

QGraphicsRectItem* XORScene::addDataBox(double x, double y, double w, double h,
                                          QColor borderColor, QColor bgColor) {
    auto* box = addRect(x, y, w, h, QPen(borderColor, 2), QBrush(bgColor));
    box->setZValue(0);
    return box;
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

QGraphicsTextItem* XORScene::addLabel(double x, double y, const QString& text,
                                        QFont font, QColor color) {
    auto* t = addText(text, font);
    t->setDefaultTextColor(color);
    t->setPos(x, y);
    t->setZValue(5);
    return t;
}

void XORScene::setupTitle() {
    auto* title = addCenteredText("XOR 加密", QFont("PingFang SC", 20, QFont::Bold),
                                   QColor(255, 100, 200), 15);
    title->setGraphicsEffect(glow(QColor(255, 100, 200), 20));
    addLine(SW / 2 - 60, 48, SW / 2 + 60, 48, QPen(QColor(255, 100, 200, 80), 1));
}

void XORScene::clearPhase() {
    for (auto* item : phaseItems_) {
        removeItem(item);
        delete item;
    }
    phaseItems_.clear();
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

    setupTitle();
    totalPhases_ = 4;
    currentPhase_ = 0;

    animationId_++;
    int myId = animationId_;
    QTimer::singleShot(1500, this, [this, myId]() {
        if (myId != animationId_) return;
        onPhaseTick();
    });
}

void XORScene::onPhaseTick() {
    clearPhase();

    if (currentPhase_ >= totalPhases_) {
        phaseTimer_->stop();
        qInfo() << "[XOR] Complete";
        emit animationComplete();
        return;
    }

    double contentY = 55;
    double centerX = SW / 2;

    if (currentPhase_ == 0) {
        addCenteredText("第 1 步: 明文 & 密钥", QFont("PingFang SC", 14, QFont::Bold),
                         QColor(255, 200, 50), contentY);

        double boxY = contentY + 45;

        auto* plainBox = addDataBox(centerX - 230, boxY, 460, 55,
                                     QColor(0, 180, 255), QColor(15, 25, 40, 220));
        phaseItems_.append(plainBox);
        addLabel(centerX - 210, boxY + 5, "明文:", QFont("PingFang SC", 11),
                 QColor(150, 150, 160));
        addLabel(centerX - 140, boxY + 10, "\"" + inputText_ + "\"",
                 QFont("Menlo", 18, QFont::Bold), QColor(0, 230, 255));
        QString byteStr;
        for (int i = 0; i < inputBytes_.size(); i++) {
            if (i > 0) byteStr += " ";
            byteStr += QString("%1").arg((uchar)inputBytes_[i], 2, 16, QChar('0')).toUpper();
        }
        addLabel(centerX - 210, boxY + 35, "字节: " + byteStr, QFont("Menlo", 10),
                 QColor(100, 180, 255));

        auto* keyBox = addDataBox(centerX - 230, boxY + 70, 460, 55,
                                   QColor(255, 100, 200), QColor(30, 15, 25, 220));
        phaseItems_.append(keyBox);
        addLabel(centerX - 210, boxY + 75, "密钥:", QFont("PingFang SC", 11),
                 QColor(150, 150, 160));
        addLabel(centerX - 140, boxY + 80, "\"" + keyText_ + "\"",
                 QFont("Menlo", 18, QFont::Bold), QColor(255, 150, 220));
        QString keyCycled;
        for (int i = 0; i < inputBytes_.size(); i++) {
            if (i > 0) keyCycled += " ";
            keyCycled += QString("%1").arg((uchar)keyBytes_[i % keyBytes_.size()], 2, 16, QChar('0')).toUpper();
        }
        addLabel(centerX - 210, boxY + 105, "循环: " + keyCycled, QFont("Menlo", 10),
                 QColor(255, 130, 200));

        addCenteredText(QString("密钥长度 %1 字节, 循环使用以匹配明文长度 %2 字节")
                           .arg(keyBytes_.size()).arg(inputBytes_.size()),
                         QFont("PingFang SC", 11), QColor(150, 150, 160), boxY + 140);

    } else if (currentPhase_ == 1) {
        addCenteredText("第 2 步: 转为二进制逐位对比", QFont("PingFang SC", 14, QFont::Bold),
                         QColor(255, 160, 50), contentY);

        double bitY = contentY + 45;
        for (int i = 0; i < qMin(inputBytes_.size(), (qsizetype)4); i++) {
            uchar p = (uchar)inputBytes_[i];
            uchar k = (uchar)keyBytes_[i % keyBytes_.size()];
            uchar r = p ^ k;
            double rowY = bitY + i * 55;

            addLabel(centerX - 250, rowY, "明文:", QFont("PingFang SC", 10),
                     QColor(0, 180, 255));
            auto* pBox = addDataBox(centerX - 180, rowY - 3, 110, 28,
                                     QColor(0, 180, 255, 150), QColor(15, 25, 40, 180));
            phaseItems_.append(pBox);
            auto* pBin = addLabel(centerX - 175, rowY, QString("%1").arg(p, 8, 2, QChar('0')),
                                   QFont("Menlo", 14, QFont::Bold), QColor(0, 230, 255));
            phaseItems_.append(pBin);

            addLabel(centerX - 55, rowY, "XOR", QFont("Menlo", 12, QFont::Bold),
                     QColor(255, 200, 80));

            addLabel(centerX - 15, rowY, "密钥:", QFont("PingFang SC", 10),
                     QColor(255, 100, 200));
            auto* kBox = addDataBox(centerX + 50, rowY - 3, 110, 28,
                                     QColor(255, 100, 200, 150), QColor(30, 15, 25, 180));
            phaseItems_.append(kBox);
            auto* kBin = addLabel(centerX + 55, rowY, QString("%1").arg(k, 8, 2, QChar('0')),
                                   QFont("Menlo", 14, QFont::Bold), QColor(255, 150, 220));
            phaseItems_.append(kBin);

            addFlowArrow(centerX + 170, rowY + 10, centerX + 195, rowY + 10,
                         QColor(0, 255, 150));
            auto* rBox = addDataBox(centerX + 200, rowY - 3, 110, 28,
                                     QColor(0, 255, 150), QColor(15, 35, 25, 220));
            phaseItems_.append(rBox);
            auto* rBin = addLabel(centerX + 205, rowY, QString("%1").arg(r, 8, 2, QChar('0')),
                                   QFont("Menlo", 14, QFont::Bold), QColor(0, 255, 140));
            phaseItems_.append(rBin);
        }

        if (inputBytes_.size() > 4) {
            addCenteredText(QString("... 共 %1 个字节").arg(inputBytes_.size()),
                             QFont("PingFang SC", 10), QColor(120, 120, 130),
                             bitY + 4 * 55);
        }

        addCenteredText("XOR 规则: 相同为 0, 不同为 1",
                         QFont("PingFang SC", 11), QColor(150, 150, 160),
                         bitY + qMin(inputBytes_.size(), (qsizetype)4) * 55 + 15);

    } else if (currentPhase_ == 2) {
        addCenteredText("第 3 步: XOR 运算规则", QFont("PingFang SC", 14, QFont::Bold),
                         QColor(0, 230, 120), contentY);

        double ruleY = contentY + 55;
        struct Rule { int a, b, result; const char* desc; };
        Rule rules[] = {
            {0, 0, 0, "0 XOR 0 = 0 (相同)"},
            {0, 1, 1, "0 XOR 1 = 1 (不同)"},
            {1, 0, 1, "1 XOR 0 = 1 (不同)"},
            {1, 1, 0, "1 XOR 1 = 0 (相同)"},
        };

        for (int i = 0; i < 4; i++) {
            double ry = ruleY + i * 45;
            auto* box1 = addDataBox(centerX - 180, ry, 35, 30,
                                     QColor(0, 180, 255), QColor(15, 25, 40, 200));
            phaseItems_.append(box1);
            addLabel(centerX - 173, ry + 5, QString::number(rules[i].a),
                     QFont("Menlo", 14, QFont::Bold), QColor(0, 230, 255));

            addLabel(centerX - 130, ry + 5, "XOR", QFont("Menlo", 12, QFont::Bold),
                     QColor(255, 200, 80));

            auto* box2 = addDataBox(centerX - 85, ry, 35, 30,
                                     QColor(255, 100, 200), QColor(30, 15, 25, 200));
            phaseItems_.append(box2);
            addLabel(centerX - 78, ry + 5, QString::number(rules[i].b),
                     QFont("Menlo", 14, QFont::Bold), QColor(255, 150, 220));

            addFlowArrow(centerX - 40, ry + 15, centerX - 15, ry + 15,
                         QColor(0, 230, 120));

            QColor resColor = rules[i].result ? QColor(0, 255, 140) : QColor(200, 80, 80);
            auto* box3 = addDataBox(centerX - 10, ry, 35, 30,
                                     resColor, QColor(15, 35, 25, 200));
            phaseItems_.append(box3);
            addLabel(centerX - 3, ry + 5, QString::number(rules[i].result),
                     QFont("Menlo", 14, QFont::Bold), resColor);

            addLabel(centerX + 40, ry + 5, rules[i].desc,
                     QFont("PingFang SC", 11), QColor(180, 180, 180));
        }

        addCenteredText("XOR 是可逆运算: (A XOR B) XOR B = A",
                         QFont("PingFang SC", 11), QColor(150, 150, 160), ruleY + 190);

    } else if (currentPhase_ == 3) {
        addCenteredText("第 4 步: 加密结果", QFont("PingFang SC", 14, QFont::Bold),
                         QColor(0, 255, 150), contentY);

        double resY = contentY + 50;

        auto* plainBox = addDataBox(centerX - 230, resY, 460, 45,
                                     QColor(0, 180, 255), QColor(15, 25, 40, 220));
        phaseItems_.append(plainBox);
        addLabel(centerX - 210, resY + 5, "明文:", QFont("PingFang SC", 11),
                 QColor(150, 150, 160));
        addLabel(centerX - 140, resY + 10, "\"" + inputText_ + "\"",
                 QFont("Menlo", 16, QFont::Bold), QColor(0, 230, 255));

        addLabel(centerX - 15, resY + 12, "XOR", QFont("Menlo", 14, QFont::Bold),
                 QColor(255, 200, 80));

        auto* keyBox = addDataBox(centerX - 230, resY + 55, 460, 45,
                                   QColor(255, 100, 200), QColor(30, 15, 25, 220));
        phaseItems_.append(keyBox);
        addLabel(centerX - 210, resY + 60, "密钥:", QFont("PingFang SC", 11),
                 QColor(150, 150, 160));
        addLabel(centerX - 140, resY + 65, "\"" + keyText_ + "\"",
                 QFont("Menlo", 16, QFont::Bold), QColor(255, 150, 220));

        addFlowArrow(centerX, resY + 105, centerX, resY + 130, QColor(0, 255, 150));

        auto* resBox = addDataBox(centerX - 230, resY + 135, 460, 55,
                                   QColor(0, 255, 150), QColor(15, 35, 25, 220));
        phaseItems_.append(resBox);
        resBox->setGraphicsEffect(glow(QColor(0, 255, 150), 20));
        addLabel(centerX - 210, resY + 140, "密文 (Hex):", QFont("PingFang SC", 11),
                 QColor(150, 150, 160));

        QString hexResult;
        for (int i = 0; i < resultBytes_.size(); i++) {
            if (i > 0) hexResult += " ";
            hexResult += QString("%1").arg((uchar)resultBytes_[i], 2, 16, QChar('0')).toUpper();
        }
        auto* resText = addLabel(centerX - 100, resY + 148, hexResult,
                                  QFont("Menlo", 20, QFont::Bold), QColor(0, 255, 140));
        phaseItems_.append(resText);
        resText->setGraphicsEffect(glow(QColor(0, 255, 140), 20));

        addCenteredText("再次 XOR 同一密钥即可解密: (密文 XOR 密钥 = 明文)",
                         QFont("PingFang SC", 11), QColor(200, 200, 200), resY + 205);
    }

    currentPhase_++;
    phaseTimer_->start(animSpeed_);
}

void XORScene::reset() {
    phaseTimer_->stop();
    clear();
    phaseItems_.clear();
    currentPhase_ = 0;
    resultBytes_.clear();
    animationId_++;
}

void XORScene::setSpeed(int ms) {
    animSpeed_ = ms;
    if (phaseTimer_->isActive()) phaseTimer_->setInterval(ms);
}
