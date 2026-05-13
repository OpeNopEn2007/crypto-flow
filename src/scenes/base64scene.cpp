#include "base64scene.h"
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QDebug>
#include <QGraphicsDropShadowEffect>

static const double SCENE_W = 600;
static const double CARD_W = 520;
static const double CARD_H = 80;
static const double CARD_X = (SCENE_W - CARD_W) / 2;
static const double CARD_SPACING = 95;
static const double START_Y = 65;
static const QString B64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static QGraphicsDropShadowEffect* createGlowEffect(QColor color, int blur = 15) {
    auto* effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(blur);
    effect->setColor(color);
    effect->setOffset(0);
    return effect;
}

Base64Scene::Base64Scene(QObject* parent)
    : QGraphicsScene(parent), timer_(new QTimer(this)) {
    setSceneRect(0, 0, SCENE_W, 500);
    connect(timer_, &QTimer::timeout, this, &Base64Scene::animateStep);
}

void Base64Scene::addStep(const QString& desc, const QString& formula, const QString& value,
                           QColor accent) {
    double y = START_Y + steps_.size() * CARD_SPACING;
    int stepNum = steps_.size() + 1;

    auto* shadow = addRect(CARD_X + 2, y + 3, CARD_W, CARD_H,
                           QPen(Qt::NoPen), QBrush(QColor(0, 0, 0, 60)));
    shadow->setZValue(-1);

    QColor bgDark = QColor(accent.red() * 0.15, accent.green() * 0.15, accent.blue() * 0.15, 240);
    auto* bg = addRect(CARD_X, y, CARD_W, CARD_H,
                       QPen(QColor(accent.red(), accent.green(), accent.blue(), 60), 1),
                       QBrush(bgDark));
    bg->setZValue(0);

    auto* bar = addRect(CARD_X, y, 5, CARD_H,
                        QPen(Qt::NoPen), QBrush(accent));
    bar->setZValue(1);
    bar->setGraphicsEffect(createGlowEffect(accent, 10));

    double numX = CARD_X + 20;
    double numY = y + CARD_H / 2 - 12;
    auto* numBg = addEllipse(numX - 2, numY - 2, 30, 30,
                              QPen(accent, 1.5), QBrush(QColor(0, 0, 0, 150)));
    numBg->setZValue(1);
    auto* numItem = addText(QString("%1").arg(stepNum), QFont("Menlo", 14, QFont::Bold));
    numItem->setPos(numX + 4, numY);
    numItem->setDefaultTextColor(accent);
    numItem->setZValue(2);

    auto* label = addText(desc, QFont("PingFang SC", 16));
    label->setPos(CARD_X + 60, y + 8);
    label->setDefaultTextColor(QColor(220, 220, 230));
    label->setZValue(1);

    auto* formulaItem = addText(formula, QFont("Menlo", 13));
    formulaItem->setPos(CARD_X + 60, y + 38);
    formulaItem->setDefaultTextColor(QColor(80, 200, 255));
    formulaItem->setZValue(1);

    QGraphicsTextItem* valueItem = nullptr;
    if (!value.isEmpty()) {
        int vFontSize = value.size() > 20 ? 13 : 20;
        QFont vFont("Menlo", vFontSize, QFont::Bold);
        valueItem = addText(value, vFont);
        double valW = valueItem->boundingRect().width();
        valueItem->setPos(CARD_X + CARD_W - valW - 25, y + 22);
        valueItem->setDefaultTextColor(QColor(0, 255, 140));
        valueItem->setGraphicsEffect(createGlowEffect(QColor(0, 255, 140), 15));
        valueItem->setZValue(1);
    }

    if (steps_.size() > 0) {
        double prevY = START_Y + (steps_.size() - 1) * CARD_SPACING + CARD_H;
        double midX = SCENE_W / 2;
        auto* arrowLine = addLine(midX, prevY + 2, midX, y - 2,
                                   QPen(QColor(accent.red(), accent.green(), accent.blue(), 100), 1.5));
        arrowLine->setZValue(0);
        auto* head = addPolygon(QPolygonF({
            QPointF(midX - 4, y - 4), QPointF(midX + 4, y - 4), QPointF(midX, y + 2)
        }), QPen(Qt::NoPen), QBrush(QColor(accent.red(), accent.green(), accent.blue(), 180)));
        head->setZValue(1);
    }

    steps_.append({label, formulaItem, valueItem, bg, nullptr});
}

void Base64Scene::startAnimation(const QString& text) {
    reset();

    auto* title = addText("Base64 编码", QFont("PingFang SC", 26, QFont::Bold));
    title->setDefaultTextColor(QColor(0, 210, 255));
    title->setGraphicsEffect(createGlowEffect(QColor(0, 210, 255), 20));
    QRectF titleRect = title->boundingRect();
    title->setPos((SCENE_W - titleRect.width()) / 2, 10);

    auto* underline = addLine((SCENE_W - titleRect.width()) / 2, 48,
                               (SCENE_W + titleRect.width()) / 2, 48,
                               QPen(QColor(0, 210, 255, 80), 1));
    underline->setZValue(0);

    QByteArray inputBytes = text.toUtf8();

    // Step 1: 明文输入
    pendingSteps_.append({"明文输入", {"", text}});
    pendingAccents_.append(QColor(0, 180, 255));

    // Step 2: 转为二进制
    QString binDetail;
    for (int i = 0; i < inputBytes.size(); i++) {
        if (i > 0) binDetail += " ";
        binDetail += QString("%1").arg((uchar)inputBytes[i], 8, 2, QChar('0'));
    }
    pendingSteps_.append({"转为二进制",
                           {binDetail, ""}});
    pendingAccents_.append(QColor(0, 200, 255));

    // Step 3: 8bit → 6bit 分组
    QString padded;
    for (uchar b : inputBytes) padded += QString("%1").arg(b, 8, 2, QChar('0'));
    while (padded.size() % 6 != 0) padded += '0';

    QVector<QString> groups;
    for (int i = 0; i < padded.size(); i += 6)
        groups.append(padded.mid(i, 6));

    QString groupDetail;
    for (int i = 0; i < groups.size(); i++) {
        if (i > 0) groupDetail += " ";
        groupDetail += groups[i];
    }
    pendingSteps_.append({"8bit → 6bit 分组",
                           {groupDetail, ""}});
    pendingAccents_.append(QColor(255, 180, 60));

    // Step 4: 查表映射
    QString mapDetail;
    QString result;
    for (int i = 0; i < groups.size(); i++) {
        int val = 0;
        for (int j = 0; j < 6; j++) val = val * 2 + (groups[i][j] == '1' ? 1 : 0);
        QChar ch = B64[val];
        result += ch;
        if (i > 0) mapDetail += "  ";
        mapDetail += QString("%1→%2").arg(val).arg(ch);
    }
    while (result.size() % 4 != 0) result += '=';

    pendingSteps_.append({"查表映射",
                           {mapDetail, ""}});
    pendingAccents_.append(QColor(255, 200, 80));

    // Step 5: 编码结果
    pendingSteps_.append({"编码结果",
                           {text, result}});
    pendingAccents_.append(QColor(0, 255, 140));

    qInfo() << "[Base64]" << text << "->" << result;
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
                    ? pendingAccents_[currentStep_]
                    : QColor(0, 180, 255);
    addStep(step.first, step.second.first, step.second.second, accent);
    currentStep_++;
}

void Base64Scene::reset() {
    timer_->stop();
    clear();
    steps_.clear();
    pendingSteps_.clear();
    pendingAccents_.clear();
    currentStep_ = 0;
}

void Base64Scene::setSpeed(int ms) {
    animSpeed_ = ms;
    if (timer_->isActive()) timer_->setInterval(ms);
}
