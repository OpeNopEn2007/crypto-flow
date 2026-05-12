#include "mainwindow.h"
#include "controlpanel.h"
#include "scenes/caesarscene.h"
#include "scenes/rsascene.h"
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

    // Create scenes
    caesarScene_ = new CaesarScene(this);
    rsaScene_ = new RSAScene(this);

    // Graphics view
    view_ = new QGraphicsView;
    view_->setRenderHint(QPainter::Antialiasing);
    view_->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view_->setStyleSheet("background: #1a1a2e; border: none;");
    view_->setScene(caesarScene_);

    // Control panel
    controlPanel_ = new ControlPanel;

    // Splitter: visualization left, controls right
    auto* splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(view_);
    splitter->addWidget(controlPanel_);
    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 1);
    splitter->setHandleWidth(2);

    setCentralWidget(splitter);

    // Connections
    connect(controlPanel_, &ControlPanel::caesarStart, this, &MainWindow::onCaesarStart);
    connect(controlPanel_, &ControlPanel::rsaKeyGen, this, &MainWindow::onRSAKeyGen);
    connect(controlPanel_, &ControlPanel::rsaEncrypt, this, &MainWindow::onRSAEncrypt);
    connect(controlPanel_, &ControlPanel::rsaDecrypt, this, &MainWindow::onRSADecrypt);
    connect(controlPanel_, &ControlPanel::resetRequested, this, &MainWindow::onReset);
    connect(controlPanel_, &ControlPanel::speedChanged, this, &MainWindow::onSpeedChanged);

    // 动画完成后自动截图
    connect(caesarScene_, &CaesarScene::animationComplete,
            this, [this]() { saveScreenshot("caesar"); });
    connect(rsaScene_, &RSAScene::animationComplete,
            this, [this]() { saveScreenshot("rsa"); });

    statusBar()->showMessage("就绪");
    qInfo() << "UI initialized";
}

void MainWindow::switchToCaesar() {
    view_->setScene(caesarScene_);
}

void MainWindow::switchToRSA() {
    view_->setScene(rsaScene_);
}

void MainWindow::onCaesarStart(const QString& text, int shift) {
    switchToCaesar();
    caesarScene_->startAnimation(text, shift);
    statusBar()->showMessage(QString("凯撒密码: 移位 %1").arg(shift));
    qInfo() << "Caesar animation started, text:" << text << "shift:" << shift;
}

void MainWindow::onRSAKeyGen(int64_t p, int64_t q) {
    switchToRSA();
    rsaScene_->startKeyGen(p, q);
    statusBar()->showMessage(QString("RSA 密钥生成: p=%1, q=%2").arg(p).arg(q));
    qInfo() << "RSA keygen started, p:" << p << "q:" << q;
}

void MainWindow::onRSAEncrypt(int64_t message, int64_t e, int64_t n) {
    switchToRSA();
    rsaScene_->startEncrypt(message, e, n);
    statusBar()->showMessage(QString("RSA 加密: M=%1").arg(message));
    qInfo() << "RSA encrypt started, message:" << message;
}

void MainWindow::onRSADecrypt(int64_t cipher, int64_t d, int64_t n) {
    switchToRSA();
    rsaScene_->startDecrypt(cipher, d, n);
    statusBar()->showMessage(QString("RSA 解密: C=%1").arg(cipher));
    qInfo() << "RSA decrypt started, cipher:" << cipher;
}

void MainWindow::onReset() {
    caesarScene_->reset();
    rsaScene_->reset();
    statusBar()->showMessage("已重置");
    qInfo() << "Scenes reset";
}

void MainWindow::onSpeedChanged(int ms) {
    caesarScene_->setSpeed(ms);
    rsaScene_->setSpeed(ms);
    qInfo() << "Animation speed changed to" << ms << "ms";
}

void MainWindow::saveScreenshot(const QString& prefix) {
    QImage image(view_->viewport()->size(), QImage::Format_ARGB32);
    view_->viewport()->render(&image);
    QString filename = QString("%1_screenshot.png").arg(prefix);
    image.save(filename);
    qInfo() << "Screenshot saved:" << filename;
}

void MainWindow::autoStartCaesar(const QString& text, int shift) {
    onCaesarStart(text, shift);
}

void MainWindow::autoStartRSA() {
    onRSAKeyGen(61, 53);
}
