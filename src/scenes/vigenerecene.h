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
    void onRewindTick();
    void onLetterTick();
    void animatePulse();

private:
    void drawRings();
    void setInnerRingRotation(double deg);
    void launchAnimation();
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
    QTimer* rewindTimer_ = nullptr;
    QTimer* letterTimer_ = nullptr;
    QTimer* pulseTimer_ = nullptr;

    QString inputText_;
    QString keyword_;
    QString pendingText_;
    QString pendingKeyword_;
    QVector<int> shifts_;
    int animSpeed_ = 1000;

    // 当前正在处理的字母索引
    int currentLetterIndex_ = 0;
    int keywordIndex_ = 0;  // 关键词位置（仅字母递增）

    // 逐格旋转状态
    double currentRotation_ = 0;
    int rotateStepIndex_ = 0;
    int rotateStepTotal_ = 0;
    int rotateDirection_ = 1;  // 1=逆时针(+), -1=顺时针(-)
    double stepFromDeg_ = 0;
    double stepToDeg_ = 0;
    int stepAnimFrame_ = 0;
    static const int STEP_ANIM_FRAMES = 12;
    static const int STEP_INTERVAL_MS = 500;

    // 高亮状态
    bool highlightShowing_ = false;

    // 脉冲
    double pulsePhase_ = 0;

    // 回转动画
    double rewindFromDeg_ = 0;
    int rewindFrame_ = 0;
    static const int REWIND_FRAMES = 30;

    // 动画保护
    int animationId_ = 0;
    int activeAnimId_ = -1;  // 当前活跃动画 ID，onLetterTick 入口校验

    // 结果
    QString resultString_;
};
