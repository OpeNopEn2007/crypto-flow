#include "caesarscene.h"
#include <QGraphicsDropShadowEffect>
#include <QFont>
#include <QtMath>
#include <QDebug>

static const double RADIUS_OUTER = 180;
static const double RADIUS_INNER = 120;
static const double CENTER_X = 300;
static const double CENTER_Y = 250;
static const double DEG_PER_LETTER = 360.0 / 26;

static QGraphicsDropShadowEffect* createGlowEffect(QColor color, int blur = 15) {
    auto* effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(blur);
    effect->setColor(color);
    effect->setOffset(0);
    return effect;
}

CaesarScene::CaesarScene(QObject* parent)
    : QGraphicsScene(parent)
    , stepTimer_(new QTimer(this))
    , stepAnimTimer_(new QTimer(this))
    , highlightTimer_(new QTimer(this))
    , pulseTimer_(new QTimer(this))
    , connectionAnimTimer_(new QTimer(this)) {
    setSceneRect(0, 0, 600, 500);
    connect(stepTimer_, &QTimer::timeout, this, &CaesarScene::onStepTick);
    connect(stepAnimTimer_, &QTimer::timeout, this, &CaesarScene::onStepAnimTick);
    connect(highlightTimer_, &QTimer::timeout, this, &CaesarScene::animateHighlight);
    connect(pulseTimer_, &QTimer::timeout, this, &CaesarScene::animatePulse);
    connect(connectionAnimTimer_, &QTimer::timeout, this, &CaesarScene::animateConnectionLine);
}

void CaesarScene::drawOuterRing() {
    outerRing_ = addEllipse(
        CENTER_X - RADIUS_OUTER, CENTER_Y - RADIUS_OUTER,
        RADIUS_OUTER * 2, RADIUS_OUTER * 2,
        QPen(QColor(0, 200, 255), 3));
    outerRing_->setZValue(0);
    outerRing_->setGraphicsEffect(createGlowEffect(QColor(0, 200, 255), 25));

    for (int i = 0; i < 26; i++) {
        double angle = (i * DEG_PER_LETTER - 90) * M_PI / 180;
        double x1 = CENTER_X + RADIUS_OUTER * 0.95 * cos(angle);
        double y1 = CENTER_Y + RADIUS_OUTER * 0.95 * sin(angle);
        double x2 = CENTER_X + RADIUS_OUTER * 1.02 * cos(angle);
        double y2 = CENTER_Y + RADIUS_OUTER * 1.02 * sin(angle);
        auto* tick = addLine(x1, y1, x2, y2, QPen(QColor(0, 200, 255, 120), 1));
        tick->setZValue(0);
    }

    QString alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < 26; i++) {
        double angle = (i * DEG_PER_LETTER - 90) * M_PI / 180;
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

    for (int i = 0; i < 26; i++) {
        double angle = (i * DEG_PER_LETTER - 90) * M_PI / 180;
        double x1 = CENTER_X + RADIUS_INNER * 0.92 * cos(angle);
        double y1 = CENTER_Y + RADIUS_INNER * 0.92 * sin(angle);
        double x2 = CENTER_X + RADIUS_INNER * 0.98 * cos(angle);
        double y2 = CENTER_Y + RADIUS_INNER * 0.98 * sin(angle);
        auto* tick = addLine(x1, y1, x2, y2, QPen(QColor(255, 100, 50, 100), 1));
        tick->setZValue(0);
    }

    QString alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < 26; i++) {
        double angle = (i * DEG_PER_LETTER - 90) * M_PI / 180;
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
        double angle = (i * DEG_PER_LETTER - 90 + deg) * M_PI / 180;
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

    auto* outerLabel = addText("明文", QFont("PingFang SC", 10));
    outerLabel->setDefaultTextColor(QColor(0, 200, 255, 180));
    outerLabel->setPos(CENTER_X + RADIUS_OUTER + 5, CENTER_Y - 10);
    outerLabel->setZValue(2);

    auto* innerLabel = addText("密文", QFont("PingFang SC", 10));
    innerLabel->setDefaultTextColor(QColor(255, 100, 50, 180));
    innerLabel->setPos(CENTER_X + RADIUS_INNER + 5, CENTER_Y - 10);
    innerLabel->setZValue(2);

    resultText_ = addText("", QFont("Menlo", 22, QFont::Bold));
    resultText_->setDefaultTextColor(QColor(0, 255, 150));
    resultText_->setZValue(5);
    resultText_->setGraphicsEffect(createGlowEffect(QColor(0, 255, 150), 20));
    resultText_->setVisible(false);
    resultTarget_ = "";
    resultCharIndex_ = 0;
    typewriterActive_ = false;

    pulsePhase_ = 0;
    pulseTimer_->start(50);

    // 逐格旋转设置
    currentRotation_ = 0;
    rotateStepIndex_ = 0;
    rotateStepTotal_ = shift_;
    stepFromDeg_ = 0;

    qInfo() << "[Caesar] Step rotation: " << rotateStepTotal_ << " steps,"
            << "interval:" << STEP_INTERVAL_MS << "ms";

    // 1 秒延迟后开始第一格
    animationId_++;
    int myId = animationId_;
    QTimer::singleShot(1000, this, [this, myId]() {
        if (myId == animationId_) {
            onStepTick(); // 立即执行第一格
            if (rotateStepTotal_ > 0) {
                stepTimer_->start(STEP_INTERVAL_MS);
            }
        }
    });
}

void CaesarScene::onStepTick() {
    if (rotateStepIndex_ >= rotateStepTotal_) {
        stepTimer_->stop();
        qInfo() << "[Caesar] Step rotation complete, now highlighting letters";
        highlightIndex_ = 0;
        highlightShowing_ = false;
        highlightTimer_->start(animSpeed_);
        return;
    }

    // 计算当前格的起止角度
    stepFromDeg_ = currentRotation_;
    stepToDeg_ = -(rotateStepIndex_ + 1) * DEG_PER_LETTER;
    stepAnimFrame_ = 0;

    rotateStepIndex_++;
    qDebug() << "[Caesar] Step" << rotateStepIndex_ << "/" << rotateStepTotal_
             << ": " << stepFromDeg_ << "->" << stepToDeg_ << "deg";

    // 启动格内过渡动画
    stepAnimTimer_->start(20);
}

void CaesarScene::onStepAnimTick() {
    stepAnimFrame_++;
    double t = static_cast<double>(stepAnimFrame_) / STEP_ANIM_FRAMES;

    // OutCubic 缓动：先快后慢，有"咔哒"感
    QEasingCurve curve(QEasingCurve::OutCubic);
    double eased = curve.valueForProgress(qMin(t, 1.0));

    currentRotation_ = stepFromDeg_ + (stepToDeg_ - stepFromDeg_) * eased;

    if (stepAnimFrame_ >= STEP_ANIM_FRAMES) {
        currentRotation_ = stepToDeg_;
        stepAnimTimer_->stop();
    }

    setInnerRingRotation(currentRotation_);
}

void CaesarScene::animateHighlight() {
    if (typewriterActive_) {
        if (resultCharIndex_ < resultTarget_.size()) {
            resultCharIndex_++;
            resultText_->setPlainText(resultTarget_.left(resultCharIndex_));
            QRectF rect = resultText_->boundingRect();
            resultText_->setPos(CENTER_X - rect.width() / 2, CENTER_Y - rect.height() / 2);
        } else {
            highlightTimer_->stop();
            typewriterActive_ = false;
            qInfo() << "[Caesar] Animation complete, result:" << resultText_->toPlainText();
            emit animationComplete();
        }
        return;
    }

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

        outerRing_->setGraphicsEffect(createGlowEffect(QColor(0, 200, 255), 25));
        innerRing_->setGraphicsEffect(createGlowEffect(QColor(255, 100, 50), 18));

        resultTarget_ = resultText_->toPlainText();
        resultCharIndex_ = 0;
        resultText_->setPlainText("");
        resultText_->setVisible(true);
        QRectF rect = resultText_->boundingRect();
        resultText_->setPos(CENTER_X - rect.width() / 2, CENTER_Y - rect.height() / 2);

        typewriterActive_ = true;
        highlightTimer_->start(80);
        return;
    }

    QChar ch = inputText_[highlightIndex_];
    if (ch.isLetter()) {
        int outerIdx = ch.unicode() - 'A';
        int innerIdx = (outerIdx + shift_) % 26;
        highlightPair(outerIdx, innerIdx);
        startConnectionLineAnimation(outerIdx, innerIdx);
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

void CaesarScene::startConnectionLineAnimation(int outerIdx, int innerIdx) {
    if (outerIdx < 0 || outerIdx >= outerLetters_.size()) return;
    if (innerIdx < 0 || innerIdx >= innerLetters_.size()) return;

    double outerAngle = (outerIdx * DEG_PER_LETTER - 90) * M_PI / 180;
    connStartX_ = CENTER_X + RADIUS_OUTER * 0.85 * cos(outerAngle);
    connStartY_ = CENTER_Y + RADIUS_OUTER * 0.85 * sin(outerAngle);

    double innerAngle = (innerIdx * DEG_PER_LETTER - 90 + currentRotation_) * M_PI / 180;
    connEndX_ = CENTER_X + RADIUS_INNER * 0.8 * cos(innerAngle);
    connEndY_ = CENTER_Y + RADIUS_INNER * 0.8 * sin(innerAngle);

    connAnimOuterIdx_ = outerIdx;
    connAnimInnerIdx_ = innerIdx;
    connAnimProgress_ = 0;

    connectionAnimTimer_->start(25);
}

void CaesarScene::animateConnectionLine() {
    connAnimProgress_ += 1.0 / CONN_ANIM_STEPS;
    if (connAnimProgress_ >= 1.0) {
        connAnimProgress_ = 1.0;
        connectionAnimTimer_->stop();
    }

    clearConnectionLines();

    double curX = connStartX_ + (connEndX_ - connStartX_) * connAnimProgress_;
    double curY = connStartY_ + (connEndY_ - connStartY_) * connAnimProgress_;

    auto* line = addLine(connStartX_, connStartY_, curX, curY,
                          QPen(QColor(255, 255, 0, 200), 2, Qt::DashLine));
    line->setZValue(0);
    connectionLines_.append(line);

    auto* dot = addEllipse(curX - 3, curY - 3, 6, 6,
                            QPen(Qt::NoPen), QBrush(QColor(255, 255, 0, 220)));
    dot->setZValue(2);
    connectionLines_.append(dot);

    if (connAnimProgress_ >= 1.0) {
        auto* finalLine = addLine(connStartX_, connStartY_, connEndX_, connEndY_,
                                   QPen(QColor(255, 255, 0, 180), 2, Qt::DashLine));
        finalLine->setZValue(0);
        connectionLines_.append(finalLine);
    }
}

void CaesarScene::clearConnectionLines() {
    for (auto* item : connectionLines_) {
        removeItem(item);
        delete item;
    }
    connectionLines_.clear();
}

void CaesarScene::reset() {
    stepTimer_->stop();
    stepAnimTimer_->stop();
    highlightTimer_->stop();
    pulseTimer_->stop();
    connectionAnimTimer_->stop();
    clear();
    outerLetters_.clear();
    innerLetters_.clear();
    clearConnectionLines();
    outerRing_ = nullptr;
    innerRing_ = nullptr;
    resultText_ = nullptr;
    currentRotation_ = 0;
    rotateStepIndex_ = 0;
    rotateStepTotal_ = 0;
    highlightIndex_ = 0;
    highlightShowing_ = false;
    connAnimProgress_ = 0;
    resultCharIndex_ = 0;
    resultTarget_.clear();
    typewriterActive_ = false;
    animationId_++;
}

void CaesarScene::setSpeed(int ms) {
    animSpeed_ = ms;
    if (highlightTimer_->isActive()) {
        highlightTimer_->setInterval(ms);
    }
}
