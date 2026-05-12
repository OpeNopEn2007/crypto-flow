#include "rsa.h"
#include <QtMath>

int64_t RSAEngine::gcd(int64_t a, int64_t b) {
    while (b != 0) {
        int64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

bool RSAEngine::isPrime(int64_t n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

int64_t RSAEngine::modPow(int64_t base, int64_t exp, int64_t mod) {
    int64_t result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        exp >>= 1;
        base = (base * base) % mod;
    }
    return result;
}

int64_t RSAEngine::modInverse(int64_t a, int64_t m) {
    int64_t m0 = m, t, q;
    int64_t x0 = 0, x1 = 1;
    if (m == 1) return 0;
    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) x1 += m0;
    return x1;
}

RSAKeys RSAEngine::generateKeys(int64_t p, int64_t q) {
    RSAKeys keys;
    keys.p = p;
    keys.q = q;
    keys.n = p * q;
    keys.phi = (p - 1) * (q - 1);
    keys.e = 65537;
    while (gcd(keys.e, keys.phi) != 1) keys.e += 2;
    keys.d = modInverse(keys.e, keys.phi);
    return keys;
}

int64_t RSAEngine::encrypt(int64_t message, int64_t e, int64_t n) {
    return modPow(message, e, n);
}

int64_t RSAEngine::decrypt(int64_t cipher, int64_t d, int64_t n) {
    return modPow(cipher, d, n);
}

QVector<RSAStep> RSAEngine::keyGenSteps(int64_t p, int64_t q) {
    QVector<RSAStep> steps;
    steps.append({"选择两个素数 p 和 q", "p, q", 0});
    steps.append({"计算 n = p × q", QString("n = %1 × %2").arg(p).arg(q), p * q});

    int64_t phi = (p - 1) * (q - 1);
    steps.append({"计算欧拉函数 φ(n) = (p-1)(q-1)",
                   QString("φ(n) = %1 × %2").arg(p - 1).arg(q - 1), phi});

    int64_t e = 65537;
    while (gcd(e, phi) != 1) e += 2;
    steps.append({"选择公钥指数 e，满足 gcd(e, φ(n)) = 1",
                   QString("e = %1").arg(e), e});

    int64_t d = modInverse(e, phi);
    steps.append({"计算私钥指数 d ≡ e⁻¹ (mod φ(n))",
                   QString("d = %1").arg(d), d});

    steps.append({"公钥 (e, n)", QString("(%1, %2)").arg(e).arg(p * q), 0});
    steps.append({"私钥 (d, n)", QString("(%1, %2)").arg(d).arg(p * q), 0});
    return steps;
}
