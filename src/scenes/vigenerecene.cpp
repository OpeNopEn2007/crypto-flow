#include "vigenerecene.h"
#include <QGraphicsDropShadowEffect>
#include <QFont>
#include <QtMath>
#include <QDebug>
#include <QRegularExpression>

static const double R_OUTER = 170;
static const double R_INNER = 110;
static const double CX = 300;
static const double CY = 240;
static const double DEG = 360.0 / 26;

static QGraphicsDropShadowEffect* glow(QColor c, int blur = 15) {
    auto* e = new QGraphicsDropShadowEffect;
    e->setBlurRadius(blur);
    e->setColor(c);
    e->setOffset(0);
    return e;
}

VigenereScene::VigenereScene(QObject* parent)
    : QGraphicsScene(parent)
    , rotateStepTimer_(new QTimer(this))
    , rotateStepAnimTimer_(new QTimer(this))
    , rewindTimer_(new QTimer(this))
    , letterTimer_(new QTimer(this))
    , pulseTimer_(new QTimer(this)) {
    setSceneRect(0, 0, 600, 500);
    connect(rotateStepTimer_, &QTimer::timeout, this, &VigenereScene::onRotateStepTick);
    connect(rotateStepAnimTimer_, &QTimer::timeout, this, &VigenereScene::onRotateStepAnimTick);
    connect(rewindTimer_, &QTimer::timeout, this, &VigenereScene::onRewindTick);
    connect(letterTimer_, &QTimer::timeout, this, &VigenereScene::onLetterTick);
    connect(pulseTimer_, &QTimer::timeout, this, &VigenereScene::animatePulse);
}

void VigenereScene::drawRings() {
    // 外圈
    outerRing_ = addEllipse(CX - R_OUTER, CY - R_OUTER, R_OUTER*2, R_OUTER*2,
                             QPen(QColor(0, 200, 255), 3));
    outerRing_->setZValue(0);
    outerRing_->setGraphicsEffect(glow(QColor(0, 200, 255), 25));

    // 刻度线
    for (int i = 0; i < 26; i++) {
        double a = (i * DEG - 90) * M_PI / 180;
        double x1 = CX + R_OUTER * 0.95 * cos(a);
        double y1 = CY + R_OUTER * 0.95 * sin(a);
        double x2 = CX + R_OUTER * 1.02 * cos(a);
        double y2 = CY + R_OUTER * 1.02 * sin(a);
        auto* tick = addLine(x1, y1, x2, y2, QPen(QColor(0, 200, 255, 120), 1));
        tick->setZValue(0);
    }

    QString alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < 26; i++) {
        double a = (i * DEG - 90) * M_PI / 180;
        double x = CX + R_OUTER * 0.85 * cos(a) - 8;
        double y = CY + R_OUTER * 0.85 * sin(a) - 10;
        auto* lt = addText(QString(alpha[i]), QFont("Menlo", 15, QFont::Bold));
        lt->setPos(x, y);
        lt->setDefaultTextColor(QColor(0, 200, 255));
        lt->setZValue(1);
        outerLetters_.append(lt);
    }

    // 内圈
    innerRing_ = addEllipse(CX - R_INNER, CY - R_INNER, R_INNER*2, R_INNER*2,
                             QPen(QColor(255, 100, 50), 3));
    innerRing_->setZValue(0);
    innerRing_->setGraphicsEffect(glow(QColor(255, 100, 50), 18));
    innerRing_->setTransformOriginPoint(CX, CY);

    for (int i = 0; i < 26; i++) {
        double a = (i * DEG - 90) * M_PI / 180;
        double x1 = CX + R_INNER * 0.92 * cos(a);
        double y1 = CY + R_INNER * 0.92 * sin(a);
        double x2 = CX + R_INNER * 0.98 * cos(a);
        double y2 = CY + R_INNER * 0.98 * sin(a);
        auto* tick = addLine(x1, y1, x2, y2, QPen(QColor(255, 100, 50, 100), 1));
        tick->setZValue(0);
    }

    for (int i = 0; i < 26; i++) {
        double a = (i * DEG - 90) * M_PI / 180;
        double x = CX + R_INNER * 0.8 * cos(a) - 8;
        double y = CY + R_INNER * 0.8 * sin(a) - 10;
        auto* lt = addText(QString(alpha[i]), QFont("Menlo", 15, QFont::Bold));
        lt->setPos(x, y);
        lt->setDefaultTextColor(QColor(255, 100, 50));
        lt->setZValue(1);
        innerLetters_.append(lt);
    }
}

void VigenereScene::setInnerRingRotation(double deg) {
    innerRing_->setRotation(deg);
    for (int i = 0; i < 26; i++) {
        double a = (i * DEG - 90 + deg) * M_PI / 180;
        double x = CX + R_INNER * 0.8 * cos(a) - 8;
        double y = CY + R_INNER * 0.8 * sin(a) - 10;
        innerLetters_[i]->setPos(x, y);
    }
}

void VigenereScene::showExplanation(const QString& text) {
    if (!explanation_) {
        explanation_ = addText("", QFont("PingFang SC", 13));
        explanation_->setDefaultTextColor(QColor(200, 200, 200));
        explanation_->setZValue(10);
    }
    explanation_->setPlainText(text);
    QRectF r = explanation_->boundingRect();
    explanation_->setPos(CX - r.width() / 2, CY + R_OUTER + 40);
}

void VigenereScene::startAnimation(const QString& text, const QString& keyword) {
    QString newText = text.toUpper();
    QString newKeyword = keyword.toUpper().remove(QRegularExpression("[^A-Z]"));
    if (newKeyword.isEmpty()) return;

    // 如果 rewind 正在进行，只更新 pending 参数，不打断当前 rewind
    if (rewindTimer_->isActive()) {
        qInfo() << "[Vigenere] Rewind in progress, queuing new animation";
        pendingText_ = newText;
        pendingKeyword_ = newKeyword;
        return;
    }

    // 先废除所有旧动画的 singleShot 回调
    animationId_++;
    activeAnimId_ = animationId_;
    rotateStepTimer_->stop();
    rotateStepAnimTimer_->stop();
    rewindTimer_->stop();
    pulseTimer_->stop();

    inputText_ = newText;
    keyword_ = newKeyword;

    // 如果内圈还在非零位置，先回转再启动新动画
    if (qAbs(currentRotation_) > 0.1 && innerRing_) {
        qInfo() << "[Vigenere] Inner ring at" << currentRotation_ << "deg, rewinding first";
        showExplanation("正在复位内圈...");
        pendingText_ = inputText_;
        pendingKeyword_ = keyword_;
        rewindFromDeg_ = currentRotation_;
        rewindFrame_ = 0;
        rewindTimer_->start(20);
        return;
    }

    reset();
    launchAnimation();
}

void VigenereScene::launchAnimation() {
    // 计算每个字母的 shift
    QString alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int ki = 0;
    keywordIndex_ = 0;
    for (int i = 0; i < inputText_.size(); i++) {
        if (inputText_[i].isLetter()) {
            shifts_.append(alpha.indexOf(keyword_[ki % keyword_.size()]));
            ki++;
        } else {
            shifts_.append(0);
        }
    }

    qInfo() << "[Vigenere] Starting, text:" << inputText_ << "keyword:" << keyword_;

    drawRings();

    auto* outerLabel = addText("明文", QFont("PingFang SC", 10));
    outerLabel->setDefaultTextColor(QColor(0, 200, 255, 180));
    outerLabel->setPos(CX + R_OUTER + 5, CY - 10);
    outerLabel->setZValue(2);

    auto* innerLabel = addText("密文", QFont("PingFang SC", 10));
    innerLabel->setDefaultTextColor(QColor(255, 100, 50, 180));
    innerLabel->setPos(CX + R_INNER + 5, CY - 10);
    innerLabel->setZValue(2);

    keywordDisplay_ = addText("密钥: " + keyword_, QFont("Menlo", 12, QFont::Bold));
    keywordDisplay_->setDefaultTextColor(QColor(255, 200, 0));
    keywordDisplay_->setGraphicsEffect(glow(QColor(255, 200, 0), 10));
    keywordDisplay_->setPos(CX - 60, CY + R_OUTER + 15);
    keywordDisplay_->setZValue(2);

    resultText_ = addText("", QFont("Menlo", 20, QFont::Bold));
    resultText_->setDefaultTextColor(QColor(0, 255, 150));
    resultText_->setZValue(5);
    resultText_->setGraphicsEffect(glow(QColor(0, 255, 150), 20));
    resultText_->setVisible(true);
    QRectF r = resultText_->boundingRect();
    resultText_->setPos(CX - r.width() / 2, CY - r.height() / 2);

    resultString_.clear();
    pulsePhase_ = 0;
    pulseTimer_->start(50);
    currentRotation_ = 0;
    currentLetterIndex_ = 0;

    showExplanation("维吉尼亚密码: 使用密钥对每个字母施加不同偏移量");

    int myId = animationId_;
    QTimer::singleShot(2000, this, [this, myId]() {
        if (myId != animationId_) return;
        currentLetterIndex_ = 0;
        onLetterTick();
    });
}

void VigenereScene::onLetterTick() {
    if (animationId_ != activeAnimId_) return;
    if (currentLetterIndex_ >= inputText_.size()) {
        letterTimer_->stop();
        // 所有字母处理完毕
        pulseTimer_->stop();
        outerRing_->setGraphicsEffect(glow(QColor(0, 200, 255), 25));
        innerRing_->setGraphicsEffect(glow(QColor(255, 100, 50), 18));

        // 居中显示结果
        resultText_->setPlainText(resultString_);
        QRectF r = resultText_->boundingRect();
        resultText_->setPos(CX - r.width() / 2, CY - r.height() / 2);

        showExplanation("加密完成! 结果: " + resultString_);
        qInfo() << "[Vigenere] Complete, result:" << resultString_;
        // 延迟 1 秒再触发完成（让用户看到最终结果）
        QTimer::singleShot(1000, this, [this, myId = animationId_]() {
            if (myId != animationId_) return;
            emit animationComplete();
        });
        return;
    }

    QChar ch = inputText_[currentLetterIndex_];
    int shift = shifts_[currentLetterIndex_];

    if (!ch.isLetter()) {
        // 非字母直接加入结果
        resultString_ += ch;
        resultText_->setPlainText(resultString_);
        QRectF r = resultText_->boundingRect();
        resultText_->setPos(CX - r.width() / 2, CY - r.height() / 2);
        currentLetterIndex_++;
        // 处理下一个
        QTimer::singleShot(500, this, [this, myId = animationId_]() {
            if (myId == animationId_) onLetterTick();
        });
        return;
    }

    // 字母处理：显示说明 → 逐格旋转 → 高亮 → 下一个
    int keyIdx = shifts_[currentLetterIndex_];
    QChar keyCh = keyword_[keywordIndex_ % keyword_.size()];
    keywordIndex_++;
    QChar encrypted = QChar('A' + (ch.unicode() - 'A' + keyIdx) % 26);

    QString expl = QString("第 %1 个字母: 明文 %2 + 密钥 %3 (偏移 %4) → %5")
        .arg(currentLetterIndex_ + 1)
        .arg(ch).arg(keyCh).arg(keyIdx).arg(encrypted);
    showExplanation(expl);

    qInfo() << "[Vigenere] Letter" << currentLetterIndex_ << ":"
            << ch << "+ key" << keyCh << "(" << keyIdx << ")" << "->" << encrypted;

    // 停顿 600ms 让用户读完说明，再开始旋转
    QTimer::singleShot(600, this, [this, keyIdx, myId = animationId_]() {
        if (myId != animationId_) return;
        startLetterRotation(keyIdx);
    });
}

void VigenereScene::startLetterRotation(int targetShift) {
    rotateStepIndex_ = 0;
    // 偏移 >= 14 时反向旋转更快（如 14→顺时针12步, 22→顺时针4步）
    if (targetShift > 13) {
        rotateStepTotal_ = 26 - targetShift;
        rotateDirection_ = -1;
    } else {
        rotateStepTotal_ = targetShift;
        rotateDirection_ = 1;
    }

    if (rotateStepTotal_ == 0) {
        // 偏移为 0，直接高亮
        int outerIdx = inputText_[currentLetterIndex_].unicode() - 'A';
        int innerIdx = outerIdx;
        highlightPair(outerIdx, innerIdx);

        QChar encrypted = QChar('A' + innerIdx);
        resultString_ += encrypted;
        resultText_->setPlainText(resultString_);
        QRectF r = resultText_->boundingRect();
        resultText_->setPos(CX - r.width() / 2, CY - r.height() / 2);

        currentLetterIndex_++;
        highlightShowing_ = true;
        // 高亮停留后，回转内圈到初始位置，再处理下一个字母
        QTimer::singleShot(animSpeed_ + 500, this, [this, myId = animationId_]() {
            if (myId != animationId_) return;
            clearHighlights();
            highlightShowing_ = false;
            if (currentLetterIndex_ < inputText_.size() && qAbs(currentRotation_) > 0.1) {
                showExplanation("回转内圈到初始位置...");
                rewindFromDeg_ = currentRotation_;
                rewindFrame_ = 0;
                rewindTimer_->start(20);
            } else {
                QTimer::singleShot(500, this, [this, myId = animationId_]() {
                    if (myId != animationId_) return;
                    onLetterTick();
                });
            }
        });
        return;
    }

    stepFromDeg_ = currentRotation_;
    stepAnimFrame_ = 0;

    qInfo() << "[Vigenere] Rotating" << rotateStepTotal_ << "steps for shift";

    // 开始第一格
    onRotateStepTick();
    if (rotateStepTotal_ > 0) {
        rotateStepTimer_->start(STEP_INTERVAL_MS);
    }
}

void VigenereScene::onRotateStepTick() {
    if (rotateStepIndex_ >= rotateStepTotal_) {
        rotateStepTimer_->stop();
        // 旋转完成，高亮配对
        int outerIdx = inputText_[currentLetterIndex_].unicode() - 'A';
        int innerIdx = (outerIdx + rotateDirection_ * rotateStepTotal_ + 26) % 26;
        highlightPair(outerIdx, innerIdx);

        QChar encrypted = QChar('A' + innerIdx);
        resultString_ += encrypted;
        resultText_->setPlainText(resultString_);
        QRectF r = resultText_->boundingRect();
        resultText_->setPos(CX - r.width() / 2, CY - r.height() / 2);

        currentLetterIndex_++;
        highlightShowing_ = true;

        // 高亮停留后，回转内圈到初始位置，再处理下一个字母
        QTimer::singleShot(animSpeed_ + 500, this, [this, myId = animationId_]() {
            if (myId != animationId_) return;
            clearHighlights();
            highlightShowing_ = false;
            if (currentLetterIndex_ < inputText_.size() && qAbs(currentRotation_) > 0.1) {
                showExplanation("回转内圈到初始位置...");
                rewindFromDeg_ = currentRotation_;
                rewindFrame_ = 0;
                rewindTimer_->start(20);
            } else {
                QTimer::singleShot(500, this, [this, myId = animationId_]() {
                    if (myId != animationId_) return;
                    onLetterTick();
                });
            }
        });
        return;
    }

    stepFromDeg_ = currentRotation_;
    stepToDeg_ = rotateDirection_ * -(rotateStepIndex_ + 1) * DEG;
    stepAnimFrame_ = 0;
    rotateStepIndex_++;
    rotateStepAnimTimer_->start(20);
}

void VigenereScene::onRotateStepAnimTick() {
    stepAnimFrame_++;
    double t = static_cast<double>(stepAnimFrame_) / STEP_ANIM_FRAMES;
    QEasingCurve curve(QEasingCurve::OutCubic);
    double eased = curve.valueForProgress(qMin(t, 1.0));
    currentRotation_ = stepFromDeg_ + (stepToDeg_ - stepFromDeg_) * eased;
    if (stepAnimFrame_ >= STEP_ANIM_FRAMES) {
        currentRotation_ = stepToDeg_;
        rotateStepAnimTimer_->stop();
    }
    setInnerRingRotation(currentRotation_);
}

void VigenereScene::onRewindTick() {
    rewindFrame_++;
    double t = static_cast<double>(rewindFrame_) / REWIND_FRAMES;
    if (t >= 1.0) t = 1.0;

    currentRotation_ = rewindFromDeg_ * (1.0 - t);
    setInnerRingRotation(currentRotation_);

    if (rewindFrame_ >= REWIND_FRAMES) {
        rewindTimer_->stop();
        currentRotation_ = 0;
        setInnerRingRotation(0);

        if (!pendingText_.isEmpty()) {
            // 用户触发的 rewind：有待处理的新动画参数
            qInfo() << "[Vigenere] User rewind complete, restoring pending:" << pendingText_;
            inputText_ = pendingText_;
            keyword_ = pendingKeyword_;
            pendingText_.clear();
            pendingKeyword_.clear();

            QTimer::singleShot(300, this, [this, myId = animationId_]() {
                if (myId != animationId_) return;
                reset();
                launchAnimation();
            });
        } else {
            // 动画内部的字母间 rewind：继续处理下一个字母
            qInfo() << "[Vigenere] Letter rewind complete, continuing to next letter";
            showExplanation("回转完成, 处理下一个字母");
            QTimer::singleShot(500, this, [this, myId = animationId_]() {
                if (myId != animationId_) return;
                onLetterTick();
            });
        }
    }
}

void VigenereScene::highlightPair(int outerIdx, int innerIdx) {
    clearHighlights();
    if (outerIdx >= 0 && outerIdx < outerLetters_.size()) {
        outerLetters_[outerIdx]->setDefaultTextColor(QColor(255, 255, 0));
        outerLetters_[outerIdx]->setGraphicsEffect(glow(QColor(255, 255, 0), 30));
    }
    if (innerIdx >= 0 && innerIdx < innerLetters_.size()) {
        innerLetters_[innerIdx]->setDefaultTextColor(QColor(255, 255, 0));
        innerLetters_[innerIdx]->setGraphicsEffect(glow(QColor(255, 255, 0), 30));
    }
}

void VigenereScene::clearHighlights() {
    for (auto* l : outerLetters_) {
        l->setDefaultTextColor(QColor(0, 200, 255));
        l->setGraphicsEffect(nullptr);
    }
    for (auto* l : innerLetters_) {
        l->setDefaultTextColor(QColor(255, 100, 50));
        l->setGraphicsEffect(nullptr);
    }
}

void VigenereScene::animatePulse() {
    pulsePhase_ += 0.08;
    double p = 0.5 + 0.5 * sin(pulsePhase_);
    outerRing_->setGraphicsEffect(glow(QColor(0, 200, 255), 20 + p * 10));
    innerRing_->setGraphicsEffect(glow(QColor(255, 100, 50), 15 + p * 8));
}

void VigenereScene::reset() {
    rotateStepTimer_->stop();
    rotateStepAnimTimer_->stop();
    rewindTimer_->stop();
    letterTimer_->stop();
    pulseTimer_->stop();
    clear();
    outerLetters_.clear();
    innerLetters_.clear();
    outerRing_ = nullptr;
    innerRing_ = nullptr;
    resultText_ = nullptr;
    keywordDisplay_ = nullptr;
    explanation_ = nullptr;
    currentRotation_ = 0;
    rotateStepIndex_ = 0;
    rotateDirection_ = 1;
    currentLetterIndex_ = 0;
    keywordIndex_ = 0;
    highlightShowing_ = false;
    resultString_.clear();
    shifts_.clear();
}

void VigenereScene::setSpeed(int ms) {
    animSpeed_ = ms;
}
