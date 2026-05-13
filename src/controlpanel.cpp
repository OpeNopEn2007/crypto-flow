#include "controlpanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QStackedWidget>
#include <QFont>
#include <QMessageBox>
#include <QtMath>

static bool isPrime(int64_t n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

ControlPanel::ControlPanel(QWidget* parent) : QWidget(parent) {
    setFixedWidth(280);
    auto* layout = new QVBoxLayout(this);
    layout->setSpacing(12);
    layout->setContentsMargins(12, 12, 12, 12);

    auto* title = new QLabel("CryptoFlow");
    title->setFont(QFont("PingFang SC", 18, QFont::Bold));
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("titleLabel");
    layout->addWidget(title);

    algoCombo_ = new QComboBox;
    algoCombo_->addItems({"凯撒密码", "RSA 加密", "维吉尼亚密码", "Base64 编码", "XOR 加密"});
    algoCombo_->setFont(QFont("PingFang SC", 12));
    connect(algoCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ControlPanel::onAlgorithmChanged);
    layout->addWidget(algoCombo_);

    stack_ = new QStackedWidget;
    setupCaesarPanel();
    setupRSAPanel();
    setupVigenerePanel();
    setupBase64Panel();
    setupXORPanel();
    layout->addWidget(stack_);

    // Speed
    auto* speedGroup = new QGroupBox("动画速度");
    auto* speedLayout = new QHBoxLayout(speedGroup);
    speedSlider_ = new QSlider(Qt::Horizontal);
    speedSlider_->setRange(200, 2000);
    speedSlider_->setValue(1000);
    speedSlider_->setInvertedAppearance(true);
    speedLabel_ = new QLabel("1000ms");
    connect(speedSlider_, &QSlider::valueChanged, this, [this](int val) {
        speedLabel_->setText(QString("%1ms").arg(val));
        emit speedChanged(val);
    });
    speedLayout->addWidget(speedSlider_);
    speedLayout->addWidget(speedLabel_);
    layout->addWidget(speedGroup);

    startBtn_ = new QPushButton("开始演示");
    startBtn_->setObjectName("startBtn");
    startBtn_->setFont(QFont("PingFang SC", 13, QFont::Bold));
    connect(startBtn_, &QPushButton::clicked, this, &ControlPanel::onStartClicked);
    layout->addWidget(startBtn_);

    resetBtn_ = new QPushButton("重置");
    resetBtn_->setObjectName("resetBtn");
    connect(resetBtn_, &QPushButton::clicked, this, &ControlPanel::resetRequested);
    layout->addWidget(resetBtn_);

    layout->addStretch();
}

void ControlPanel::setupCaesarPanel() {
    auto* panel = new QWidget;
    auto* layout = new QVBoxLayout(panel);
    auto* label = new QLabel("输入文本:");
    label->setFont(QFont("PingFang SC", 11));
    layout->addWidget(label);
    caesarInput_ = new QLineEdit("HELLO");
    caesarInput_->setFont(QFont("Menlo", 12));
    layout->addWidget(caesarInput_);
    auto* shiftLabel = new QLabel("移位量 (1-25):");
    shiftLabel->setFont(QFont("PingFang SC", 11));
    layout->addWidget(shiftLabel);
    caesarShift_ = new QSpinBox;
    caesarShift_->setRange(1, 25);
    caesarShift_->setValue(3);
    caesarShift_->setFont(QFont("Menlo", 12));
    layout->addWidget(caesarShift_);
    layout->addStretch();
    stack_->addWidget(panel);
}

void ControlPanel::setupVigenerePanel() {
    auto* panel = new QWidget;
    auto* layout = new QVBoxLayout(panel);
    auto* label = new QLabel("输入文本:");
    label->setFont(QFont("PingFang SC", 11));
    layout->addWidget(label);
    vigenereInput_ = new QLineEdit("HELLO");
    vigenereInput_->setFont(QFont("Menlo", 12));
    layout->addWidget(vigenereInput_);
    auto* keyLabel = new QLabel("密钥:");
    keyLabel->setFont(QFont("PingFang SC", 11));
    layout->addWidget(keyLabel);
    vigenereKey_ = new QLineEdit("KEY");
    vigenereKey_->setFont(QFont("Menlo", 12));
    layout->addWidget(vigenereKey_);
    layout->addStretch();
    stack_->addWidget(panel);
}

void ControlPanel::setupRSAPanel() {
    auto* panel = new QWidget;
    auto* layout = new QVBoxLayout(panel);

    auto* primeLabel = new QLabel("选择素数 p, q:");
    primeLabel->setFont(QFont("PingFang SC", 11));
    layout->addWidget(primeLabel);

    auto* primeLayout = new QHBoxLayout;
    rsaP_ = new QSpinBox;
    rsaP_->setRange(2, 997);
    rsaP_->setValue(61);
    rsaP_->setFont(QFont("Menlo", 12));
    rsaQ_ = new QSpinBox;
    rsaQ_->setRange(2, 997);
    rsaQ_->setValue(53);
    rsaQ_->setFont(QFont("Menlo", 12));
    primeLayout->addWidget(new QLabel("p:"));
    primeLayout->addWidget(rsaP_);
    primeLayout->addWidget(new QLabel("q:"));
    primeLayout->addWidget(rsaQ_);
    layout->addLayout(primeLayout);

    rsaMode_ = new QComboBox;
    rsaMode_->addItems({"生成密钥", "加密", "解密"});
    rsaMode_->setFont(QFont("PingFang SC", 11));
    connect(rsaMode_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int idx) {
        rsaMessage_->setVisible(idx > 0);
        rsaE_->setVisible(idx == 1);
        rsaN_->setVisible(idx > 0);
        rsaD_->setVisible(idx == 2);
        if (idx == 2) {
            rsaMessage_->setText(QString::number(lastCipher_));
        }
    });
    layout->addWidget(rsaMode_);

    auto* msgLabel = new QLabel("数值:");
    msgLabel->setFont(QFont("PingFang SC", 11));
    layout->addWidget(msgLabel);
    rsaMessage_ = new QLineEdit("42");
    rsaMessage_->setFont(QFont("Menlo", 12));
    layout->addWidget(rsaMessage_);

    auto* eLabel = new QLabel("公钥 e:");
    layout->addWidget(eLabel);
    rsaE_ = new QLineEdit("65537");
    rsaE_->setFont(QFont("Menlo", 12));
    layout->addWidget(rsaE_);

    auto* nLabel = new QLabel("模数 n:");
    layout->addWidget(nLabel);
    rsaN_ = new QLineEdit;
    rsaN_->setFont(QFont("Menlo", 12));
    rsaN_->setReadOnly(true);
    layout->addWidget(rsaN_);

    auto* dLabel = new QLabel("私钥 d:");
    layout->addWidget(dLabel);
    rsaD_ = new QLineEdit;
    rsaD_->setFont(QFont("Menlo", 12));
    rsaD_->setReadOnly(true);
    layout->addWidget(rsaD_);

    layout->addStretch();
    stack_->addWidget(panel);
}

void ControlPanel::setupBase64Panel() {
    auto* panel = new QWidget;
    auto* layout = new QVBoxLayout(panel);
    auto* label = new QLabel("输入文本:");
    label->setFont(QFont("PingFang SC", 11));
    layout->addWidget(label);
    base64Input_ = new QLineEdit("Hello");
    base64Input_->setFont(QFont("Menlo", 12));
    layout->addWidget(base64Input_);
    layout->addStretch();
    stack_->addWidget(panel);
}

void ControlPanel::setupXORPanel() {
    auto* panel = new QWidget;
    auto* layout = new QVBoxLayout(panel);
    auto* label = new QLabel("输入文本:");
    label->setFont(QFont("PingFang SC", 11));
    layout->addWidget(label);
    xorInput_ = new QLineEdit("Hello");
    xorInput_->setFont(QFont("Menlo", 12));
    layout->addWidget(xorInput_);
    auto* keyLabel = new QLabel("密钥:");
    keyLabel->setFont(QFont("PingFang SC", 11));
    layout->addWidget(keyLabel);
    xorKey_ = new QLineEdit("Key");
    xorKey_->setFont(QFont("Menlo", 12));
    layout->addWidget(xorKey_);
    layout->addStretch();
    stack_->addWidget(panel);
}

void ControlPanel::onAlgorithmChanged(int index) {
    stack_->setCurrentIndex(index);
    emit algorithmChanged(static_cast<Algorithm>(index));
}

void ControlPanel::setRSAValues(int64_t e, int64_t n, int64_t d) {
    rsaE_->setText(QString::number(e));
    rsaN_->setText(QString::number(n));
    rsaD_->setText(QString::number(d));
}

void ControlPanel::setLastCipher(int64_t cipher) {
    lastCipher_ = cipher;
}

void ControlPanel::setAlgorithm(int index) {
    algoCombo_->setCurrentIndex(index);
}

void ControlPanel::onStartClicked() {
    int idx = algoCombo_->currentIndex();
    if (idx == Caesar) {
        emit caesarStart(caesarInput_->text(), caesarShift_->value());
    } else if (idx == RSA) {
        int mode = rsaMode_->currentIndex();
        int64_t p = rsaP_->value();
        int64_t q = rsaQ_->value();
        if (mode == 0) {
            if (!isPrime(p) || !isPrime(q)) {
                QMessageBox::warning(this, "输入错误",
                    QString("p 和 q 必须是素数!\n当前: p=%1, q=%2").arg(p).arg(q));
                return;
            }
            if (p == q) {
                QMessageBox::warning(this, "输入错误", "p 和 q 不能相同!");
                return;
            }
            if (p < 3 || q < 3) {
                QMessageBox::warning(this, "输入错误", "p 和 q 必须 >= 3!");
                return;
            }
            emit rsaKeyGen(p, q);
        } else if (mode == 1) {
            bool ok = false;
            int64_t msg = rsaMessage_->text().toLongLong(&ok);
            if (!ok) { QMessageBox::warning(this, "输入错误", "明文必须是数字"); return; }
            int64_t n = rsaN_->text().toLongLong(&ok);
            if (!ok || n <= 0) { QMessageBox::warning(this, "输入错误", "请先生成密钥 (n > 0)"); return; }
            if (msg < 0 || msg >= n) {
                QMessageBox::warning(this, "输入错误",
                    QString("明文 M 必须满足 0 <= M < n!\n当前: M=%1, n=%2").arg(msg).arg(n));
                return;
            }
            int64_t e = rsaE_->text().toLongLong(&ok);
            if (!ok || e <= 0) { QMessageBox::warning(this, "输入错误", "公钥 e 必须是正整数"); return; }
            emit rsaEncrypt(msg, e, n);
        } else {
            bool ok = false;
            int64_t cipher = rsaMessage_->text().toLongLong(&ok);
            if (!ok) { QMessageBox::warning(this, "输入错误", "密文必须是数字"); return; }
            int64_t n = rsaN_->text().toLongLong(&ok);
            if (!ok || n <= 0) { QMessageBox::warning(this, "输入错误", "请先生成密钥 (n > 0)"); return; }
            int64_t d = rsaD_->text().toLongLong(&ok);
            if (!ok || d <= 0) { QMessageBox::warning(this, "输入错误", "私钥 d 必须是正整数"); return; }
            if (cipher < 0 || cipher >= n) {
                QMessageBox::warning(this, "输入错误",
                    QString("密文 C 必须满足 0 <= C < n!\n当前: C=%1, n=%2").arg(cipher).arg(n));
                return;
            }
            emit rsaDecrypt(cipher, d, n);
        }
    } else if (idx == Vigenere) {
        if (vigenereKey_->text().isEmpty()) {
            QMessageBox::warning(this, "输入错误", "密钥不能为空!");
            return;
        }
        if (vigenereInput_->text().isEmpty()) {
            QMessageBox::warning(this, "输入错误", "输入文本不能为空!");
            return;
        }
        emit vigenereStart(vigenereInput_->text(), vigenereKey_->text());
    } else if (idx == Base64) {
        if (base64Input_->text().isEmpty()) {
            QMessageBox::warning(this, "输入错误", "输入文本不能为空!");
            return;
        }
        emit base64Start(base64Input_->text());
    } else if (idx == XOR) {
        if (xorKey_->text().isEmpty()) {
            QMessageBox::warning(this, "输入错误", "密钥不能为空!");
            return;
        }
        if (xorInput_->text().isEmpty()) {
            QMessageBox::warning(this, "输入错误", "输入文本不能为空!");
            return;
        }
        emit xorStart(xorInput_->text(), xorKey_->text());
    }
}
