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

private slots:
    void animateStep();

private:
    void drawOuterRing();
    void drawInnerRing();
    void highlightLetter(int index);

    QGraphicsEllipseItem* outerRing_ = nullptr;
    QGraphicsEllipseItem* innerRing_ = nullptr;
    QVector<QGraphicsTextItem*> outerLetters_;
    QVector<QGraphicsTextItem*> innerLetters_;
    QGraphicsTextItem* resultText_ = nullptr;

    QTimer* timer_ = nullptr;
    QString inputText_;
    int shift_ = 0;
    int currentStep_ = 0;
    int animSpeed_ = 300;
    double currentRotation_ = 0;
};
