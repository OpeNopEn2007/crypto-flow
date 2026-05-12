#include "caesarscene.h"
#include <QGraphicsDropShadowEffect>
#include <QFont>
#include <QtMath>
#include <QDebug>

static QGraphicsDropShadowEffect* createGlowEffect(QColor color, int blur = 15) {
    auto* effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(blur);
    effect->setColor(color);
    effect->setOffset(0);
    return effect;
}

static const double RADIUS_OUTER = 180;
static const double RADIUS_INNER = 120;
static const double CENTER_X = 300;
static const double CENTER_Y = 250;

CaesarScene::CaesarScene(QObject* parent)
    : QGraphicsScene(parent), timer_(new QTimer(this)) {
    setSceneRect(0, 0, 600, 500);
    connect(timer_, &QTimer::timeout, this, &CaesarScene::animateStep);
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

void CaesarScene::startAnimation(const QString& text, int shift) {
    reset();
    inputText_ = text.toUpper();
    shift_ = ((shift % 26) + 26) % 26;
    qInfo() << "[Caesar] Starting animation, text:" << inputText_ << "shift:" << shift_;

    drawOuterRing();
    drawInnerRing();

    resultText_ = addText("", QFont("Menlo", 20, QFont::Bold));
    resultText_->setPos(CENTER_X - 80, CENTER_Y - 15);
    resultText_->setDefaultTextColor(QColor(0, 255, 150));
    resultText_->setZValue(5);
    resultText_->setGraphicsEffect(createGlowEffect(QColor(0, 255, 150), 20));

    currentStep_ = 0;
    currentRotation_ = 0;
    timer_->start(animSpeed_);
}

void CaesarScene::animateStep() {
    if (currentStep_ >= inputText_.size()) {
        timer_->stop();
        // 居中结果文字
        QRectF rect = resultText_->boundingRect();
        resultText_->setPos(CENTER_X - rect.width() / 2, CENTER_Y - rect.height() / 2);
        qInfo() << "[Caesar] Animation complete, result:" << resultText_->toPlainText();
        emit animationComplete();
        return;
    }

    QChar ch = inputText_[currentStep_];
    if (ch.isLetter()) {
        int idx = ch.unicode() - 'A';
        highlightLetter(idx);
        QChar encrypted = QChar('A' + (idx + shift_) % 26);
        resultText_->setPlainText(resultText_->toPlainText() + encrypted);
        qDebug() << "[Caesar] Step" << currentStep_ << ":" << ch << "->" << encrypted;
    } else {
        resultText_->setPlainText(resultText_->toPlainText() + ch);
    }
    currentStep_++;
}

void CaesarScene::highlightLetter(int index) {
    // 重置所有字母颜色
    for (auto* letter : outerLetters_) {
        letter->setDefaultTextColor(QColor(0, 200, 255));
        letter->setGraphicsEffect(nullptr);
    }
    for (auto* letter : innerLetters_) {
        letter->setDefaultTextColor(QColor(255, 100, 50));
        letter->setGraphicsEffect(nullptr);
    }
    // 高亮当前字母 - 金色 + 强发光
    if (index >= 0 && index < outerLetters_.size()) {
        outerLetters_[index]->setDefaultTextColor(QColor(255, 255, 0));
        outerLetters_[index]->setGraphicsEffect(createGlowEffect(QColor(255, 255, 0), 25));
        int targetIdx = (index + shift_) % 26;
        innerLetters_[targetIdx]->setDefaultTextColor(QColor(255, 255, 0));
        innerLetters_[targetIdx]->setGraphicsEffect(createGlowEffect(QColor(255, 255, 0), 25));
    }
}

void CaesarScene::reset() {
    timer_->stop();
    clear();
    outerLetters_.clear();
    innerLetters_.clear();
    outerRing_ = nullptr;
    innerRing_ = nullptr;
    resultText_ = nullptr;
    currentStep_ = 0;
    currentRotation_ = 0;
}

void CaesarScene::setSpeed(int ms) {
    animSpeed_ = ms;
    if (timer_->isActive()) {
        timer_->setInterval(ms);
    }
}
