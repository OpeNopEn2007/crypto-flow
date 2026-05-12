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
    , highlightTimer_(new QTimer(this)) {
    setSceneRect(0, 0, 600, 500);
    connect(rotateTimer_, &QTimer::timeout, this, &CaesarScene::animateRotation);
    connect(highlightTimer_, &QTimer::timeout, this, &CaesarScene::animateHighlight);
}

void CaesarScene::drawOuterRing() {
    outerRing_ = addEllipse(
        CENTER_X - RADIUS_OUTER, CENTER_Y - RADIUS_OUTER,
        RADIUS_OUTER * 2, RADIUS_OUTER * 2,
        QPen(QColor(0, 200, 255), 2));
    outerRing_->setZValue(0);
    outerRing_->setGraphicsEffect(createGlowEffect(QColor(0, 200, 255), 20));

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
        QPen(QColor(255, 100, 50), 2));
    innerRing_->setZValue(0);
    innerRing_->setGraphicsEffect(createGlowEffect(QColor(255, 100, 50), 15));
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
    // 旋转圆环
    innerRing_->setRotation(deg);

    // 字母跟着移动到旋转后的位置，但保持正立（不旋转字母本身）
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

    resultText_ = addText("", QFont("Menlo", 20, QFont::Bold));
    resultText_->setDefaultTextColor(QColor(0, 255, 150));
    resultText_->setZValue(5);
    resultText_->setGraphicsEffect(createGlowEffect(QColor(0, 255, 150), 20));
    resultText_->setVisible(false);

    // 逆时针旋转，让密文字母对齐明文字母
    currentRotation_ = 0;
    targetRotation_ = -shift_ * (360.0 / 26);
    rotateStep_ = targetRotation_ / 40.0;

    qInfo() << "[Caesar] Rotation phase: target" << targetRotation_ << "degrees";
    rotateTimer_->start(40);
}

void CaesarScene::animateRotation() {
    currentRotation_ += rotateStep_;

    bool done = (rotateStep_ > 0) ? (currentRotation_ >= targetRotation_)
                                   : (currentRotation_ <= targetRotation_);
    if (done) {
        currentRotation_ = targetRotation_;
        rotateTimer_->stop();
    }

    setInnerRingRotation(currentRotation_);

    if (!rotateTimer_->isActive()) {
        qInfo() << "[Caesar] Rotation complete, now highlighting letters";
        highlightIndex_ = 0;
        highlightTimer_->start(animSpeed_);
    }
}

void CaesarScene::animateHighlight() {
    if (highlightIndex_ >= inputText_.size()) {
        highlightTimer_->stop();
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
        QChar encrypted = QChar('A' + innerIdx);
        resultText_->setPlainText(resultText_->toPlainText() + encrypted);
        qDebug() << "[Caesar] Highlight" << highlightIndex_ << ":" << ch << "->" << encrypted;
    } else {
        resultText_->setPlainText(resultText_->toPlainText() + ch);
    }
    highlightIndex_++;
}

void CaesarScene::highlightPair(int outerIdx, int innerIdx) {
    for (auto* letter : outerLetters_) {
        letter->setDefaultTextColor(QColor(0, 200, 255));
        letter->setGraphicsEffect(nullptr);
    }
    for (auto* letter : innerLetters_) {
        letter->setDefaultTextColor(QColor(255, 100, 50));
        letter->setGraphicsEffect(nullptr);
    }

    if (outerIdx >= 0 && outerIdx < outerLetters_.size()) {
        outerLetters_[outerIdx]->setDefaultTextColor(QColor(255, 255, 0));
        outerLetters_[outerIdx]->setGraphicsEffect(createGlowEffect(QColor(255, 255, 0), 25));
    }
    if (innerIdx >= 0 && innerIdx < innerLetters_.size()) {
        innerLetters_[innerIdx]->setDefaultTextColor(QColor(255, 255, 0));
        innerLetters_[innerIdx]->setGraphicsEffect(createGlowEffect(QColor(255, 255, 0), 25));
    }
}

void CaesarScene::reset() {
    rotateTimer_->stop();
    highlightTimer_->stop();
    clear();
    outerLetters_.clear();
    innerLetters_.clear();
    outerRing_ = nullptr;
    innerRing_ = nullptr;
    resultText_ = nullptr;
    currentRotation_ = 0;
    highlightIndex_ = 0;
}

void CaesarScene::setSpeed(int ms) {
    animSpeed_ = ms;
    if (highlightTimer_->isActive()) {
        highlightTimer_->setInterval(ms);
    }
}
