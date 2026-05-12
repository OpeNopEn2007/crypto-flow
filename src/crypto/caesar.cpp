#include "caesar.h"

CaesarResult CaesarCipher::encrypt(const QString& text, int shift) {
    CaesarResult res;
    shift = ((shift % 26) + 26) % 26;
    for (QChar ch : text) {
        if (ch.isLetter()) {
            QChar base = ch.isUpper() ? 'A' : 'a';
            QChar encrypted = QChar(base.unicode() + (ch.unicode() - base.unicode() + shift) % 26);
            res.result += encrypted;
            res.shifts.append(shift);
        } else {
            res.result += ch;
            res.shifts.append(0);
        }
    }
    return res;
}

CaesarResult CaesarCipher::decrypt(const QString& text, int shift) {
    return encrypt(text, 26 - (shift % 26));
}

QString CaesarCipher::alphabet() {
    return "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
}
