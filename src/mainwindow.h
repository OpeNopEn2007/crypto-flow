#pragma once
#include <QMainWindow>
#include <QGraphicsView>

class ControlPanel;
class CaesarScene;
class RSAScene;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    void autoStartCaesar(const QString& text, int shift);
    void autoStartRSA();

private slots:
    void switchToCaesar();
    void switchToRSA();
    void onCaesarStart(const QString& text, int shift);
    void onRSAKeyGen(int64_t p, int64_t q);
    void onRSAEncrypt(int64_t message, int64_t e, int64_t n);
    void onRSADecrypt(int64_t cipher, int64_t d, int64_t n);
    void onReset();
    void onSpeedChanged(int ms);
    void saveScreenshot(const QString& prefix);

private:
    void setupUI();
    void setupLogging();

    QGraphicsView* view_ = nullptr;
    ControlPanel* controlPanel_ = nullptr;
    CaesarScene* caesarScene_ = nullptr;
    RSAScene* rsaScene_ = nullptr;
};
