#pragma once
#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QStackedWidget>

class ControlPanel : public QWidget {
    Q_OBJECT
public:
    explicit ControlPanel(QWidget* parent = nullptr);
    void setRSAValues(int64_t n, int64_t d);

    enum Algorithm { Caesar, RSA };

signals:
    void algorithmChanged(Algorithm algo);
    void caesarStart(const QString& text, int shift);
    void rsaKeyGen(int64_t p, int64_t q);
    void rsaEncrypt(int64_t message, int64_t e, int64_t n);
    void rsaDecrypt(int64_t cipher, int64_t d, int64_t n);
    void resetRequested();
    void speedChanged(int ms);

private slots:
    void onAlgorithmChanged(int index);
    void onStartClicked();

private:
    void setupCaesarPanel();
    void setupRSAPanel();
    void showCaesarPanel(bool show);

    QComboBox* algoCombo_ = nullptr;
    QStackedWidget* stack_ = nullptr;

    // Caesar controls
    QLineEdit* caesarInput_ = nullptr;
    QSpinBox* caesarShift_ = nullptr;

    // RSA controls
    QSpinBox* rsaP_ = nullptr;
    QSpinBox* rsaQ_ = nullptr;
    QLineEdit* rsaMessage_ = nullptr;
    QLineEdit* rsaE_ = nullptr;
    QLineEdit* rsaN_ = nullptr;
    QLineEdit* rsaD_ = nullptr;
    QComboBox* rsaMode_ = nullptr;

    QPushButton* startBtn_ = nullptr;
    QPushButton* resetBtn_ = nullptr;
    QSlider* speedSlider_ = nullptr;
    QLabel* speedLabel_ = nullptr;
};
