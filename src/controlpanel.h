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

    enum Algorithm { Caesar, RSA, Vigenere, Base64, XOR };

signals:
    void algorithmChanged(Algorithm algo);
    void caesarStart(const QString& text, int shift);
    void vigenereStart(const QString& text, const QString& keyword);
    void rsaKeyGen(int64_t p, int64_t q);
    void rsaEncrypt(int64_t message, int64_t e, int64_t n);
    void rsaDecrypt(int64_t cipher, int64_t d, int64_t n);
    void base64Start(const QString& text);
    void xorStart(const QString& text, const QString& key);
    void resetRequested();
    void speedChanged(int ms);

private slots:
    void onAlgorithmChanged(int index);
    void onStartClicked();

private:
    void setupCaesarPanel();
    void setupVigenerePanel();
    void setupRSAPanel();
    void setupBase64Panel();
    void setupXORPanel();

    QComboBox* algoCombo_ = nullptr;
    QStackedWidget* stack_ = nullptr;

    // Caesar
    QLineEdit* caesarInput_ = nullptr;
    QSpinBox* caesarShift_ = nullptr;

    // Vigenere
    QLineEdit* vigenereInput_ = nullptr;
    QLineEdit* vigenereKey_ = nullptr;

    // RSA
    QSpinBox* rsaP_ = nullptr;
    QSpinBox* rsaQ_ = nullptr;
    QLineEdit* rsaMessage_ = nullptr;
    QLineEdit* rsaE_ = nullptr;
    QLineEdit* rsaN_ = nullptr;
    QLineEdit* rsaD_ = nullptr;
    QComboBox* rsaMode_ = nullptr;

    // Base64
    QLineEdit* base64Input_ = nullptr;

    // XOR
    QLineEdit* xorInput_ = nullptr;
    QLineEdit* xorKey_ = nullptr;

    QPushButton* startBtn_ = nullptr;
    QPushButton* resetBtn_ = nullptr;
    QSlider* speedSlider_ = nullptr;
    QLabel* speedLabel_ = nullptr;
};
