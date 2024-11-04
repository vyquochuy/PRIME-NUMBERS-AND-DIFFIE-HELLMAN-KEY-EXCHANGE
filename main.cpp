#include <iostream>

// A: Triên khai hàm lũy thừa mô-đun
// hàm cần thực hiện: (base^exponent) % mod
<...> modular_exponentiation(<...> base, <...> exponent, <...> mod) {
    <...> result = 1;
    base = base% mod;

    // Ví dụ về logic lũy thừa mô đun
    while (exponent > 0){
        if(exponent % 2 == 1){
        // cập nhật kết quả với phép nhân mô-đun
        }
        // cập nhật base và exponent cho lần lặp tiếp theo
    }
    return result;
}

// B: Triên khai hàm sinh số nguyên tố ngẫu nhiên
<...> generate_safe_prime(int bit_size){
    // 1. Tự cài đặt logic để sinh 1 số nguyên tố an toàn
    // 2. Tự viết hàm kiểm tra nguyên tố (ví dụ: Miller-Rabin)

    <...> prime= 0;
    return prime;
}

// C: Triên khai hàm sinh khóa riêng ngẫu nhiên
<...> generate_private_key(<...> p){
    // sử dụng hàm sinh số nguyên tố ngẫu nhiên để sinh khóa riêng
    // Khóa riêng nên nằm trong khoảng [2, p-2]
    <...> private_key = 0;
    return private_key;
}

// D: Hoàn thành logic trao đôi khóa Diffie-Hellman
int main(){

    // 1. Sinh số nguyên tố lớn p và phần tử sinh g
    int bit_size = 512; // Kích thước bit ví dụ, có thể điều chỉnh
    <...> p = generate_safe_prime(bit_size); // Sinh một số nguyên tố
    <...> g = 2; // phần tử sinh, sinh viên tự cần tìm hiểu và chọn giá trị khác

    // 2. Sinh khóa riêng của Alice và Bob
    <...> a = generate_private_key(p); // Khóa riêng của Alice
    <...> b = generate_private_key(p); // Khóa riêng của Bob

    // 3. Tính giá trị công khai của Alice và Bob
    <...> A = modular_exponentiation(g,a,p); // Alice tính A = g^a % p
    <...> B = modular_exponentiation(g,b,p); // Bob tính B = g^b % p

    // 4. Tính bí mật chung
    <...> alice_shared_secret=modular_exponentiation(B,a, p); // Alice tính s = B^a % p
    <...> bob_shared_secret =modular_exponentiation(A,b, p); // Bob tính = A^b % p

    // 5. Hiên thị kết quả và xác minh chung trùng khớp
    std::cout << "Bí Mật Chung Alice Nhận Được: "<<alice_shared_secret<<"\n";
    std::cout << "Bí Mật Chung Bob Nhận Được: "<< bob_shared_secret<<"\n";
    std::cout << "Quá trình tính toán đúng không? " <<(alice_shared_secret==bob_shared_secret)<< "\n";

    return 0;
}