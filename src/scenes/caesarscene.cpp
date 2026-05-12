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
    // 创建内圈 group，方便整体旋转
    innerGroup_ = createItemGroup({});

    innerRing_ = addEllipse(
        CENTER_X - RADIUS_INNER, CENTER_Y - RADIUS_INNER,
        RADIUS_INNER * 2, RADIUS_INNER * 2,
        QPen(QColor(255, 100, 50), 2));
    innerRing_->setZValue(0);
    innerRing_->setGraphicsEffect(createGlowEffect(QColor(255, 100, 50), 15));
    innerGroup_->addToGroup(innerRing_);

    QString alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < 26; i++) {
        double angle = (i * 360.0 / 26 - 90) * M_PI / 180;
        double x = CENTER_X + RADIUS_INNER * 0.8 * cos(angle) - 8;
        double y = CENTER_Y + RADIUS_INNER * 0.8 * sin(angle) - 10;
        auto* letter = addText(QString(alpha[i]), QFont("Menlo", 16, QFont::Bold));
        letter->setPos(x, y);
        letter->setDefaultTextColor(QColor(255, 100, 50));
        letter->setZValue(1);
        innerGroup_->addToGroup(letter);
        innerLetters_.append(letter);
    }

    // 保存原始变换
    originalInnerTransform_ = innerGroup_->transform();
}

void CaesarScene::startAnimation(const QString& text, int shift) {
    reset();
    inputText_ = text.toUpper();
    shift_ = ((shift % 26) + 26) % 26;
    qInfo() << "[Caesar] Starting animation, text:" << inputText_ << "shift:" << shift_;

    drawOuterRing();
    drawInnerRing();

    // 结果文字（初始隐藏，旋转完成后显示）
    resultText_ = addText("", QFont("Menlo", 20, QFont::Bold));
    resultText_->setDefaultTextColor(QColor(0, 255, 150));
    resultText_->setZValue(5);
    resultText_->setGraphicsEffect(createGlowEffect(QColor(0, 255, 150), 20));
    resultText_->setVisible(false);

    // 阶段1：旋转内圈
    currentRotation_ = 0;
    targetRotation_ = shift_ * (360.0 / 26);  // 每个字母间距 = 360/26 度
    rotateStep_ = targetRotation_ / 30.0;       // 分30步完成旋转
    currentStep_ = 0;

    qInfo() << "[Caesar] Rotation phase: target" << targetRotation_ << "degrees";
    rotateTimer_->start(30);  // 每30ms旋转一步，共约900ms
}

void CaesarScene::animateRotation() {
    currentRotation_ += rotateStep_;

    if (currentRotation_ >= targetRotation_) {
        currentRotation_ = targetRotation_;
        rotateTimer_->stop();

        // 应用最终旋转
        QTransform t;
        t.translate(CENTER_X, CENTER_Y);
        t.rotate(currentRotation_);
        t.translate(-CENTER_X, -CENTER_Y);
        innerGroup_->setTransform(t);

        qInfo() << "[Caesar] Rotation complete, now highlighting letters";

        // 阶段2：开始逐字母高亮
        highlightIndex_ = 0;
        highlightTimer_->start(animSpeed_);
        return;
    }

    // 平滑旋转
    QTransform t;
    t.translate(CENTER_X, CENTER_Y);
    t.rotate(currentRotation_);
    t.translate(-CENTER_X, -CENTER_Y);
    innerGroup_->setTransform(t);
}

void CaesarScene::animateHighlight() {
    if (highlightIndex_ >= inputText_.size()) {
        highlightTimer_->stop();
        // 显示结果文字，居中
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
    // 重置所有颜色
    for (auto* letter : outerLetters_) {
        letter->setDefaultTextColor(QColor(0, 200, 255));
        letter->setGraphicsEffect(nullptr);
    }
    for (auto* letter : innerLetters_) {
        letter->setDefaultTextColor(QColor(255, 100, 50));
        letter->setGraphicsEffect(nullptr);
    }

    // 高亮配对的字母 - 金色 + 强发光
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
    innerGroup_ = nullptr;
    resultText_ = nullptr;
    currentStep_ = 0;
    currentRotation_ = 0;
    highlightIndex_ = 0;
}

void CaesarScene::setSpeed(int ms) {
    animSpeed_ = ms;
    if (highlightTimer_->isActive()) {
        highlightTimer_->setInterval(ms);
    }
}
