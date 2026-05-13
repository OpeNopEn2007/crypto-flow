#include <QApplication>
#include <QCommandLineParser>
#include <QTimer>
#include "mainwindow.h"
#include <QFile>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("CryptoFlow");
    app.setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("交互式密码学可视化演示系统");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption caesarOpt("caesar", "自动启动凯撒密码演示", "text", "HELLO");
    QCommandLineOption shiftOpt("shift", "凯撒移位量", "n", "3");
    QCommandLineOption rsaOpt("rsa", "自动启动RSA密钥生成演示");
    QCommandLineOption base64Opt("base64", "自动启动Base64编码演示", "text", "Hello");
    QCommandLineOption xorOpt("xor", "自动启动XOR加密演示", "text", "HELLO");
    QCommandLineOption xorKeyOpt("xor-key", "XOR密钥", "key", "KEY");
    parser.addOption(caesarOpt);
    parser.addOption(shiftOpt);
    parser.addOption(rsaOpt);
    parser.addOption(base64Opt);
    parser.addOption(xorOpt);
    parser.addOption(xorKeyOpt);
    parser.process(app);

    // Load stylesheet
    app.setStyleSheet(R"(
        QMainWindow { background-color: #0d1117; }
        QWidget { background-color: #161b22; color: #c9d1d9; font-family: "PingFang SC", "Helvetica Neue", sans-serif; }
        QSplitter::handle { background-color: #21262d; width: 2px; }
        QGroupBox { border: 1px solid #30363d; border-radius: 6px; margin-top: 8px; padding-top: 16px; font-size: 12px; color: #8b949e; }
        QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 4px; }
        QComboBox { background-color: #21262d; border: 1px solid #30363d; border-radius: 4px; padding: 6px 10px; color: #c9d1d9; font-size: 13px; }
        QComboBox::drop-down { border: none; width: 20px; }
        QComboBox QAbstractItemView { background-color: #21262d; border: 1px solid #30363d; selection-background-color: #1f6feb; color: #c9d1d9; }
        QLineEdit, QSpinBox { background-color: #0d1117; border: 1px solid #30363d; border-radius: 4px; padding: 6px 10px; color: #58a6ff; font-family: "Menlo", monospace; font-size: 13px; }
        QLineEdit:focus, QSpinBox:focus { border-color: #1f6feb; }
        QPushButton { background-color: #21262d; border: 1px solid #30363d; border-radius: 6px; padding: 8px 16px; color: #c9d1d9; font-size: 13px; }
        QPushButton:hover { background-color: #30363d; border-color: #8b949e; }
        QPushButton#startBtn { background-color: #238636; border-color: #2ea043; color: #ffffff; }
        QPushButton#startBtn:hover { background-color: #2ea043; }
        QPushButton#resetBtn { background-color: #da3633; border-color: #f85149; color: #ffffff; }
        QPushButton#resetBtn:hover { background-color: #f85149; }
        QLabel#titleLabel { color: #58a6ff; font-size: 20px; padding: 8px 0; }
        QLabel { color: #8b949e; font-size: 11px; }
        QSlider::groove:horizontal { background: #21262d; height: 6px; border-radius: 3px; }
        QSlider::handle:horizontal { background: #58a6ff; width: 16px; height: 16px; margin: -5px 0; border-radius: 8px; }
        QSlider::handle:horizontal:hover { background: #79c0ff; }
        QStatusBar { background-color: #0d1117; color: #8b949e; font-size: 11px; border-top: 1px solid #21262d; }
        QGraphicsView { background-color: #1a1a2e; border: none; }
    )");

    MainWindow window;
    window.show();

    if (parser.isSet(caesarOpt)) {
        QTimer::singleShot(500, [&window, &parser, &caesarOpt, &shiftOpt]() {
            int shift = parser.value(shiftOpt).toInt();
            if (shift < 1 || shift > 25) shift = 3;
            window.autoStartCaesar(parser.value(caesarOpt), shift);
        });
    } else if (parser.isSet(rsaOpt)) {
        QTimer::singleShot(500, [&window]() {
            window.autoStartRSA();
        });
    } else if (parser.isSet(base64Opt)) {
        QTimer::singleShot(500, [&window, &parser, &base64Opt]() {
            window.autoStartBase64(parser.value(base64Opt));
        });
    } else if (parser.isSet(xorOpt)) {
        QTimer::singleShot(500, [&window, &parser, &xorOpt, &xorKeyOpt]() {
            window.autoStartXOR(parser.value(xorOpt), parser.value(xorKeyOpt));
        });
    }

    return app.exec();
}
