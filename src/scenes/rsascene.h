#pragma once
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QVector>

class RSAScene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit RSAScene(QObject* parent = nullptr);
    void startKeyGen(int64_t p, int64_t q);
    void startEncrypt(int64_t message, int64_t e, int64_t n);
    void startDecrypt(int64_t cipher, int64_t d, int64_t n);
    void reset();
    void setSpeed(int ms);

private slots:
    void animateStep();

private:
    struct StepWidget {
        QGraphicsTextItem* label;
        QGraphicsTextItem* formula;
        QGraphicsTextItem* value;
        QGraphicsRectItem* bg;
    };

    void addStep(const QString& desc, const QString& formula, const QString& value);
    void drawFlowArrow(double y);

    QVector<StepWidget> steps_;
    QTimer* timer_ = nullptr;
    QVector<QPair<QString, QPair<QString, QString>>> pendingSteps_;
    int currentStep_ = 0;
    int animSpeed_ = 800;
};
