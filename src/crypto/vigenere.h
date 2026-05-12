#pragma once
#include <QString>
#include <QVector>

struct VigenereResult {
    QString result;
    QVector<int> shifts;
};

class VigenereCipher {
public:
    static VigenereResult encrypt(const QString& text, const QString& key);
    static VigenereResult decrypt(const QString& text, const QString& key);
    static QString alphabet();
};
