#pragma once
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QTimer>
#include <QVector>
#include <QEasingCurve>

class CaesarScene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit CaesarScene(QObject* parent = nullptr);
    void startAnimation(const QString& text, int shift);
    void reset();
    void setSpeed(int ms);

signals:
    void animationComplete();

private slots:
    void animateRotation();
    void animateHighlight();
    void animatePulse();

private:
    void drawOuterRing();
    void drawInnerRing();
    void setInnerRingRotation(double deg);
    void highlightPair(int outerIdx, int innerIdx);
    void clearHighlights();
    void drawConnectionLine(int outerIdx, int innerIdx);
    void clearConnectionLines();

    QGraphicsEllipseItem* outerRing_ = nullptr;
    QGraphicsEllipseItem* innerRing_ = nullptr;
    QVector<QGraphicsTextItem*> outerLetters_;
    QVector<QGraphicsTextItem*> innerLetters_;
    QGraphicsTextItem* resultText_ = nullptr;
    QVector<QGraphicsLineItem*> connectionLines_;

    QTimer* rotateTimer_ = nullptr;
    QTimer* highlightTimer_ = nullptr;
    QTimer* pulseTimer_ = nullptr;

    QString inputText_;
    int shift_ = 0;
    int animSpeed_ = 500;
    double currentRotation_ = 0;
    double targetRotation_ = 0;
    int rotateTotalSteps_ = 40;
    int rotateCurrentStep_ = 0;

    int highlightIndex_ = 0;
    bool highlightShowing_ = false;

    // 脉冲动画
    double pulsePhase_ = 0;
    double outerGlowBase_ = 20;
    double innerGlowBase_ = 15;
};
