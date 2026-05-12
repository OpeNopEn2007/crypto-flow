#include "base64scene.h"
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QDebug>
#include <QGraphicsDropShadowEffect>

static const double SW = 600;
static const double CW = 480;
static const double CH = 56;
static const double CX = (SW - CW) / 2;
static const double SP = 62;
static const double SY = 45;

static const QString B64_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static QGraphicsDropShadowEffect* g(QColor c, int b = 15) {
    auto* e = new QGraphicsDropShadowEffect;
    e->setBlurRadius(b);
    e->setColor(c);
    e->setOffset(0);
    return e;
}

Base64Scene::Base64Scene(QObject* parent)
    : QGraphicsScene(parent), timer_(new QTimer(this)) {
    setSceneRect(0, 0, SW, 500);
    connect(timer_, &QTimer::timeout, this, &Base64Scene::animateStep);
}

void Base64Scene::addCard(int step, const QString& title, const QString& detail,
                           const QString& value, QColor accent) {
    double y = SY + (step - 1) * SP;

    // 阴影
    addRect(CX + 2, y + 3, CW, CH, QPen(Qt::NoPen), QBrush(QColor(0, 0, 0, 50)))->setZValue(-1);

    // 卡片
    addRect(CX, y, CW, CH,
            QPen(QColor(accent.red(), accent.green(), accent.blue(), 60), 1),
            QBrush(QColor(20, 28, 40, 240)))->setZValue(0);

    // 左侧竖条
    auto* bar = addRect(CX, y, 5, CH, QPen(Qt::NoPen), QBrush(accent));
    bar->setZValue(1);
    bar->setGraphicsEffect(g(accent, 10));

    // 编号
    auto* num = addText(QString("#%1").arg(step), QFont("Menlo", 10, QFont::Bold));
    num->setPos(CX + 14, y + 5);
    num->setDefaultTextColor(accent);
    num->setZValue(1);

    // 标题
    auto* t = addText(title, QFont("PingFang SC", 11));
    t->setPos(CX + 50, y + 5);
    t->setDefaultTextColor(QColor(220, 220, 230));
    t->setZValue(1);

    // 详情
    auto* d = addText(detail, QFont("Menlo", 10));
    d->setPos(CX + 50, y + 26);
    d->setDefaultTextColor(QColor(80, 180, 255));
    d->setZValue(1);

    // 值
    if (!value.isEmpty()) {
        auto* v = addText(value, QFont("Menlo", 13, QFont::Bold));
        double vw = v->boundingRect().width();
        v->setPos(CX + CW - vw - 20, y + CH / 2 - 10);
        v->setDefaultTextColor(QColor(0, 255, 140));
        v->setGraphicsEffect(g(QColor(0, 255, 140), 12));
        v->setZValue(1);
    }

    // 箭头
    if (step > 1) {
        double prevY = SY + (step - 2) * SP + CH;
        double mx = SW / 2;
        addLine(mx, prevY + 2, mx, y - 2,
                QPen(QColor(accent.red(), accent.green(), accent.blue(), 100), 1.5))->setZValue(0);
        addPolygon(QPolygonF({QPointF(mx-4, y-4), QPointF(mx+4, y-4), QPointF(mx, y+2)}),
                   QPen(Qt::NoPen), QBrush(QColor(accent.red(), accent.green(), accent.blue(), 180)))->setZValue(1);
    }
}

void Base64Scene::startAnimation(const QString& text) {
    reset();

    auto* title = addText("Base64 编码", QFont("PingFang SC", 18, QFont::Bold));
    title->setDefaultTextColor(QColor(0, 210, 255));
    title->setGraphicsEffect(g(QColor(0, 210, 255), 20));
    QRectF tr = title->boundingRect();
    title->setPos((SW - tr.width()) / 2, 10);
    addLine((SW - tr.width()) / 2, 38, (SW + tr.width()) / 2, 38,
            QPen(QColor(0, 210, 255, 80), 1));

    QByteArray bytes = text.toUtf8();
    QString asciiBin;
    for (uchar b : bytes) {
        asciiBin += QString("%1").arg(b, 8, 2, QChar('0'));
    }

    // 补零到 6 的倍数
    while (asciiBin.size() % 6 != 0) asciiBin += '0';

    QString b64Result;
    QVector<QString> groups;
    for (int i = 0; i < asciiBin.size(); i += 6) {
        QString group = asciiBin.mid(i, 6);
        int val = 0;
        for (int j = 0; j < 6; j++) {
            val = val * 2 + (group[j] == '1' ? 1 : 0);
        }
        groups.append(group);
        b64Result += B64_CHARS[val];
    }

    // 补 =
    while (b64Result.size() % 4 != 0) b64Result += '=';

    QColor blue(0, 180, 255);
    QColor green(0, 230, 120);
    QColor orange(255, 160, 50);

    // Step 1: 原始文本
    pendingSteps_.append({"原始文本 (UTF-8)", {text, ""}});
    pendingAccents_.append(blue);

    // Step 2: 转二进制
    QString binDisplay;
    for (int i = 0; i < asciiBin.size(); i += 8) {
        if (i > 0) binDisplay += " ";
        binDisplay += asciiBin.mid(i, 8);
    }
    pendingSteps_.append({"转为二进制", {"8bit/字节", binDisplay}});
    pendingAccents_.append(blue);

    // Step 3: 分组为 6bit
    QString groupDisplay;
    for (int i = 0; i < groups.size(); i++) {
        if (i > 0) groupDisplay += " ";
        groupDisplay += groups[i];
    }
    pendingSteps_.append({"6bit 分组", {QString("%1 组").arg(groups.size()), groupDisplay}});
    pendingAccents_.append(orange);

    // Step 4: 查表映射
    QString mapDisplay;
    for (int i = 0; i < groups.size(); i++) {
        int val = 0;
        for (int j = 0; j < 6; j++) {
            val = val * 2 + (groups[i][j] == '1' ? 1 : 0);
        }
        if (i > 0) mapDisplay += " ";
        mapDisplay += QString("%1→%2").arg(val).arg(B64_CHARS[val]);
    }
    pendingSteps_.append({"Base64 查表", {"0-63 → A-Z,a-z,0-9,+,/", mapDisplay}});
    pendingAccents_.append(green);

    // Step 5: 结果
    pendingSteps_.append({"编码结果", {"Base64", b64Result}});
    pendingAccents_.append(QColor(0, 255, 150));

    totalSteps_ = pendingSteps_.size();
    timer_->start(animSpeed_);
}

void Base64Scene::animateStep() {
    if (currentStep_ >= pendingSteps_.size()) {
        timer_->stop();
        qInfo() << "[Base64] Animation complete";
        emit animationComplete();
        return;
    }
    auto& step = pendingSteps_[currentStep_];
    QColor accent = (currentStep_ < pendingAccents_.size())
                    ? pendingAccents_[currentStep_] : QColor(0, 180, 255);
    addCard(currentStep_ + 1, step.first, step.second.first, step.second.second, accent);
    currentStep_++;
}

void Base64Scene::reset() {
    timer_->stop();
    clear();
    pendingSteps_.clear();
    pendingAccents_.clear();
    currentStep_ = 0;
}

void Base64Scene::setSpeed(int ms) {
    animSpeed_ = ms;
    if (timer_->isActive()) timer_->setInterval(ms);
}
