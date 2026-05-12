#include "rsascene.h"
#include "../crypto/rsa.h"
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QDebug>

RSAScene::RSAScene(QObject* parent)
    : QGraphicsScene(parent), timer_(new QTimer(this)) {
    setSceneRect(0, 0, 600, 500);
    connect(timer_, &QTimer::timeout, this, &RSAScene::animateStep);
}

void RSAScene::addStep(const QString& desc, const QString& formula, const QString& value) {
    double y = 20 + steps_.size() * 60;

    auto* bg = addRect(10, y, 580, 50,
                        QPen(QColor(0, 200, 255, 80)),
                        QBrush(QColor(30, 40, 60, 200)));
    bg->setZValue(0);

    auto* label = addText(desc, QFont("PingFang SC", 12));
    label->setPos(20, y + 5);
    label->setDefaultTextColor(QColor(200, 200, 200));
    label->setZValue(1);

    auto* formulaItem = addText(formula, QFont("Menlo", 13, QFont::Bold));
    formulaItem->setPos(20, y + 25);
    formulaItem->setDefaultTextColor(QColor(0, 200, 255));
    formulaItem->setZValue(1);

    auto* valueItem = addText(value, QFont("Menlo", 13, QFont::Bold));
    valueItem->setPos(400, y + 25);
    valueItem->setDefaultTextColor(QColor(0, 255, 150));
    valueItem->setZValue(1);

    steps_.append({label, formulaItem, valueItem, bg});
}

void RSAScene::drawFlowArrow(double y) {
    auto* arrow = addLine(300, y, 300, y + 10,
                           QPen(QColor(0, 200, 255, 100), 1, Qt::DashLine));
    arrow->setZValue(0);
}

void RSAScene::startKeyGen(int64_t p, int64_t q) {
    reset();
    qInfo() << "[RSA] Starting key generation, p:" << p << "q:" << q;

    auto steps = RSAEngine::keyGenSteps(p, q);
    for (const auto& step : steps) {
        pendingSteps_.append({step.description, {step.formula, QString::number(step.value)}});
        qDebug() << "[RSA] Step:" << step.description << "=" << step.value;
    }

    timer_->start(animSpeed_);
}

void RSAScene::startEncrypt(int64_t message, int64_t e, int64_t n) {
    reset();

    int64_t cipher = RSAEngine::encrypt(message, e, n);
    pendingSteps_.append({"明文 M", {QString("M = %1").arg(message), ""}});
    pendingSteps_.append({"加密: C = M^e mod n",
                           {QString("C = %1^%2 mod %3").arg(message).arg(e).arg(n),
                            QString("C = %1").arg(cipher)}});

    timer_->start(animSpeed_);
}

void RSAScene::startDecrypt(int64_t cipher, int64_t d, int64_t n) {
    reset();

    int64_t message = RSAEngine::decrypt(cipher, d, n);
    pendingSteps_.append({"密文 C", {QString("C = %1").arg(cipher), ""}});
    pendingSteps_.append({"解密: M = C^d mod n",
                           {QString("M = %1^%2 mod %3").arg(cipher).arg(d).arg(n),
                            QString("M = %1").arg(message)}});

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
    addStep(step.first, step.second.first, step.second.second);
    qDebug() << "[RSA] Step" << currentStep_ << ":" << step.first;
    currentStep_++;
}

void RSAScene::reset() {
    timer_->stop();
    clear();
    steps_.clear();
    pendingSteps_.clear();
    currentStep_ = 0;
}

void RSAScene::setSpeed(int ms) {
    animSpeed_ = ms;
    if (timer_->isActive()) {
        timer_->setInterval(ms);
    }
}
