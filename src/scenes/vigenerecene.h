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
    void onRotateStepTick();
    void onRotateStepAnimTick();
    void onLetterTick();
    void animatePulse();

private:
    void drawRings();
    void setInnerRingRotation(double deg);
    void clearHighlights();
    void showExplanation(const QString& text);
    void startLetterRotation(int targetShift);
    void highlightPair(int outerIdx, int innerIdx);

    QGraphicsEllipseItem* outerRing_ = nullptr;
    QGraphicsEllipseItem* innerRing_ = nullptr;
    QVector<QGraphicsTextItem*> outerLetters_;
    QVector<QGraphicsTextItem*> innerLetters_;
    QGraphicsTextItem* resultText_ = nullptr;
    QGraphicsTextItem* keywordDisplay_ = nullptr;
    QGraphicsTextItem* explanation_ = nullptr;

    QTimer* rotateStepTimer_ = nullptr;
    QTimer* rotateStepAnimTimer_ = nullptr;
    QTimer* letterTimer_ = nullptr;
    QTimer* pulseTimer_ = nullptr;

    QString inputText_;
    QString keyword_;
    QVector<int> shifts_;
    int animSpeed_ = 800;

    // 当前正在处理的字母索引
    int currentLetterIndex_ = 0;

    // 逐格旋转状态
    double currentRotation_ = 0;
    int rotateStepIndex_ = 0;
    int rotateStepTotal_ = 0;
    double stepFromDeg_ = 0;
    double stepToDeg_ = 0;
    int stepAnimFrame_ = 0;
    static const int STEP_ANIM_FRAMES = 12;
    static const int STEP_INTERVAL_MS = 300;

    // 高亮状态
    bool highlightShowing_ = false;

    // 脉冲
    double pulsePhase_ = 0;

    // 动画保护
    int animationId_ = 0;

    // 结果
    QString resultString_;
};
