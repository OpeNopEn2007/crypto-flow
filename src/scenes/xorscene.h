#pragma once
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QVector>
#include <QEasingCurve>

class XORScene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit XORScene(QObject* parent = nullptr);
    void startAnimation(const QString& text, const QString& key);
    void reset();
    void setSpeed(int ms);

signals:
    void animationComplete();

private slots:
    void onAnimFrame();
    void onPhaseDelay();

private:
    struct DataBlock {
        QGraphicsRectItem* bg = nullptr;
        QGraphicsTextItem* text = nullptr;
        QGraphicsTextItem* label = nullptr;
        double targetX = 0;
        double targetY = 0;
        QColor color;
    };

    void setupPipeline();
    void clearDataBlocks();
    void addDataBlock(double x, double y, const QString& text, QColor color,
                      const QString& label = "");
    void addFlowArrow(double x1, double y1, double x2, double y2, QColor color);
    void addExplanation(const QString& text);
    QGraphicsTextItem* addCenteredText(const QString& text, const QFont& font,
                                       QColor color, double y);

    QTimer* animTimer_ = nullptr;
    QTimer* phaseTimer_ = nullptr;
    int animFrame_ = 0;
    static const int ANIM_FRAMES = 20;
    int currentPhase_ = 0;
    int animSpeed_ = 1000;
    int animationId_ = 0;

    QString inputText_;
    QString keyText_;
    QByteArray inputBytes_;
    QByteArray keyBytes_;
    QByteArray resultBytes_;

    QVector<DataBlock> dataBlocks_;
    QGraphicsTextItem* explanation_ = nullptr;
    QGraphicsTextItem* title_ = nullptr;
};
