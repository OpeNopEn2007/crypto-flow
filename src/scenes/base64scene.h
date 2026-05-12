#pragma once
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QVector>
#include <QEasingCurve>

class Base64Scene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit Base64Scene(QObject* parent = nullptr);
    void startAnimation(const QString& text);
    void reset();
    void setSpeed(int ms);

signals:
    void animationComplete();

private slots:
    void onAnimFrame();
    void onPhaseDelay();

private:
    // 数据项：场景中可移动的数据块
    struct DataBlock {
        QGraphicsRectItem* bg = nullptr;
        QGraphicsTextItem* text = nullptr;
        QGraphicsTextItem* label = nullptr;
        double targetX = 0;
        double targetY = 0;
        QColor color;
    };

    // 处理站：流水线上的工位
    struct Station {
        QGraphicsRectItem* bg = nullptr;
        QGraphicsTextItem* title = nullptr;
        QGraphicsTextItem* desc = nullptr;
    };

    void setupPipeline();
    void clearDataBlocks();
    void addDataBlock(double x, double y, const QString& text, QColor color,
                      const QString& label = "");
    void addStation(int index, const QString& title, const QString& desc);
    void addFlowArrow(double x1, double y1, double x2, double y2, QColor color);
    void addExplanation(const QString& text);
    QGraphicsTextItem* addCenteredText(const QString& text, const QFont& font,
                                       QColor color, double y);

    // 动画
    QTimer* animTimer_ = nullptr;
    QTimer* phaseTimer_ = nullptr;
    int animFrame_ = 0;
    static const int ANIM_FRAMES = 20;
    int currentPhase_ = 0;
    int animSpeed_ = 1000;
    int animationId_ = 0;

    // 数据
    QString inputText_;
    QByteArray inputBytes_;
    QString binaryString_;
    QVector<QString> sixBitGroups_;
    QVector<int> groupValues_;
    QString b64Result_;

    // 场景元素
    QVector<DataBlock> dataBlocks_;
    Station stations_[3];
    QGraphicsTextItem* explanation_ = nullptr;
    QGraphicsTextItem* title_ = nullptr;

    // 管道线
    QGraphicsLineItem* pipeLine_ = nullptr;
};
