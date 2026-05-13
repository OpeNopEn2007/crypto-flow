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
    , rewindTimer_(new QTimer(this))
    , highlightTimer_(new QTimer(this))
    , pulseTimer_(new QTimer(this))
    , connectionAnimTimer_(new QTimer(this)) {
    setSceneRect(0, 0, 600, 500);
    connect(stepTimer_, &QTimer::timeout, this, &CaesarScene::onStepTick);
    connect(stepAnimTimer_, &QTimer::timeout, this, &CaesarScene::onStepAnimTick);
    connect(rewindTimer_, &QTimer::timeout, this, &CaesarScene::onRewindTick);
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
    QString newText = text.toUpper();
    int newShift = ((shift % 26) + 26) % 26;

    // 如果 rewind 正在进行，只更新 pending 参数，不打断当前 rewind
    if (rewindTimer_->isActive()) {
        qInfo() << "[Caesar] Rewind in progress, queuing new animation";
        pendingText_ = newText;
        pendingShift_ = newShift;
        return;
    }

    // 先废除所有旧动画的 singleShot 回调
    animationId_++;
    stepTimer_->stop();
    stepAnimTimer_->stop();
    rewindTimer_->stop();
    highlightTimer_->stop();
    pulseTimer_->stop();

    inputText_ = newText;
    shift_ = newShift;
    qInfo() << "[Caesar] Starting animation, text:" << inputText_ << "shift:" << shift_;

    // 如果内圈还在非零位置，先回转再启动新动画
    if (qAbs(currentRotation_) > 0.1 && innerRing_) {
        qInfo() << "[Caesar] Inner ring at" << currentRotation_ << "deg, rewinding first";
        showExplanation("正在复位内圈...");
        pendingText_ = inputText_;
        pendingShift_ = shift_;
        rewindFromDeg_ = currentRotation_;
        rewindFrame_ = 0;
        rewindTimer_->start(20);
        return;
    }

    reset();
    launchAnimation();
}

void CaesarScene::onStepTick() {
    if (rotateStepIndex_ >= rotateStepTotal_) {
        stepTimer_->stop();
        qInfo() << "[Caesar] Step rotation complete, now highlighting letters";
        showExplanation(QString("旋转完成! 内圈已偏移 %1 位, 开始逐字母加密").arg(shift_));
        highlightIndex_ = 0;
        highlightShowing_ = false;
        // 停顿 500ms 让用户看到旋转结果，再开始高亮
        QTimer::singleShot(500, this, [this, myId = animationId_]() {
            if (myId != animationId_) return;
            highlightTimer_->start(animSpeed_);
        });
        return;
    }

    // 计算当前格的起止角度
    stepFromDeg_ = currentRotation_;
    stepToDeg_ = rotateDirection_ * -(rotateStepIndex_ + 1) * DEG_PER_LETTER;
    stepAnimFrame_ = 0;

    rotateStepIndex_++;
    qDebug() << "[Caesar] Step" << rotateStepIndex_ << "/" << rotateStepTotal_
             << ": " << stepFromDeg_ << "->" << stepToDeg_ << "deg";

    // 旋转阶段说明
    QChar fromCh = QChar('A' + rotateStepIndex_ - 1);
    QChar toCh = QChar('A' + rotateStepIndex_);
    showExplanation(QString("内圈旋转第 %1/%2 格: %3 → %4")
        .arg(rotateStepIndex_).arg(rotateStepTotal_).arg(fromCh).arg(toCh));

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

void CaesarScene::onRewindTick() {
    rewindFrame_++;
    double t = static_cast<double>(rewindFrame_) / REWIND_FRAMES;
    if (t >= 1.0) t = 1.0;

    currentRotation_ = rewindFromDeg_ * (1.0 - t);
    setInnerRingRotation(currentRotation_);

    if (rewindFrame_ >= REWIND_FRAMES) {
        rewindTimer_->stop();
        currentRotation_ = 0;
        setInnerRingRotation(0);
        qInfo() << "[Caesar] Rewind complete, starting pending animation";
        showExplanation("复位完成, 开始新的加密");

        // 用待处理的参数启动新动画
        inputText_ = pendingText_;
        shift_ = pendingShift_;
        pendingText_.clear();

        // 停顿 300ms 再启动新动画（必须先 reset 清理旧场景）
        QTimer::singleShot(300, this, [this, myId = animationId_]() {
            if (myId != animationId_) return;
            reset();
            launchAnimation();
        });
    }
}

void CaesarScene::launchAnimation() {
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

    showExplanation(QString("凯撒密码: 每个字母后移 %1 位 (A→%2)")
        .arg(shift_).arg(QChar('A' + shift_)));

    pulsePhase_ = 0;
    pulseTimer_->start(50);

    currentRotation_ = 0;
    rotateStepIndex_ = 0;
    // 偏移 >= 14 时反向旋转更快（如 14→顺时针12步, 22→顺时针4步）
    if (shift_ > 13) {
        rotateStepTotal_ = 26 - shift_;
        rotateDirection_ = -1;
    } else {
        rotateStepTotal_ = shift_;
        rotateDirection_ = 1;
    }
    stepFromDeg_ = 0;

    qInfo() << "[Caesar] Step rotation: " << rotateStepTotal_ << " steps,"
            << "direction:" << (rotateDirection_ > 0 ? "CCW" : "CW")
            << "interval:" << STEP_INTERVAL_MS << "ms";

    int myId = animationId_;
    QTimer::singleShot(2000, this, [this, myId]() {
        if (myId == animationId_) {
            onStepTick();
            if (rotateStepTotal_ > 0) {
                stepTimer_->start(STEP_INTERVAL_MS);
            }
        }
    });
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
        highlightShowing_ = false;
        highlightIndex_++;
        highlightTimer_->setInterval(300);
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
        highlightTimer_->start(100);
        showExplanation("加密完成! 结果: " + resultTarget_);
        return;
    }

    QChar ch = inputText_[highlightIndex_];
    if (ch.isLetter()) {
        int outerIdx = ch.unicode() - 'A';
        int innerIdx = (outerIdx + shift_) % 26;
        highlightPair(outerIdx, innerIdx);
        QChar encrypted = QChar('A' + innerIdx);
        resultText_->setPlainText(resultText_->toPlainText() + encrypted);
        qDebug() << "[Caesar] Highlight" << highlightIndex_ << ":" << ch << "->" << encrypted;

        showExplanation(QString("第 %1 个字母: %2 → %3 (偏移 %4)")
            .arg(highlightIndex_ + 1).arg(ch).arg(encrypted).arg(shift_));
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

void CaesarScene::showExplanation(const QString& text) {
    if (!explanation_) {
        explanation_ = addText("", QFont("PingFang SC", 13));
        explanation_->setDefaultTextColor(QColor(200, 200, 200));
        explanation_->setZValue(10);
    }
    explanation_->setPlainText(text);
    QRectF r = explanation_->boundingRect();
    explanation_->setPos(CENTER_X - r.width() / 2, CENTER_Y + RADIUS_OUTER + 25);
}

void CaesarScene::reset() {
    stepTimer_->stop();
    stepAnimTimer_->stop();
    rewindTimer_->stop();
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
    explanation_ = nullptr;
    currentRotation_ = 0;
    rotateStepIndex_ = 0;
    rotateStepTotal_ = 0;
    rotateDirection_ = 1;
    highlightIndex_ = 0;
    highlightShowing_ = false;
    connAnimProgress_ = 0;
    resultCharIndex_ = 0;
    resultTarget_.clear();
    typewriterActive_ = false;
}

void CaesarScene::setSpeed(int ms) {
    animSpeed_ = ms;
    if (highlightTimer_->isActive()) {
        highlightTimer_->setInterval(ms);
    }
}
