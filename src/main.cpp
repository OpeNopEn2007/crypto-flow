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
    QFile styleFile(":/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        app.setStyleSheet(styleFile.readAll());
        styleFile.close();
    }

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
