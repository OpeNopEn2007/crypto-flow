#pragma once
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QVector>
#include <QEasingCurve>

class VigenereScene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit VigenereScene(QObject* parent = nullptr);
    void startAnimation(const QString& text, const QString& keyword);
    void reset();
    void setSpeed(int ms);

signals:
    void animationComplete();

private slots:
    void onStepTick();
    void onStepAnimTick();
    void animateHighlight();
    void animatePulse();

private:
    void drawRings();
    void setInnerRingRotation(double deg);

    QGraphicsEllipseItem* outerRing_ = nullptr;
    QGraphicsEllipseItem* innerRing_ = nullptr;
    QVector<QGraphicsTextItem*> outerLetters_;
    QVector<QGraphicsTextItem*> innerLetters_;
    QGraphicsTextItem* resultText_ = nullptr;
    QGraphicsTextItem* keywordDisplay_ = nullptr;

    QTimer* stepTimer_ = nullptr;
    QTimer* stepAnimTimer_ = nullptr;
    QTimer* highlightTimer_ = nullptr;
    QTimer* pulseTimer_ = nullptr;

    QString inputText_;
    QString keyword_;
    QVector<int> shifts_;
    int animSpeed_ = 800;

    // 逐格旋转
    double currentRotation_ = 0;
    int rotateStepIndex_ = 0;
    int rotateStepTotal_ = 0;
    double stepFromDeg_ = 0;
    double stepToDeg_ = 0;
    int stepAnimFrame_ = 0;
    static const int STEP_ANIM_FRAMES = 12;
    static const int STEP_INTERVAL_MS = 400;

    int highlightIndex_ = 0;
    bool highlightShowing_ = false;
    int animationId_ = 0;

    // 脉冲
    double pulsePhase_ = 0;

    // 结果
    int resultCharIndex_ = 0;
    QString resultTarget_;
    bool typewriterActive_ = false;
};
