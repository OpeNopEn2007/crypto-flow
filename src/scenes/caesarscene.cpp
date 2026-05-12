#include "caesarscene.h"
#include <QGraphicsDropShadowEffect>
#include <QFont>
#include <QtMath>
#include <QDebug>

static const double RADIUS_OUTER = 180;
static const double RADIUS_INNER = 120;
static const double CENTER_X = 300;
static const double CENTER_Y = 250;

static QGraphicsDropShadowEffect* createGlowEffect(QColor color, int blur = 15) {
    auto* effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(blur);
    effect->setColor(color);
    effect->setOffset(0);
    return effect;
}

CaesarScene::CaesarScene(QObject* parent)
    : QGraphicsScene(parent)
    , rotateTimer_(new QTimer(this))
    , highlightTimer_(new QTimer(this))
    , pulseTimer_(new QTimer(this)) {
    setSceneRect(0, 0, 600, 500);
    connect(rotateTimer_, &QTimer::timeout, this, &CaesarScene::animateRotation);
    connect(highlightTimer_, &QTimer::timeout, this, &CaesarScene::animateHighlight);
    connect(pulseTimer_, &QTimer::timeout, this, &CaesarScene::animatePulse);
}

void CaesarScene::drawOuterRing() {
    // 渐变圆环
    outerRing_ = addEllipse(
        CENTER_X - RADIUS_OUTER, CENTER_Y - RADIUS_OUTER,
        RADIUS_OUTER * 2, RADIUS_OUTER * 2,
        QPen(QColor(0, 200, 255), 3));
    outerRing_->setZValue(0);
    outerRing_->setGraphicsEffect(createGlowEffect(QColor(0, 200, 255), 25));

    QString alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < 26; i++) {
        double angle = (i * 360.0 / 26 - 90) * M_PI / 180;
        double x = CENTER_X + RADIUS_OUTER * 0.85 * cos(angle) - 8;
        double y = CENTER_Y + RADIUS_OUTER * 0.85 * sin(angle) - 10;
        auto* letter = addText(QString(alpha[i]), QFont("Menlo", 16, QFont::Bold));
        letter->setPos(x, y);
        letter->setDefaultTextColor(QColor(0, 200, 255));
        letter->setZValue(1);
        outerLetters_.append(letter);
    }
}

void CaesarScene::drawInnerRing() {
    innerRing_ = addEllipse(
        CENTER_X - RADIUS_INNER, CENTER_Y - RADIUS_INNER,
        RADIUS_INNER * 2, RADIUS_INNER * 2,
        QPen(QColor(255, 100, 50), 3));
    innerRing_->setZValue(0);
    innerRing_->setGraphicsEffect(createGlowEffect(QColor(255, 100, 50), 18));
    innerRing_->setTransformOriginPoint(CENTER_X, CENTER_Y);

    QString alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < 26; i++) {
        double angle = (i * 360.0 / 26 - 90) * M_PI / 180;
        double x = CENTER_X + RADIUS_INNER * 0.8 * cos(angle) - 8;
        double y = CENTER_Y + RADIUS_INNER * 0.8 * sin(angle) - 10;
        auto* letter = addText(QString(alpha[i]), QFont("Menlo", 16, QFont::Bold));
        letter->setPos(x, y);
        letter->setDefaultTextColor(QColor(255, 100, 50));
        letter->setZValue(1);
        innerLetters_.append(letter);
    }
}

void CaesarScene::setInnerRingRotation(double deg) {
    innerRing_->setRotation(deg);
    for (int i = 0; i < 26; i++) {
        double angle = (i * 360.0 / 26 - 90 + deg) * M_PI / 180;
        double x = CENTER_X + RADIUS_INNER * 0.8 * cos(angle) - 8;
        double y = CENTER_Y + RADIUS_INNER * 0.8 * sin(angle) - 10;
        innerLetters_[i]->setPos(x, y);
    }
}

void CaesarScene::startAnimation(const QString& text, int shift) {
    reset();
    inputText_ = text.toUpper();
    shift_ = ((shift % 26) + 26) % 26;
    qInfo() << "[Caesar] Starting animation, text:" << inputText_ << "shift:" << shift_;

    drawOuterRing();
    drawInnerRing();

    resultText_ = addText("", QFont("Menlo", 22, QFont::Bold));
    resultText_->setDefaultTextColor(QColor(0, 255, 150));
    resultText_->setZValue(5);
    resultText_->setGraphicsEffect(createGlowEffect(QColor(0, 255, 150), 20));
    resultText_->setVisible(false);

    // 启动脉冲动画
    pulsePhase_ = 0;
    pulseTimer_->start(50);

    // 阶段1：缓动旋转
    currentRotation_ = 0;
    targetRotation_ = -shift_ * (360.0 / 26);
    rotateCurrentStep_ = 0;
    rotateTotalSteps_ = 40;

    qInfo() << "[Caesar] Rotation phase: target" << targetRotation_ << "degrees";
    rotateTimer_->start(35);
}

void CaesarScene::animateRotation() {
    rotateCurrentStep_++;

    // OutCubic 缓动：先快后慢
    double t = static_cast<double>(rotateCurrentStep_) / rotateTotalSteps_;
    QEasingCurve curve(QEasingCurve::OutCubic);
    double eased = curve.valueForProgress(qMin(t, 1.0));
    currentRotation_ = targetRotation_ * eased;

    if (rotateCurrentStep_ >= rotateTotalSteps_) {
        currentRotation_ = targetRotation_;
        rotateTimer_->stop();
    }

    setInnerRingRotation(currentRotation_);

    if (!rotateTimer_->isActive()) {
        qInfo() << "[Caesar] Rotation complete, now highlighting letters";
        highlightIndex_ = 0;
        highlightShowing_ = false;
        highlightTimer_->start(animSpeed_);
    }
}

void CaesarScene::animateHighlight() {
    if (highlightShowing_) {
        clearHighlights();
        clearConnectionLines();
        highlightShowing_ = false;
        highlightIndex_++;
        highlightTimer_->setInterval(150);
        return;
    }

    if (highlightIndex_ >= inputText_.size()) {
        highlightTimer_->stop();
        pulseTimer_->stop();

        // 最终脉冲：恢复静态发光
        outerRing_->setGraphicsEffect(createGlowEffect(QColor(0, 200, 255), 25));
        innerRing_->setGraphicsEffect(createGlowEffect(QColor(255, 100, 50), 18));

        QRectF rect = resultText_->boundingRect();
        resultText_->setPos(CENTER_X - rect.width() / 2, CENTER_Y - rect.height() / 2);
        resultText_->setVisible(true);
        qInfo() << "[Caesar] Animation complete, result:" << resultText_->toPlainText();
        emit animationComplete();
        return;
    }

    QChar ch = inputText_[highlightIndex_];
    if (ch.isLetter()) {
        int outerIdx = ch.unicode() - 'A';
        int innerIdx = (outerIdx + shift_) % 26;
        highlightPair(outerIdx, innerIdx);
        drawConnectionLine(outerIdx, innerIdx);
        QChar encrypted = QChar('A' + innerIdx);
        resultText_->setPlainText(resultText_->toPlainText() + encrypted);
        qDebug() << "[Caesar] Highlight" << highlightIndex_ << ":" << ch << "->" << encrypted;
    } else {
        resultText_->setPlainText(resultText_->toPlainText() + ch);
        highlightIndex_++;
        return;
    }

    highlightShowing_ = true;
    highlightTimer_->setInterval(animSpeed_);
}

void CaesarScene::animatePulse() {
    pulsePhase_ += 0.08;
    double pulse = 0.5 + 0.5 * sin(pulsePhase_);

    // 圆环发光脉冲
    int outerGlow = static_cast<int>(outerGlowBase_ + pulse * 10);
    int innerGlow = static_cast<int>(innerGlowBase_ + pulse * 8);
    outerRing_->setGraphicsEffect(createGlowEffect(QColor(0, 200, 255), outerGlow));
    innerRing_->setGraphicsEffect(createGlowEffect(QColor(255, 100, 50), innerGlow));
}

void CaesarScene::highlightPair(int outerIdx, int innerIdx) {
    clearHighlights();

    if (outerIdx >= 0 && outerIdx < outerLetters_.size()) {
        outerLetters_[outerIdx]->setDefaultTextColor(QColor(255, 255, 0));
        outerLetters_[outerIdx]->setGraphicsEffect(createGlowEffect(QColor(255, 255, 0), 30));
    }
    if (innerIdx >= 0 && innerIdx < innerLetters_.size()) {
        innerLetters_[innerIdx]->setDefaultTextColor(QColor(255, 255, 0));
        innerLetters_[innerIdx]->setGraphicsEffect(createGlowEffect(QColor(255, 255, 0), 30));
    }
}

void CaesarScene::clearHighlights() {
    for (auto* letter : outerLetters_) {
        letter->setDefaultTextColor(QColor(0, 200, 255));
        letter->setGraphicsEffect(nullptr);
    }
    for (auto* letter : innerLetters_) {
        letter->setDefaultTextColor(QColor(255, 100, 50));
        letter->setGraphicsEffect(nullptr);
    }
}

void CaesarScene::drawConnectionLine(int outerIdx, int innerIdx) {
    if (outerIdx < 0 || outerIdx >= outerLetters_.size()) return;
    if (innerIdx < 0 || innerIdx >= innerLetters_.size()) return;

    // 外圈字母中心
    double outerAngle = (outerIdx * 360.0 / 26 - 90) * M_PI / 180;
    double ox = CENTER_X + RADIUS_OUTER * 0.85 * cos(outerAngle);
    double oy = CENTER_Y + RADIUS_OUTER * 0.85 * sin(outerAngle);

    // 内圈字母中心（考虑旋转）
    double innerAngle = (innerIdx * 360.0 / 26 - 90 + currentRotation_) * M_PI / 180;
    double ix = CENTER_X + RADIUS_INNER * 0.8 * cos(innerAngle);
    double iy = CENTER_Y + RADIUS_INNER * 0.8 * sin(innerAngle);

    auto* line = addLine(ox, oy, ix, iy,
                          QPen(QColor(255, 255, 0, 180), 2, Qt::DashLine));
    line->setZValue(0);
    connectionLines_.append(line);
}

void CaesarScene::clearConnectionLines() {
    for (auto* line : connectionLines_) {
        removeItem(line);
        delete line;
    }
    connectionLines_.clear();
}

void CaesarScene::reset() {
    rotateTimer_->stop();
    highlightTimer_->stop();
    pulseTimer_->stop();
    clear();
    outerLetters_.clear();
    innerLetters_.clear();
    clearConnectionLines();
    outerRing_ = nullptr;
    innerRing_ = nullptr;
    resultText_ = nullptr;
    currentRotation_ = 0;
    highlightIndex_ = 0;
    highlightShowing_ = false;
}

void CaesarScene::setSpeed(int ms) {
    animSpeed_ = ms;
    if (highlightTimer_->isActive()) {
        highlightTimer_->setInterval(ms);
    }
}
