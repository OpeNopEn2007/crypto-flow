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
    parser.addOption(caesarOpt);
    parser.addOption(shiftOpt);
    parser.addOption(rsaOpt);
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
            window.autoStartCaesar(parser.value(caesarOpt), parser.value(shiftOpt).toInt());
        });
    } else if (parser.isSet(rsaOpt)) {
        QTimer::singleShot(500, [&window]() {
            window.autoStartRSA();
        });
    }

    return app.exec();
}
