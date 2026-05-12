#pragma once
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QVector>

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

private:
    void drawOuterRing();
    void drawInnerRing();
    void setInnerRingRotation(double deg);
    void highlightPair(int outerIdx, int innerIdx);

    QGraphicsEllipseItem* outerRing_ = nullptr;
    QGraphicsEllipseItem* innerRing_ = nullptr;
    QVector<QGraphicsTextItem*> outerLetters_;
    QVector<QGraphicsTextItem*> innerLetters_;
    QGraphicsTextItem* resultText_ = nullptr;

    QTimer* rotateTimer_ = nullptr;
    QTimer* highlightTimer_ = nullptr;

    QString inputText_;
    int shift_ = 0;
    int animSpeed_ = 500;
    double currentRotation_ = 0;
    double targetRotation_ = 0;
    double rotateStep_ = 0;

    int highlightIndex_ = 0;
    bool highlightShowing_ = false;  // true=亮, false=灭
};
