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
    void onStepTick();
    void onStepAnimTick();
    void animateHighlight();
    void animatePulse();
    void animateConnectionLine();

private:
    void drawOuterRing();
    void drawInnerRing();
    void setInnerRingRotation(double deg);
    void highlightPair(int outerIdx, int innerIdx);
    void clearHighlights();
    void startConnectionLineAnimation(int outerIdx, int innerIdx);
    void clearConnectionLines();

    QGraphicsEllipseItem* outerRing_ = nullptr;
    QGraphicsEllipseItem* innerRing_ = nullptr;
    QVector<QGraphicsTextItem*> outerLetters_;
    QVector<QGraphicsTextItem*> innerLetters_;
    QGraphicsTextItem* resultText_ = nullptr;
    QVector<QGraphicsItem*> connectionLines_;

    QTimer* stepTimer_ = nullptr;      // 控制每格跳动的间隔
    QTimer* stepAnimTimer_ = nullptr;  // 控制每格内部的过渡动画
    QTimer* highlightTimer_ = nullptr;
    QTimer* pulseTimer_ = nullptr;
    QTimer* connectionAnimTimer_ = nullptr;

    QString inputText_;
    int shift_ = 0;
    int animSpeed_ = 800;

    // 逐格旋转
    double currentRotation_ = 0; // 当前总旋转角度
    int rotateStepIndex_ = 0;   // 当前第几格
    int rotateStepTotal_ = 0;   // 总共几格
    double stepFromDeg_ = 0;    // 当前格起始角度
    double stepToDeg_ = 0;      // 当前格目标角度
    int stepAnimFrame_ = 0;     // 当前格内动画帧
    static const int STEP_ANIM_FRAMES = 12; // 每格过渡帧数
    static const int STEP_INTERVAL_MS = 300; // 每格之间的间隔

    int highlightIndex_ = 0;
    bool highlightShowing_ = false;
    int animationId_ = 0;

    // 连接线动画
    double connAnimProgress_ = 0;
    int connAnimOuterIdx_ = -1;
    int connAnimInnerIdx_ = -1;
    double connStartX_ = 0, connStartY_ = 0;
    double connEndX_ = 0, connEndY_ = 0;
    static const int CONN_ANIM_STEPS = 12;

    // 脉冲动画
    double pulsePhase_ = 0;
    double outerGlowBase_ = 20;
    double innerGlowBase_ = 15;

    // 结果打字机效果
    int resultCharIndex_ = 0;
    QString resultTarget_;
    bool typewriterActive_ = false;
};
