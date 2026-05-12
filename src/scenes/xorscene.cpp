#include "xorscene.h"
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QDebug>
#include <QGraphicsDropShadowEffect>

static const double SW = 600;
static const double CW = 480;
static const double CH = 56;
static const double CX_ = (SW - CW) / 2;
static const double SP = 62;
static const double SY = 45;

static QGraphicsDropShadowEffect* gx(QColor c, int b = 15) {
    auto* e = new QGraphicsDropShadowEffect;
    e->setBlurRadius(b);
    e->setColor(c);
    e->setOffset(0);
    return e;
}

XORScene::XORScene(QObject* parent)
    : QGraphicsScene(parent), timer_(new QTimer(this)) {
    setSceneRect(0, 0, SW, 500);
    connect(timer_, &QTimer::timeout, this, &XORScene::animateStep);
}

void XORScene::addCard(int step, const QString& title, const QString& detail,
                        const QString& value, QColor accent) {
    double y = SY + (step - 1) * SP;

    addRect(CX_ + 2, y + 3, CW, CH, QPen(Qt::NoPen), QBrush(QColor(0, 0, 0, 50)))->setZValue(-1);
    addRect(CX_, y, CW, CH,
            QPen(QColor(accent.red(), accent.green(), accent.blue(), 60), 1),
            QBrush(QColor(20, 28, 40, 240)))->setZValue(0);

    auto* bar = addRect(CX_, y, 5, CH, QPen(Qt::NoPen), QBrush(accent));
    bar->setZValue(1);
    bar->setGraphicsEffect(gx(accent, 10));

    auto* num = addText(QString("#%1").arg(step), QFont("Menlo", 10, QFont::Bold));
    num->setPos(CX_ + 14, y + 5);
    num->setDefaultTextColor(accent);
    num->setZValue(1);

    auto* t = addText(title, QFont("PingFang SC", 11));
    t->setPos(CX_ + 50, y + 5);
    t->setDefaultTextColor(QColor(220, 220, 230));
    t->setZValue(1);

    auto* d = addText(detail, QFont("Menlo", 10));
    d->setPos(CX_ + 50, y + 26);
    d->setDefaultTextColor(QColor(80, 180, 255));
    d->setZValue(1);

    if (!value.isEmpty()) {
        auto* v = addText(value, QFont("Menlo", 13, QFont::Bold));
        double vw = v->boundingRect().width();
        v->setPos(CX_ + CW - vw - 20, y + CH / 2 - 10);
        v->setDefaultTextColor(QColor(0, 255, 140));
        v->setGraphicsEffect(gx(QColor(0, 255, 140), 12));
        v->setZValue(1);
    }

    if (step > 1) {
        double prevY = SY + (step - 2) * SP + CH;
        double mx = SW / 2;
        addLine(mx, prevY + 2, mx, y - 2,
                QPen(QColor(accent.red(), accent.green(), accent.blue(), 100), 1.5))->setZValue(0);
        addPolygon(QPolygonF({QPointF(mx-4, y-4), QPointF(mx+4, y-4), QPointF(mx, y+2)}),
                   QPen(Qt::NoPen), QBrush(QColor(accent.red(), accent.green(), accent.blue(), 180)))->setZValue(1);
    }
}

void XORScene::startAnimation(const QString& text, const QString& key) {
    reset();

    auto* title = addText("XOR 加密", QFont("PingFang SC", 18, QFont::Bold));
    title->setDefaultTextColor(QColor(0, 210, 255));
    title->setGraphicsEffect(gx(QColor(0, 210, 255), 20));
    QRectF tr = title->boundingRect();
    title->setPos((SW - tr.width()) / 2, 10);
    addLine((SW - tr.width()) / 2, 38, (SW + tr.width()) / 2, 38,
            QPen(QColor(0, 210, 255, 80), 1));

    QByteArray textBytes = text.toUtf8();
    QByteArray keyBytes = key.toUtf8();

    QString textBin, keyBin, xorBin;
    for (int i = 0; i < textBytes.size(); i++) {
        uchar tb = textBytes[i];
        uchar kb = keyBytes[i % keyBytes.size()];
        uchar xb = tb ^ kb;
        textBin += QString("%1 ").arg(tb, 8, 2, QChar('0'));
        keyBin += QString("%1 ").arg(kb, 8, 2, QChar('0'));
        xorBin += QString("%1 ").arg(xb, 8, 2, QChar('0'));
    }

    QByteArray xorBytes;
    for (int i = 0; i < textBytes.size(); i++) {
        xorBytes.append(textBytes[i] ^ keyBytes[i % keyBytes.size()]);
    }

    QColor blue(0, 180, 255);
    QColor orange(255, 140, 50);
    QColor green(0, 230, 120);

    pendingSteps_.append({"明文", {text, textBin.trimmed()}});
    pendingAccents_.append(blue);

    pendingSteps_.append({"密钥 (循环使用)", {key, keyBin.trimmed()}});
    pendingAccents_.append(orange);

    pendingSteps_.append({"XOR 运算: 0⊕0=0, 0⊕1=1, 1⊕0=1, 1⊕1=0",
                           {"逐位异或", xorBin.trimmed()}});
    pendingAccents_.append(QColor(255, 200, 0));

    // Hex 显示
    QString hexResult;
    for (uchar b : xorBytes) {
        if (!hexResult.isEmpty()) hexResult += " ";
        hexResult += QString("%1").arg(b, 2, 16, QChar('0')).toUpper();
    }
    pendingSteps_.append({"密文 (Hex)", {"十六进制表示", hexResult}});
    pendingAccents_.append(green);

    timer_->start(animSpeed_);
}

void XORScene::animateStep() {
    if (currentStep_ >= pendingSteps_.size()) {
        timer_->stop();
        qInfo() << "[XOR] Animation complete";
        emit animationComplete();
        return;
    }
    auto& step = pendingSteps_[currentStep_];
    QColor accent = (currentStep_ < pendingAccents_.size())
                    ? pendingAccents_[currentStep_] : QColor(0, 180, 255);
    addCard(currentStep_ + 1, step.first, step.second.first, step.second.second, accent);
    currentStep_++;
}

void XORScene::reset() {
    timer_->stop();
    clear();
    pendingSteps_.clear();
    pendingAccents_.clear();
    currentStep_ = 0;
}

void XORScene::setSpeed(int ms) {
    animSpeed_ = ms;
    if (timer_->isActive()) timer_->setInterval(ms);
}
