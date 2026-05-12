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
    void animateStep();

private:
    void addCard(int step, const QString& title, const QString& detail,
                 const QString& value, QColor accent);

    QTimer* timer_ = nullptr;
    QVector<QPair<QString, QPair<QString, QString>>> pendingSteps_;
    QVector<QColor> pendingAccents_;
    int currentStep_ = 0;
    int animSpeed_ = 800;
};
