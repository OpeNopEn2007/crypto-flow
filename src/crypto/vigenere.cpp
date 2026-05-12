#include "vigenere.h"
#include <QRegularExpression>

QString VigenereCipher::alphabet() {
    return "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
}

VigenereResult VigenereCipher::encrypt(const QString& text, const QString& key) {
    VigenereResult result;
    QString upperKey = key.toUpper().remove(QRegularExpression("[^A-Z]"));
    if (upperKey.isEmpty()) return result;

    QString alpha = alphabet();
    int keyIdx = 0;

    for (int i = 0; i < text.size(); i++) {
        QChar ch = text[i];
        if (ch.isLetter()) {
            bool isLower = ch.isLower();
            int pIdx = alpha.indexOf(ch.toUpper());
            int kIdx = alpha.indexOf(upperKey[keyIdx % upperKey.size()]);
            int cIdx = (pIdx + kIdx) % 26;
            result.shifts.append(kIdx);
            result.result.append(isLower ? alpha[cIdx].toLower() : alpha[cIdx]);
            keyIdx++;
        } else {
            result.shifts.append(0);
            result.result.append(ch);
        }
    }
    return result;
}

VigenereResult VigenereCipher::decrypt(const QString& text, const QString& key) {
    VigenereResult result;
    QString upperKey = key.toUpper().remove(QRegularExpression("[^A-Z]"));
    if (upperKey.isEmpty()) return result;

    QString alpha = alphabet();
    int keyIdx = 0;

    for (int i = 0; i < text.size(); i++) {
        QChar ch = text[i];
        if (ch.isLetter()) {
            bool isLower = ch.isLower();
            int cIdx = alpha.indexOf(ch.toUpper());
            int kIdx = alpha.indexOf(upperKey[keyIdx % upperKey.size()]);
            int pIdx = (cIdx - kIdx + 26) % 26;
            result.shifts.append(kIdx);
            result.result.append(isLower ? alpha[pIdx].toLower() : alpha[pIdx]);
            keyIdx++;
        } else {
            result.shifts.append(0);
            result.result.append(ch);
        }
    }
    return result;
}
