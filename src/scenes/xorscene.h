#pragma once
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
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
    void animateStep();

private:
    struct StepWidget {
        QGraphicsTextItem* label;
        QGraphicsTextItem* formula;
        QGraphicsTextItem* value;
        QGraphicsRectItem* bg;
        QGraphicsLineItem* arrow;
    };

    void addStep(const QString& desc, const QString& formula, const QString& value,
                 QColor accent = QColor(0, 200, 255));

    QVector<StepWidget> steps_;
    QTimer* timer_ = nullptr;
    QVector<QPair<QString, QPair<QString, QString>>> pendingSteps_;
    QVector<QColor> pendingAccents_;
    int currentStep_ = 0;
    int animSpeed_ = 1000;
};
