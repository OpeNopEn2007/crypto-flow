#include <QApplication>
#include "mainwindow.h"
#include <QFile>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("CryptoFlow");
    app.setApplicationVersion("1.0");

    // Load stylesheet
    QFile styleFile(":/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        app.setStyleSheet(styleFile.readAll());
        styleFile.close();
    }

    MainWindow window;
    window.show();

    return app.exec();
}
