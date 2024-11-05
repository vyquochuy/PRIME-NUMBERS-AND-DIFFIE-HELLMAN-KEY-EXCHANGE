#include <iostream>
#include <random>
#include <cmath>

// A: Triển khai hàm lũy thừa mô-đun
// hàm cần thực hiện: (base^exponent) % mod
long long modular_exponentiation(long long base, long long exponent, long long mod) {
    long long result = 1;
    base = base % mod;

    // Ví dụ về logic lũy thừa mô đun
    while (exponent > 0) {
        if (exponent % 2 == 1) {
            // cập nhật kết quả với phép nhân mô-đun
            result = (result * base) % mod;
        }
        // cập nhật base và exponent cho lần lặp tiếp theo
        base = (base * base) % mod;
        exponent = exponent / 2;
    }
    return result;
}

// kiểm tra số nguyên tố
bool is_prime(long long n, int k = 5) {
    if (n <= 1 || n == 4) return false;
    if (n <= 3) return true;

    while (k > 0) {
        long long a = 2 + rand() % (n - 4);
        if (modular_exponentiation(a, n - 1, n) != 1) return false;
        k--;
    }
    return true;
}

// B: Triển khai hàm sinh số nguyên tố ngẫu nhiên
long long generate_safe_prime(int bit_size) {
    // 1. Tự cài đặt logic để sinh 1 số nguyên tố an toàn
    // 2. Tự viết hàm kiểm tra nguyên tố (ví dụ: Miller-Rabin)
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<long long> dis(1LL << (bit_size - 1), (1LL << bit_size) - 1);

    long long prime = 0;
    while (true) {
        prime = dis(gen);
        if (is_prime(prime) && is_prime((prime - 1) / 2)) {
            break;
        }
    }
    return prime;
}

// C: Triển khai hàm sinh khóa riêng ngẫu nhiên
long long generate_private_key(long long p) {
    // sử dụng hàm sinh số nguyên tố ngẫu nhiên để sinh khóa riêng
    // Khóa riêng nên nằm trong khoảng [2, p-2]
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<long long> dis(2, p - 2);
    long long private_key = dis(gen);
    return private_key;
}

// D: Hoàn thành logic trao đổi khóa Diffie-Hellman
int main() {
    std::cout << "Diffie-Hellman Key Exchange\n";
    // 1. Sinh số nguyên tố lớn p và phần tử sinh g
    int bit_size = 32; // Kích thước bit ví dụ, có thể điều chỉnh

    time_t start, end;
    start = time(NULL);
    long long p = generate_safe_prime(bit_size); // Sinh một số nguyên tố
    long long g = 2; // phần tử sinh, sinh viên tự cần tìm hiểu và chọn giá trị khác
    end = time(NULL);
    std::cout << "So Nguyen To P: " << p << "\n";
    std::cout << "thoi gian sinh so nguyen to: " << (end - start)*1000 << "s\n\n";

    // 2. Sinh khóa riêng của Alice và Bob
    start = time(NULL);
    long long a = generate_private_key(p); // Khóa riêng của Alice
    long long b = generate_private_key(p); // Khóa riêng của Bob
    end = time(NULL);
    std::cout << "Khoa Rieng Cua Alice: " << a << "\n";
    std::cout << "Khoa Rieng Cua Bob: " << b << "\n";
    std::cout << "thoi gian sinh khoa rieng: " << (end - start)*1000 << "s\n\n";

    // 3. Tính giá trị công khai của Alice và Bob
    start = time(NULL);
    long long A = modular_exponentiation(g, a, p); // Alice tính A = g^a % p
    long long B = modular_exponentiation(g, b, p); // Bob tính B = g^b % p
    end = time(NULL);
    std::cout << "Gia Tri Cong Khai Cua Alice: " << A << "\n";
    std::cout << "Gia Tri Cong Khai Cua Bob: " << B << "\n";
    std::cout << "thoi gian tinh gia tri cong khai: " << (end - start)*1000 << "s\n\n";


    // 4. Tính bí mật chung
    start = time(NULL);
    long long alice_shared_secret = modular_exponentiation(B, a, p); // Alice tính s = B^a % p
    long long bob_shared_secret = modular_exponentiation(A, b, p); // Bob tính = A^b % p
    end = time(NULL);
    std::cout << "thoi gian tinh bi mat chung: " << (end - start)*1000 << "s\n\n";

    // 5. Hiển thị kết quả và xác minh chung trùng khớp
    std::cout << "Bi Mat Chung Alice Nhan Duoc: " << alice_shared_secret << "\n";
    std::cout << "Bi Mat Chung Bob Nhan Duoc: " << bob_shared_secret << "\n";
    std::cout << "Qua Trinh Tinh Toan Co Dung Khong ? " << (alice_shared_secret == bob_shared_secret ? "Dung " : "Sai" ) << "\n";

    return 0;
}