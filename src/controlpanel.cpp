#include "controlpanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QStackedWidget>
#include <QFont>

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
    auto* keyLabel = new QLabel("关键词 (字母):");
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

void ControlPanel::setRSAValues(int64_t n, int64_t d) {
    rsaN_->setText(QString::number(n));
    rsaD_->setText(QString::number(d));
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
            emit rsaKeyGen(p, q);
        } else if (mode == 1) {
            emit rsaEncrypt(rsaMessage_->text().toLongLong(),
                           rsaE_->text().toLongLong(),
                           rsaN_->text().toLongLong());
        } else {
            emit rsaDecrypt(rsaMessage_->text().toLongLong(),
                           rsaD_->text().toLongLong(),
                           rsaN_->text().toLongLong());
        }
    } else if (idx == Vigenere) {
        emit vigenereStart(vigenereInput_->text(), vigenereKey_->text());
    } else if (idx == Base64) {
        emit base64Start(base64Input_->text());
    } else if (idx == XOR) {
        emit xorStart(xorInput_->text(), xorKey_->text());
    }
}
