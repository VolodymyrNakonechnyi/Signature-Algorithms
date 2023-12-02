#include <iostream>
#include <cmath>
#include <random>

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int mod_inverse(int a, int m) {
    int m0 = m, t, q;
    int x0 = 0, x1 = 1;

    if (m == 1)
        return 0;

    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0)
        x1 += m0;

    return x1;
}

int powmod(int base, int exponent, int modulus) {
    int result = 1;

    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = (result * static_cast<long long>(base)) % modulus;
        }
        exponent = exponent >> 1;
        base = (static_cast<long long>(base) * base) % modulus;
    }

    return result;
}

std::pair<std::pair<int, int>, std::pair<int, int>> key_gen() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> distribution(50, 99);

    int P = distribution(gen);
    int Q = distribution(gen);

    int n = P * Q;
    int phi_n = (P - 1) * (Q - 1);

    int e = 2;
    while (e < phi_n && gcd(e, phi_n) != 1) {
        ++e;
    }

    int d = mod_inverse(e, phi_n);

    return std::make_pair(std::make_pair(d, n), std::make_pair(e, n));
}

int encrypt(int message, std::pair<int, int> public_key) {
    int e = public_key.first;
    int n = public_key.second;

    return powmod(message, e, n);
}

int decrypt(int ciphertext, std::pair<int, int> private_key) {
    int d = private_key.first;
    int n = private_key.second;

    return powmod(ciphertext, d, n);
}

int main() {
    std::pair<std::pair<int, int>, std::pair<int, int>> keys = key_gen();
    std::pair<int, int> private_key = keys.first;
    std::pair<int, int> public_key = keys.second;

    int message = 42;

    int ciphertext = encrypt(message, public_key);
    int decrypted_message = decrypt(ciphertext, private_key);

    std::cout << "Original message: " << message << std::endl;
    std::cout << "Ciphertext: " << ciphertext << std::endl;
    std::cout << "Decrypted message: " << decrypted_message << std::endl;

    return 0;
}
