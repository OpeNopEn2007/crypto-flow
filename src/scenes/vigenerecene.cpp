#include "vigenerecene.h"
#include "../crypto/vigenere.h"
#include <QGraphicsDropShadowEffect>
#include <QFont>
#include <QtMath>
#include <QDebug>
#include <QRegularExpression>

static const double R_OUTER = 170;
static const double R_INNER = 110;
static const double CX = 300;
static const double CY = 250;
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
    , stepTimer_(new QTimer(this))
    , stepAnimTimer_(new QTimer(this))
    , highlightTimer_(new QTimer(this))
    , pulseTimer_(new QTimer(this)) {
    setSceneRect(0, 0, 600, 500);
    connect(stepTimer_, &QTimer::timeout, this, &VigenereScene::onStepTick);
    connect(stepAnimTimer_, &QTimer::timeout, this, &VigenereScene::onStepAnimTick);
    connect(highlightTimer_, &QTimer::timeout, this, &VigenereScene::animateHighlight);
    connect(pulseTimer_, &QTimer::timeout, this, &VigenereScene::animatePulse);
}

void VigenereScene::drawRings() {
    // 外圈
    outerRing_ = addEllipse(CX - R_OUTER, CY - R_OUTER, R_OUTER*2, R_OUTER*2,
                             QPen(QColor(0, 200, 255), 3));
    outerRing_->setZValue(0);
    outerRing_->setGraphicsEffect(glow(QColor(0, 200, 255), 25));

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

void VigenereScene::startAnimation(const QString& text, const QString& keyword) {
    reset();
    inputText_ = text.toUpper();
    keyword_ = keyword.toUpper().remove(QRegularExpression("[^A-Z]"));

    // 计算每个字母的 shift
    QString alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int ki = 0;
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

    // 标签
    auto* outerLabel = addText("明文", QFont("PingFang SC", 10));
    outerLabel->setDefaultTextColor(QColor(0, 200, 255, 180));
    outerLabel->setPos(CX + R_OUTER + 5, CY - 10);
    outerLabel->setZValue(2);

    auto* innerLabel = addText("密文", QFont("PingFang SC", 10));
    innerLabel->setDefaultTextColor(QColor(255, 100, 50, 180));
    innerLabel->setPos(CX + R_INNER + 5, CY - 10);
    innerLabel->setZValue(2);

    // 关键词显示
    keywordDisplay_ = addText("KEY: " + keyword_, QFont("Menlo", 12, QFont::Bold));
    keywordDisplay_->setDefaultTextColor(QColor(255, 200, 0));
    keywordDisplay_->setGraphicsEffect(glow(QColor(255, 200, 0), 10));
    keywordDisplay_->setPos(CX - 60, CY + R_OUTER + 15);
    keywordDisplay_->setZValue(2);

    resultText_ = addText("", QFont("Menlo", 20, QFont::Bold));
    resultText_->setDefaultTextColor(QColor(0, 255, 150));
    resultText_->setZValue(5);
    resultText_->setGraphicsEffect(glow(QColor(0, 255, 150), 20));
    resultText_->setVisible(false);
    resultTarget_ = "";
    resultCharIndex_ = 0;
    typewriterActive_ = false;

    pulsePhase_ = 0;
    pulseTimer_->start(50);

    // 逐字母旋转（每个字母用对应的 shift）
    currentRotation_ = 0;
    rotateStepIndex_ = 0;
    // 只旋转字母字符的 shift
    rotateStepTotal_ = 0;
    for (int s : shifts_) {
        if (s > 0) rotateStepTotal_ += s;
    }
    // 如果有非零 shift，计算总旋转量
    // 实际上我们逐字母处理，每个字母独立旋转

    animationId_++;
    int myId = animationId_;
    rotateStepIndex_ = 0;

    // 找到第一个需要旋转的字母
    QTimer::singleShot(1000, this, [this, myId]() {
        if (myId != animationId_) return;
        highlightIndex_ = 0;
        highlightShowing_ = false;
        highlightTimer_->start(animSpeed_);
    });
}

void VigenereScene::onStepTick() {}

void VigenereScene::onStepAnimTick() {
    stepAnimFrame_++;
    double t = static_cast<double>(stepAnimFrame_) / STEP_ANIM_FRAMES;
    QEasingCurve curve(QEasingCurve::OutCubic);
    double eased = curve.valueForProgress(qMin(t, 1.0));
    currentRotation_ = stepFromDeg_ + (stepToDeg_ - stepFromDeg_) * eased;
    if (stepAnimFrame_ >= STEP_ANIM_FRAMES) {
        currentRotation_ = stepToDeg_;
        stepAnimTimer_->stop();
    }
    setInnerRingRotation(currentRotation_);
}

void VigenereScene::animateHighlight() {
    if (typewriterActive_) {
        if (resultCharIndex_ < resultTarget_.size()) {
            resultCharIndex_++;
            resultText_->setPlainText(resultTarget_.left(resultCharIndex_));
            QRectF r = resultText_->boundingRect();
            resultText_->setPos(CX - r.width() / 2, CY - r.height() / 2);
        } else {
            highlightTimer_->stop();
            typewriterActive_ = false;
            qInfo() << "[Vigenere] Complete, result:" << resultText_->toPlainText();
            emit animationComplete();
        }
        return;
    }

    if (highlightShowing_) {
        // 清除高亮
        for (auto* l : outerLetters_) {
            l->setDefaultTextColor(QColor(0, 200, 255));
            l->setGraphicsEffect(nullptr);
        }
        for (auto* l : innerLetters_) {
            l->setDefaultTextColor(QColor(255, 100, 50));
            l->setGraphicsEffect(nullptr);
        }
        highlightShowing_ = false;
        highlightIndex_++;
        highlightTimer_->setInterval(150);
        return;
    }

    if (highlightIndex_ >= inputText_.size()) {
        highlightTimer_->stop();
        pulseTimer_->stop();
        outerRing_->setGraphicsEffect(glow(QColor(0, 200, 255), 25));
        innerRing_->setGraphicsEffect(glow(QColor(255, 100, 50), 18));

        resultTarget_ = resultText_->toPlainText();
        resultCharIndex_ = 0;
        resultText_->setPlainText("");
        resultText_->setVisible(true);
        QRectF r = resultText_->boundingRect();
        resultText_->setPos(CX - r.width() / 2, CY - r.height() / 2);
        typewriterActive_ = true;
        highlightTimer_->start(80);
        return;
    }

    QChar ch = inputText_[highlightIndex_];
    int shift = shifts_[highlightIndex_];

    if (ch.isLetter() && shift > 0) {
        // 旋转内圈到对应 shift
        stepFromDeg_ = currentRotation_;
        stepToDeg_ = -shift * DEG;
        stepAnimFrame_ = 0;
        stepAnimTimer_->start(20);

        // 高亮配对
        int outerIdx = ch.unicode() - 'A';
        int innerIdx = (outerIdx + shift) % 26;

        for (auto* l : outerLetters_) {
            l->setDefaultTextColor(QColor(0, 200, 255));
            l->setGraphicsEffect(nullptr);
        }
        for (auto* l : innerLetters_) {
            l->setDefaultTextColor(QColor(255, 100, 50));
            l->setGraphicsEffect(nullptr);
        }

        if (outerIdx >= 0 && outerIdx < outerLetters_.size()) {
            outerLetters_[outerIdx]->setDefaultTextColor(QColor(255, 255, 0));
            outerLetters_[outerIdx]->setGraphicsEffect(glow(QColor(255, 255, 0), 30));
        }
        if (innerIdx >= 0 && innerIdx < innerLetters_.size()) {
            innerLetters_[innerIdx]->setDefaultTextColor(QColor(255, 255, 0));
            innerLetters_[innerIdx]->setGraphicsEffect(glow(QColor(255, 255, 0), 30));
        }

        QChar enc = QChar('A' + innerIdx);
        resultText_->setPlainText(resultText_->toPlainText() + enc);
        qDebug() << "[Vigenere]" << highlightIndex_ << ":" << ch << "shift=" << shift << "->" << enc;
    } else {
        resultText_->setPlainText(resultText_->toPlainText() + ch);
        highlightIndex_++;
        return;
    }

    highlightShowing_ = true;
    highlightTimer_->setInterval(animSpeed_);
}

void VigenereScene::animatePulse() {
    pulsePhase_ += 0.08;
    double p = 0.5 + 0.5 * sin(pulsePhase_);
    outerRing_->setGraphicsEffect(glow(QColor(0, 200, 255), 20 + p * 10));
    innerRing_->setGraphicsEffect(glow(QColor(255, 100, 50), 15 + p * 8));
}

void VigenereScene::reset() {
    stepTimer_->stop();
    stepAnimTimer_->stop();
    highlightTimer_->stop();
    pulseTimer_->stop();
    clear();
    outerLetters_.clear();
    innerLetters_.clear();
    outerRing_ = nullptr;
    innerRing_ = nullptr;
    resultText_ = nullptr;
    keywordDisplay_ = nullptr;
    currentRotation_ = 0;
    rotateStepIndex_ = 0;
    highlightIndex_ = 0;
    highlightShowing_ = false;
    resultCharIndex_ = 0;
    resultTarget_.clear();
    typewriterActive_ = false;
    shifts_.clear();
    animationId_++;
}

void VigenereScene::setSpeed(int ms) {
    animSpeed_ = ms;
    if (highlightTimer_->isActive()) {
        highlightTimer_->setInterval(ms);
    }
}
