#include "rsascene.h"
#include "../crypto/rsa.h"
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsRectItem>

static const double SCENE_W = 600;
static const double CARD_W = 520;
static const double CARD_H = 60;
static const double CARD_X = (SCENE_W - CARD_W) / 2;
static const double CARD_SPACING = 72;
static const double START_Y = 58;

static QGraphicsDropShadowEffect* createGlowEffect(QColor color, int blur = 15) {
    auto* effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(blur);
    effect->setColor(color);
    effect->setOffset(0);
    return effect;
}

static QGraphicsDropShadowEffect* createShadowEffect() {
    auto* effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(20);
    effect->setColor(QColor(0, 0, 0, 120));
    effect->setOffset(0, 4);
    return effect;
}

RSAScene::RSAScene(QObject* parent)
    : QGraphicsScene(parent), timer_(new QTimer(this)) {
    setSceneRect(0, 0, SCENE_W, 600);
    connect(timer_, &QTimer::timeout, this, &RSAScene::animateStep);
}

void RSAScene::addStep(const QString& desc, const QString& formula, const QString& value,
                       QColor accent) {
    double y = START_Y + steps_.size() * CARD_SPACING;
    int stepNum = steps_.size() + 1;

    // 底层阴影
    auto* shadow = addRect(CARD_X + 2, y + 3, CARD_W, CARD_H,
                           QPen(Qt::NoPen), QBrush(QColor(0, 0, 0, 60)));
    shadow->setZValue(-1);

    // 卡片背景 — 深色渐变感
    QColor bgDark = QColor(accent.red() * 0.15, accent.green() * 0.15, accent.blue() * 0.15, 240);
    QColor bgLight = QColor(accent.red() * 0.08, accent.green() * 0.08, accent.blue() * 0.08, 220);
    auto* bg = addRect(CARD_X, y, CARD_W, CARD_H,
                       QPen(QColor(accent.red(), accent.green(), accent.blue(), 60), 1),
                       QBrush(bgDark));
    bg->setZValue(0);

    // 左侧渐变竖条（宽 5px）
    auto* bar = addRect(CARD_X, y, 5, CARD_H,
                        QPen(Qt::NoPen), QBrush(accent));
    bar->setZValue(1);
    bar->setGraphicsEffect(createGlowEffect(accent, 10));

    // 步骤编号 — 圆形背景
    double numX = CARD_X + 20;
    double numY = y + CARD_H / 2 - 11;
    auto* numBg = addEllipse(numX - 2, numY - 2, 30, 30,
                              QPen(accent, 1.5), QBrush(QColor(0, 0, 0, 150)));
    numBg->setZValue(1);
    auto* numItem = addText(QString("%1").arg(stepNum), QFont("Menlo", 14, QFont::Bold));
    numItem->setPos(numX + 4, numY);
    numItem->setDefaultTextColor(accent);
    numItem->setZValue(2);

    // 描述文字 — 灰白色
    auto* label = addText(desc, QFont("PingFang SC", 16));
    label->setPos(CARD_X + 60, y + 6);
    label->setDefaultTextColor(QColor(220, 220, 230));
    label->setZValue(1);

    // 公式 — 亮蓝色，稍小
    auto* formulaItem = addText(formula, QFont("Menlo", 14, QFont::Bold));
    formulaItem->setPos(CARD_X + 60, y + 32);
    formulaItem->setDefaultTextColor(QColor(80, 200, 255));
    formulaItem->setZValue(1);

    // 结果值 — 右侧，绿色发光
    QGraphicsTextItem* valueItem = nullptr;
    if (!value.isEmpty()) {
        valueItem = addText(value, QFont("Menlo", 20, QFont::Bold));
        double valW = valueItem->boundingRect().width();
        valueItem->setPos(CARD_X + CARD_W - valW - 25, y + 18);
        valueItem->setDefaultTextColor(QColor(0, 255, 140));
        valueItem->setGraphicsEffect(createGlowEffect(QColor(0, 255, 140), 15));
        valueItem->setZValue(1);
    }

    // 流程箭头 — 带发光的细线
    if (steps_.size() > 0) {
        double prevY = START_Y + (steps_.size() - 1) * CARD_SPACING + CARD_H;
        double midX = SCENE_W / 2;
        auto* arrowLine = addLine(midX, prevY + 2, midX, y - 2,
                                   QPen(QColor(accent.red(), accent.green(), accent.blue(), 100), 1.5));
        arrowLine->setZValue(0);
        // 小三角箭头
        auto* head = addPolygon(QPolygonF({
            QPointF(midX - 4, y - 4), QPointF(midX + 4, y - 4), QPointF(midX, y + 2)
        }), QPen(Qt::NoPen), QBrush(QColor(accent.red(), accent.green(), accent.blue(), 180)));
        head->setZValue(1);
    }

    steps_.append({label, formulaItem, valueItem, bg, nullptr});
}

void RSAScene::drawFlowArrow(double y) {}

void RSAScene::startKeyGen(int64_t p, int64_t q) {
    reset();
    qInfo() << "[RSA] Starting key generation, p:" << p << "q:" << q;

    // 标题
    auto* title = addText("RSA 密钥生成", QFont("PingFang SC", 26, QFont::Bold));
    title->setDefaultTextColor(QColor(0, 210, 255));
    title->setGraphicsEffect(createGlowEffect(QColor(0, 210, 255), 20));
    QRectF titleRect = title->boundingRect();
    title->setPos((SCENE_W - titleRect.width()) / 2, 10);

    // 标题下划线
    auto* underline = addLine((SCENE_W - titleRect.width()) / 2, 48,
                               (SCENE_W + titleRect.width()) / 2, 48,
                               QPen(QColor(0, 210, 255, 80), 1));
    underline->setZValue(0);

    auto steps = RSAEngine::keyGenSteps(p, q);
    QColor calcColor(0, 180, 255);    // 蓝色 - 计算
    QColor privColor(255, 180, 0);    // 金色 - 私钥
    QColor pubColor(0, 230, 120);     // 绿色 - 公钥

    for (int i = 0; i < steps.size(); i++) {
        const auto& step = steps[i];
        QString val;
        if (i == 0) {
            val = QString("p=%1, q=%2").arg(p).arg(q);
        } else if (i < steps.size() - 2) {
            val = QString::number(step.value);
        }
        pendingSteps_.append({step.description, {step.formula, val}});
        if (i >= steps.size() - 2) {
            pendingAccents_.append(i == steps.size() - 1 ? privColor : pubColor);
        } else {
            pendingAccents_.append(calcColor);
        }
    }

    timer_->start(animSpeed_);
}

void RSAScene::startEncrypt(int64_t message, int64_t e, int64_t n) {
    reset();

    auto* title = addText("RSA 加密", QFont("PingFang SC", 26, QFont::Bold));
    title->setDefaultTextColor(QColor(0, 210, 255));
    title->setGraphicsEffect(createGlowEffect(QColor(0, 210, 255), 20));
    QRectF titleRect = title->boundingRect();
    title->setPos((SCENE_W - titleRect.width()) / 2, 10);

    auto* underline = addLine((SCENE_W - titleRect.width()) / 2, 48,
                               (SCENE_W + titleRect.width()) / 2, 48,
                               QPen(QColor(0, 210, 255, 80), 1));

    int64_t cipher = RSAEngine::encrypt(message, e, n);
    pendingSteps_.append({"明文 M", {QString("M = %1").arg(message), ""}});
    pendingAccents_.append(QColor(0, 180, 255));
    pendingSteps_.append({"公钥 (e, n)", {QString("e = %1, n = %2").arg(e).arg(n), ""}});
    pendingAccents_.append(QColor(0, 230, 120));
    pendingSteps_.append({"加密: C = M^e mod n",
                           {QString("C = %1^%2 mod %3").arg(message).arg(e).arg(n),
                            QString("= %1").arg(cipher)}});
    pendingAccents_.append(QColor(255, 120, 50));

    timer_->start(animSpeed_);
}

void RSAScene::startDecrypt(int64_t cipher, int64_t d, int64_t n) {
    reset();

    auto* title = addText("RSA 解密", QFont("PingFang SC", 26, QFont::Bold));
    title->setDefaultTextColor(QColor(0, 210, 255));
    title->setGraphicsEffect(createGlowEffect(QColor(0, 210, 255), 20));
    QRectF titleRect = title->boundingRect();
    title->setPos((SCENE_W - titleRect.width()) / 2, 10);

    auto* underline = addLine((SCENE_W - titleRect.width()) / 2, 48,
                               (SCENE_W + titleRect.width()) / 2, 48,
                               QPen(QColor(0, 210, 255, 80), 1));

    int64_t message = RSAEngine::decrypt(cipher, d, n);
    pendingSteps_.append({"密文 C", {QString("C = %1").arg(cipher), ""}});
    pendingAccents_.append(QColor(255, 120, 50));
    pendingSteps_.append({"私钥 (d, n)", {QString("d = %1, n = %2").arg(d).arg(n), ""}});
    pendingAccents_.append(QColor(255, 180, 0));
    pendingSteps_.append({"解密: M = C^d mod n",
                           {QString("M = %1^%2 mod %3").arg(cipher).arg(d).arg(n),
                            QString("= %1").arg(message)}});
    pendingAccents_.append(QColor(0, 230, 120));

    timer_->start(animSpeed_);
}

void RSAScene::animateStep() {
    if (currentStep_ >= pendingSteps_.size()) {
        timer_->stop();
        qInfo() << "[RSA] Animation complete";
        emit animationComplete();
        return;
    }

    auto& step = pendingSteps_[currentStep_];
    QColor accent = (currentStep_ < pendingAccents_.size())
                    ? pendingAccents_[currentStep_]
                    : QColor(0, 180, 255);
    addStep(step.first, step.second.first, step.second.second, accent);
    qDebug() << "[RSA] Step" << currentStep_ << ":" << step.first;
    currentStep_++;
}

void RSAScene::reset() {
    timer_->stop();
    clear();
    steps_.clear();
    pendingSteps_.clear();
    pendingAccents_.clear();
    currentStep_ = 0;
}

void RSAScene::setSpeed(int ms) {
    animSpeed_ = ms;
    if (timer_->isActive()) {
        timer_->setInterval(ms);
    }
}
