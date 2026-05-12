#pragma once
#include <cstdint>
#include <QVector>

struct RSAKeys {
    int64_t p, q;
    int64_t n;
    int64_t phi;
    int64_t e, d;
};

struct RSAStep {
    QString description;
    QString formula;
    int64_t value;
};

class RSAEngine {
public:
    static RSAKeys generateKeys(int64_t p, int64_t q);
    static int64_t encrypt(int64_t message, int64_t e, int64_t n);
    static int64_t decrypt(int64_t cipher, int64_t d, int64_t n);
    static QVector<RSAStep> keyGenSteps(int64_t p, int64_t q);

private:
    static int64_t modPow(int64_t base, int64_t exp, int64_t mod);
    static int64_t modInverse(int64_t a, int64_t m);
    static int64_t gcd(int64_t a, int64_t b);
    static bool isPrime(int64_t n);
};
