#include <iostream>
#include <random>
#include <cmath>
#include <ctime>
#include <vector>
#include "BigInt.h"

// A: Triển khai hàm lũy thừa mô-đun (Exponentiation by Squaring)
BigInt modular_exponentiation(BigInt base, BigInt exponent, BigInt mod) {
    BigInt result = 1;
    base = base % mod; // Giảm base modulo mod để tránh tràn số

    while (exponent > 0) {
        // Nếu exponent là số lẻ, nhân với base
        if (exponent % 2 == 1) {
            result = (result * base) % mod;
        }
        // Lũy thừa nhanh, tăng số mũ và giảm exponent
        base = (base * base) % mod;
        exponent = exponent / 2;
    }

    return result;
}

// B: Kiểm tra số nguyên tố bằng thuật toán Miller-Rabin
bool miller_rabin(BigInt n, int k = 5) {
    if (n <= 1 || n == 4) return false;
    if (n == 2 || n == 3) return true;

    BigInt d = n - 1;
    int r = 0;
    while (d % 2 == 0) {
        d = d / 2;
        r++;
    }

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<int> dis(2, 9);

    for (int i = 0; i < k; i++) {
        BigInt a = dis(gen); // Chọn a ngẫu nhiên
        BigInt x = modular_exponentiation(a, d, n);
        if (x == 1 || x == n - 1) continue;
        for (int j = 0; j < r - 1; j++) {
            x = (x * x) % n;
            if (x == n - 1) break;
        }
        if (x != n - 1) return false;
    }
    return true;
}

// C: Hàm sinh số nguyên tố an toàn
BigInt generate_safe_prime(int bit_size) {
    BigInt min = BigInt(1) << (bit_size - 1);
    BigInt max = (BigInt(1) << bit_size) - 1;

    BigInt prime = 0;

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<int> dis(0, 9);

    while (true) {
        prime = 0;
        for (int i = 0; i < bit_size / 3.32; ++i) {
            prime = prime * 10 + dis(gen);
        }
        if (prime % 2 == 0)
            continue;
        std::cout << "Generated candidate prime: " << prime << std::endl;
        if (miller_rabin(prime) && miller_rabin((prime - 1) / 2)) {
            std::cout << "Prime passed Miller-Rabin test: " << prime << std::endl;
            break;
        }
    }

    return prime;
}

// D: Hàm sinh khóa riêng ngẫu nhiên
BigInt generate_private_key(BigInt p) {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<int> dis(0, 9);

    BigInt key = 0;
    for (int i = 0; i < p.NumDigits() - 1; ++i) {
        key = key * 10 + dis(gen);
    }
    return key % (p - 2) + 2; // Khóa trong khoảng [2, p-2]
}

// E: Chương trình chính cho Diffie-Hellman Key Exchange
int main() {
    std::cout << "Diffie-Hellman Key Exchange\n";
    // 1. Sinh số nguyên tố lớn p và phần tử sinh g
    int bit_size = 32; // Kích thước bit ví dụ, có thể điều chỉnh

    time_t start, end;
    start = time(NULL);
    BigInt p = generate_safe_prime(bit_size); // Sinh một số nguyên tố
    BigInt g = 5; // phần tử sinh, sinh viên tự cần tìm hiểu và chọn giá trị khác
    end = time(NULL);
    std::cout << "So Nguyen To P: " << p << "\n";
    std::cout << "thoi gian sinh so nguyen to: " << (end - start)*1000 << "s\n\n";

    // 2. Sinh khóa riêng của Alice và Bob
    start = time(NULL);
    BigInt a = generate_private_key(p); // Khóa riêng của Alice
    BigInt b = generate_private_key(p); // Khóa riêng của Bob
    end = time(NULL);
    std::cout << "Khoa Rieng Cua Alice: " << a << "\n";
    std::cout << "Khoa Rieng Cua Bob: " << b << "\n";
    std::cout << "thoi gian sinh khoa rieng: " << (end - start)*1000 << "s\n\n";

    // 3. Tính giá trị công khai của Alice và Bob
    start = time(NULL);
    BigInt A = modular_exponentiation(g, a, p); // Alice tính A = g^a % p
    BigInt B = modular_exponentiation(g, b, p); // Bob tính B = g^b % p
    end = time(NULL);
    std::cout << "Gia Tri Cong Khai Cua Alice: " << A << "\n";
    std::cout << "Gia Tri Cong Khai Cua Bob: " << B << "\n";
    std::cout << "thoi gian tinh gia tri cong khai: " << (end - start)*1000 << "s\n\n";


    // 4. Tính bí mật chung
    start = time(NULL);
    BigInt alice_shared_secret = modular_exponentiation(B, a, p); // Alice tính s = B^a % p
    BigInt bob_shared_secret = modular_exponentiation(A, b, p); // Bob tính = A^b % p
    end = time(NULL);
    std::cout << "thoi gian tinh bi mat chung: " << (end - start)*1000 << "s\n\n";

    // 5. Hiển thị kết quả và xác minh chung trùng khớp
    std::cout << "Bi Mat Chung Alice Nhan Duoc: " << alice_shared_secret << "\n";
    std::cout << "Bi Mat Chung Bob Nhan Duoc: " << bob_shared_secret << "\n";
    std::cout << "Qua Trinh Tinh Toan Co Dung Khong ? " << (alice_shared_secret == bob_shared_secret ? "Dung " : "Sai" ) << "\n";

    return 0;
}