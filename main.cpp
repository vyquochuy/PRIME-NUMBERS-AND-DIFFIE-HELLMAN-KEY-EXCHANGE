#include <iostream>
#include <random>
#include <cmath>
#include <ctime>
#include <vector>
#include "BigInt.h"

// 200 số nguyên tố đầu tiên
std::vector<int> first_prime = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 
                                73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 
                                151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 
                                229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 
                                311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 
                                397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 
                                479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571,
                                577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653,
                                659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751,
                                757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839, 853,
                                857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947,   
                                953, 967, 971, 977, 983, 991, 997, 1009, 1013, 1019, 1021, 1031, 1033,
                                1039, 1049, 1051, 1061, 1063, 1069, 1087, 1091, 1093, 1097, 1103, 1109,};
                                
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
    std::uniform_int_distribution<int> dis(2, 100);

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
        BigInt prime2 = (prime - 1) / 2;
        // kiểm tra số nguyên tố có kết thúc bằng 1, 3, 7, 9
        if (prime.lastDigit() != 1  && prime.lastDigit() != 3 && prime.lastDigit() != 7 && prime.lastDigit() != 9) continue;
        if (prime2.lastDigit() != 1  && prime2.lastDigit() != 3 && prime2.lastDigit() != 7 && prime2.lastDigit() != 9) continue;

        // kiểm tra số nguyên tố có dạng 3k+1
        if (prime % 3 == 1 || prime2 % 3 == 1) continue;

        // chia cho 100 số nguyên tố đầu tiên
        bool is_composite = false;
        for (int p : first_prime) {
            if ((prime % p == 0 && prime != p) || (prime2 % p == 0 && prime2 != p)) {
                is_composite = true;
                break;
            }
        }
        if (is_composite) continue;

        std::cout << "Generated candidate prime: " << prime << std::endl;
        if (miller_rabin(prime) && miller_rabin(prime2)) {
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
    int bit_size = 128; // Kích thước bit ví dụ, có thể điều chỉnh

    time_t start, end;
    start = time(NULL);
    BigInt p = generate_safe_prime(bit_size); // Sinh một số nguyên tố
    BigInt g = 5; // phần tử sinh, sinh viên tự cần tìm hiểu và chọn giá trị khác
    end = time(NULL);
    std::cout << "So Nguyen To P: " << p << "\n";
    std::cout << "thoi gian sinh so nguyen to: " << (end - start)*1000 << "ms\n\n";

    // 2. Sinh khóa riêng của Alice và Bob
    start = time(NULL);
    BigInt a = generate_private_key(p); // Khóa riêng của Alice
    BigInt b = generate_private_key(p); // Khóa riêng của Bob
    end = time(NULL);
    std::cout << "Khoa Rieng Cua Alice: " << a << "\n";
    std::cout << "Khoa Rieng Cua Bob: " << b << "\n";
    std::cout << "thoi gian sinh khoa rieng: " << (end - start)*1000 << "ms\n\n";

    // 3. Tính giá trị công khai của Alice và Bob
    start = time(NULL);
    BigInt A = modular_exponentiation(g, a, p); // Alice tính A = g^a % p
    BigInt B = modular_exponentiation(g, b, p); // Bob tính B = g^b % p
    end = time(NULL);
    std::cout << "Gia Tri Cong Khai Cua Alice: " << A << "\n";
    std::cout << "Gia Tri Cong Khai Cua Bob: " << B << "\n";
    std::cout << "thoi gian tinh gia tri cong khai: " << (end - start)*1000 << "ms\n\n";

    // 4. Tính bí mật chung
    start = time(NULL);
    BigInt alice_shared_secret = modular_exponentiation(B, a, p); // Alice tính s = B^a % p
    BigInt bob_shared_secret = modular_exponentiation(A, b, p); // Bob tính = A^b % p
    end = time(NULL);
    std::cout << "thoi gian tinh bi mat chung: " << (end - start)*1000 << "ms\n\n";

    // 5. Hiển thị kết quả và xác minh chung trùng khớp
    std::cout << "Bi Mat Chung Alice Nhan Duoc: " << alice_shared_secret << "\n";
    std::cout << "Bi Mat Chung Bob Nhan Duoc: " << bob_shared_secret << "\n";
    std::cout << "Qua Trinh Tinh Toan Co Dung Khong ? " << (alice_shared_secret == bob_shared_secret ? "Dung " : "Sai" ) << "\n";

    return 0;
}