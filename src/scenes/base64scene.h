#pragma once
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QVector>

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
    void onPhaseTick();
    void animatePulse();

private:
    void setupTitle();
    void clearPhase();
    QGraphicsTextItem* addCenteredText(const QString& text, const QFont& font,
                                       QColor color, double y);
    QGraphicsRectItem* addDataBox(double x, double y, double w, double h,
                                   QColor borderColor, QColor bgColor);
    void addFlowArrow(double x1, double y1, double x2, double y2, QColor color);
    QGraphicsTextItem* addLabel(double x, double y, const QString& text,
                                 QFont font, QColor color);

    QTimer* phaseTimer_ = nullptr;
    QTimer* pulseTimer_ = nullptr;

    int currentPhase_ = 0;
    int totalPhases_ = 0;
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
    QVector<QGraphicsItem*> phaseItems_;
    double pulsePhase_ = 0;
};
