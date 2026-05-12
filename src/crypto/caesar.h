#pragma once
#include <QString>
#include <QVector>

struct CaesarResult {
    QString result;
    QVector<int> shifts;  // 每个字符的移位量
};

class CaesarCipher {
public:
    static CaesarResult encrypt(const QString& text, int shift);
    static CaesarResult decrypt(const QString& text, int shift);
    static QString alphabet();
};
