#pragma once
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QVector>

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
    void onPhaseTick();

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

    int currentPhase_ = 0;
    int totalPhases_ = 0;
    int animSpeed_ = 1000;
    int animationId_ = 0;

    // 数据
    QString inputText_;
    QString keyText_;
    QByteArray inputBytes_;
    QByteArray keyBytes_;
    QByteArray resultBytes_;

    QVector<QGraphicsItem*> phaseItems_;
};
