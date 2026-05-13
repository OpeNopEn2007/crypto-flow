#include "mainwindow.h"
#include "controlpanel.h"
#include "scenes/caesarscene.h"
#include "scenes/rsascene.h"
#include "scenes/vigenerecene.h"
#include "scenes/base64scene.h"
#include "scenes/xorscene.h"
#include "crypto/rsa.h"
#include <QSplitter>
#include <QStatusBar>
#include <QFile>
#include <QDateTime>
#include <QDebug>
#include <QTimer>

static QFile* g_logFile = nullptr;

void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
    QByteArray localMsg = msg.toLocal8Bit();
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    const char* file = context.file ? context.file : "";
    const char* function = context.function ? context.function : "";

    const char* level = "";
    switch (type) {
    case QtDebugMsg:    level = "DEBUG"; break;
    case QtWarningMsg:  level = "WARN "; break;
    case QtCriticalMsg: level = "CRIT "; break;
    case QtFatalMsg:    level = "FATAL"; break;
    case QtInfoMsg:     level = "INFO "; break;
    }

    fprintf(stderr, "[%s] [%s] %s (%s:%d)\n",
            timestamp.toUtf8().constData(), level, localMsg.constData(), file, context.line);

    if (g_logFile && g_logFile->isOpen()) {
        QString logLine = QString("[%1] [%2] %3 (%4:%5)\n")
            .arg(timestamp, level, msg, file).arg(context.line);
        g_logFile->write(logLine.toUtf8());
        g_logFile->flush();
    }

    if (type == QtFatalMsg) abort();
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupLogging();
    setupUI();
    qInfo() << "CryptoFlow started";
}

void MainWindow::setupLogging() {
    g_logFile = new QFile("cryptoflow.log");
    (void)g_logFile->open(QIODevice::WriteOnly | QIODevice::Append);
    qInstallMessageHandler(messageHandler);
}

void MainWindow::setupUI() {
    setWindowTitle("CryptoFlow - 交互式密码学可视化");
    resize(1200, 700);

    caesarScene_ = new CaesarScene(this);
    rsaScene_ = new RSAScene(this);
    vigenereScene_ = new VigenereScene(this);
    base64Scene_ = new Base64Scene(this);
    xorScene_ = new XORScene(this);

    view_ = new QGraphicsView;
    view_->setRenderHint(QPainter::Antialiasing);
    view_->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view_->setStyleSheet("background: #1a1a2e; border: none;");
    view_->setScene(caesarScene_);

    controlPanel_ = new ControlPanel;

    auto* splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(view_);
    splitter->addWidget(controlPanel_);
    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 1);
    splitter->setHandleWidth(2);
    setCentralWidget(splitter);

    // 连接信号
    connect(controlPanel_, &ControlPanel::caesarStart, this, &MainWindow::onCaesarStart);
    connect(controlPanel_, &ControlPanel::vigenereStart, this, &MainWindow::onVigenereStart);
    connect(controlPanel_, &ControlPanel::rsaKeyGen, this, &MainWindow::onRSAKeyGen);
    connect(controlPanel_, &ControlPanel::rsaEncrypt, this, &MainWindow::onRSAEncrypt);
    connect(controlPanel_, &ControlPanel::rsaDecrypt, this, &MainWindow::onRSADecrypt);
    connect(controlPanel_, &ControlPanel::base64Start, this, &MainWindow::onBase64Start);
    connect(controlPanel_, &ControlPanel::xorStart, this, &MainWindow::onXORStart);
    connect(controlPanel_, &ControlPanel::resetRequested, this, &MainWindow::onReset);
    connect(controlPanel_, &ControlPanel::speedChanged, this, &MainWindow::onSpeedChanged);

    connect(caesarScene_, &CaesarScene::animationComplete,
            this, [this]() { saveScreenshot("caesar"); });
    connect(rsaScene_, &RSAScene::animationComplete,
            this, [this]() { saveScreenshot("rsa"); });
    connect(vigenereScene_, &VigenereScene::animationComplete,
            this, [this]() { saveScreenshot("vigenere"); });
    connect(base64Scene_, &Base64Scene::animationComplete,
            this, [this]() { saveScreenshot("base64"); });
    connect(xorScene_, &XORScene::animationComplete,
            this, [this]() { saveScreenshot("xor"); });

    statusBar()->showMessage("就绪");
    qInfo() << "UI initialized";

    // 欢迎画面
    auto* welcomeScene = new QGraphicsScene(this);
    welcomeScene->setSceneRect(0, 0, 600, 500);
    auto* title = welcomeScene->addText("CryptoFlow", QFont("PingFang SC", 36, QFont::Bold));
    title->setDefaultTextColor(QColor(0, 200, 255));
    title->setPos(150, 150);
    auto* subtitle = welcomeScene->addText("交互式密码学原理可视化", QFont("PingFang SC", 16));
    subtitle->setDefaultTextColor(QColor(150, 150, 150));
    subtitle->setPos(170, 220);
    auto* hint = welcomeScene->addText("→ 选择算法，点击开始演示", QFont("PingFang SC", 13));
    hint->setDefaultTextColor(QColor(100, 100, 100));
    hint->setPos(180, 280);
    view_->setScene(welcomeScene);
}

void MainWindow::onCaesarStart(const QString& text, int shift) {
    view_->setScene(caesarScene_);
    caesarScene_->startAnimation(text, shift);
    statusBar()->showMessage(QString("凯撒密码: 移位 %1").arg(shift));
}

void MainWindow::onVigenereStart(const QString& text, const QString& keyword) {
    view_->setScene(vigenereScene_);
    vigenereScene_->startAnimation(text, keyword);
    statusBar()->showMessage(QString("维吉尼亚密码: 密钥 %1").arg(keyword));
}

void MainWindow::onRSAKeyGen(int64_t p, int64_t q) {
    view_->setScene(rsaScene_);
    rsaScene_->startKeyGen(p, q);
    auto keys = RSAEngine::generateKeys(p, q);
    controlPanel_->setRSAValues(keys.e, keys.n, keys.d);
    statusBar()->showMessage(QString("RSA 密钥生成: p=%1, q=%2").arg(p).arg(q));
}

void MainWindow::onRSAEncrypt(int64_t message, int64_t e, int64_t n) {
    view_->setScene(rsaScene_);
    rsaScene_->startEncrypt(message, e, n);
    int64_t cipher = RSAEngine::encrypt(message, e, n);
    controlPanel_->setLastCipher(cipher);
    statusBar()->showMessage(QString("RSA 加密: M=%1 → C=%2").arg(message).arg(cipher));
}

void MainWindow::onRSADecrypt(int64_t cipher, int64_t d, int64_t n) {
    view_->setScene(rsaScene_);
    rsaScene_->startDecrypt(cipher, d, n);
    statusBar()->showMessage(QString("RSA 解密: C=%1").arg(cipher));
}

void MainWindow::onBase64Start(const QString& text) {
    view_->setScene(base64Scene_);
    base64Scene_->startAnimation(text);
    statusBar()->showMessage("Base64 编码");
}

void MainWindow::onXORStart(const QString& text, const QString& key) {
    view_->setScene(xorScene_);
    xorScene_->startAnimation(text, key);
    statusBar()->showMessage(QString("XOR 加密: 密钥 %1").arg(key));
}

void MainWindow::onReset() {
    caesarScene_->reset();
    rsaScene_->reset();
    vigenereScene_->reset();
    base64Scene_->reset();
    xorScene_->reset();
    statusBar()->showMessage("已重置");
    qInfo() << "Scenes reset";
}

void MainWindow::onSpeedChanged(int ms) {
    caesarScene_->setSpeed(ms);
    rsaScene_->setSpeed(ms);
    vigenereScene_->setSpeed(ms);
    base64Scene_->setSpeed(ms);
    xorScene_->setSpeed(ms);
}

void MainWindow::saveScreenshot(const QString& prefix) {
    QImage image(view_->viewport()->size(), QImage::Format_ARGB32);
    view_->viewport()->render(&image);
    QString filename = QString("%1_screenshot.png").arg(prefix);
    image.save(filename);
    qInfo() << "Screenshot saved:" << filename;
}

void MainWindow::autoStartCaesar(const QString& text, int shift) {
    controlPanel_->setAlgorithm(0);
    onCaesarStart(text, shift);
}

void MainWindow::autoStartRSA() {
    controlPanel_->setAlgorithm(1);
    onRSAKeyGen(61, 53);
}

void MainWindow::autoStartBase64(const QString& text) {
    controlPanel_->setAlgorithm(3);
    onBase64Start(text);
}

void MainWindow::autoStartXOR(const QString& text, const QString& key) {
    controlPanel_->setAlgorithm(4);
    onXORStart(text, key);
}
