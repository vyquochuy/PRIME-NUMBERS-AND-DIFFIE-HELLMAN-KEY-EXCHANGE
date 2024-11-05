#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;
class BigInt {
public:
    // Constructors
    BigInt();
    BigInt(const string &);
    BigInt(int);

    // Member functions
    bool IsNegative() const;
    bool IsPositive() const;
    int NumDigits() const;
    int GetDigit(int k) const;
    void AddSigDigit(int value);
    void ChangeDigit(int k, int value);
    void Normalize();
    string to_string () const;
    // Friend functions
    friend ostream & operator << (ostream &, const BigInt &);
    friend istream & operator >> (istream &, BigInt &);
    friend BigInt operator + (const BigInt & lhs, const BigInt & rhs);
    friend BigInt operator - (const BigInt & lhs, const BigInt & rhs);
    BigInt operator -() const;
    friend BigInt operator * (const BigInt & lhs, const BigInt & rhs);
    friend BigInt operator * (const BigInt & lhs, int num);
    friend BigInt operator * (int num, const BigInt & rhs);
    friend BigInt operator % (const BigInt & lhs, const BigInt & rhs);
    friend BigInt operator % (const BigInt & lhs, int num);
    friend BigInt operator / (const BigInt & lhs, const BigInt & rhs);
    friend BigInt operator / (const BigInt & lhs, int num);

    friend bool operator == (const BigInt & lhs, const BigInt & rhs);
    friend bool operator <  (const BigInt & lhs, const BigInt & rhs);
    friend bool operator != (const BigInt & lhs, const BigInt & rhs);
    friend bool operator >  (const BigInt & lhs, const BigInt & rhs);
    friend bool operator >= (const BigInt & lhs, const BigInt & rhs);
    friend bool operator <= (const BigInt & lhs, const BigInt & rhs);

private:
    enum Sign { positive, negative };
    Sign mySign; // is number positive or negative
    vector<char> myDigits; // stores all digits of number
    int myNumDigits; // stores # of digits of number
};

// Constructors
BigInt::BigInt() : mySign(positive), myNumDigits(0) {}

BigInt::BigInt(const std::string &str) {
    if (str[0] == '-') {
        mySign = negative;
        myDigits.assign(str.begin() + 1, str.end());
    } else {
        mySign = positive;
        myDigits.assign(str.begin(), str.end());
    }
    std::reverse(myDigits.begin(), myDigits.end());
    myNumDigits = myDigits.size();
}

BigInt::BigInt(int num) {
    if (num < 0) {
        mySign = negative;
        num = -num;
    } else {
        mySign = positive;
    }
    while (num > 0) {
        myDigits.push_back(num % 10 + '0');
        num /= 10;
    }
    myNumDigits = myDigits.size();
}

// Member functions
bool BigInt::IsNegative() const {
    return mySign == negative;
}

bool BigInt::IsPositive() const {
    return mySign == positive;
}

int BigInt::NumDigits() const {
    return myNumDigits;
}

int BigInt::GetDigit(int k) const {
    if (k < 0 || k >= myNumDigits) {
        return 0;
    }
    return myDigits[k] - '0';
}

void BigInt::AddSigDigit(int value) {
    myDigits.push_back(value + '0');
    myNumDigits++;
}

void BigInt::ChangeDigit(int k, int value) {
    if (k >= 0 && k < myNumDigits) {
        myDigits[k] = value + '0';
    }
}

void BigInt::Normalize() {
    while (myNumDigits > 1 && myDigits.back() == '0') {
        myDigits.pop_back();
        myNumDigits--;
    }
    if (myNumDigits == 1 && myDigits[0] == '0') {
        mySign = positive;
    }
}

string BigInt::to_string() const {
    string result;
    if (mySign == negative) {
        result.push_back('-');
    }
    for (int i = myNumDigits - 1; i >= 0; i--) {
        result.push_back(myDigits[i]);
    }
    return result;
}

// Friend functions
std::ostream & operator <<(std::ostream &out, const BigInt &num) {
    if (num.mySign == BigInt::negative) {
        out << '-';
    }
    for (int i = num.myNumDigits - 1; i >= 0; i--) {
        out << num.myDigits[i];
    }
    return out;
}

std::istream & operator >>(std::istream &in, BigInt &num) {
    std::string str;
    in >> str;
    num = BigInt(str);
    return in;
}

BigInt BigInt::operator -() const {
    BigInt result = *this;
    if (result.myNumDigits != 0 && !(result.myNumDigits == 1 && result.myDigits[0] == '0')) {
        result.mySign = (mySign == positive) ? negative : positive;
    }
    return result;
}

BigInt operator +(const BigInt &lhs, const BigInt &rhs) {
    if (lhs.mySign == rhs.mySign) {
        BigInt result;
        result.mySign = lhs.mySign;
        int carry = 0;
        int maxDigits = std::max(lhs.myNumDigits, rhs.myNumDigits);
        for (int i = 0; i < maxDigits; i++) {
            int sum = lhs.GetDigit(i) + rhs.GetDigit(i) + carry;
            result.AddSigDigit(sum % 10);
            carry = sum / 10;
        }
        if (carry > 0) {
            result.AddSigDigit(carry);
        }
        result.Normalize();
        return result;
    } 
    else if (lhs.mySign == BigInt::positive) {
        return lhs - (-rhs);
    } 
    else {
        return rhs - (-lhs);
    }
}

BigInt operator -(const BigInt &lhs, const BigInt &rhs) {
    if (lhs.mySign == rhs.mySign) {
        if (lhs.mySign == BigInt::positive) {
            if (lhs >= rhs) {
                BigInt result;
                result.mySign = BigInt::positive;
                int borrow = 0;
                for (int i = 0; i < lhs.myNumDigits; i++) {
                    int diff = lhs.GetDigit(i) - rhs.GetDigit(i) - borrow;
                    if (diff < 0) {
                        diff += 10;
                        borrow = 1;
                    } 
                    else {
                        borrow = 0;
                    }
                    result.AddSigDigit(diff);
                }
                result.Normalize();
                return result;
            } 
            else {
                return -(rhs - lhs);
            }
        } 
        else {
            return -((-lhs) - (-rhs));
        }
    } 
    else if (lhs.mySign == BigInt::positive) {
        return lhs + (-rhs);
    } 
    else {
        return -((-lhs) + rhs);
    }
}

BigInt operator *(const BigInt &lhs, const BigInt &rhs) {
    if (lhs == BigInt(0) || rhs == BigInt(0)) {
        return BigInt(0);
    }
    if (lhs == BigInt(1) || rhs == BigInt(1)) {
        return (lhs == BigInt(1)) ? rhs : lhs;
    }
    BigInt result;
    result.mySign = (lhs.mySign == rhs.mySign) ? BigInt::positive : BigInt::negative;
    result.myDigits.resize(lhs.myNumDigits + rhs.myNumDigits, '0');
    result.myNumDigits = result.myDigits.size();

    for (int i = 0; i < lhs.myNumDigits; i++) {
        int carry = 0;
        for (int j = 0; j < rhs.myNumDigits; j++) {
            int product = (lhs.GetDigit(i) * rhs.GetDigit(j)) + (result.GetDigit(i + j)) + carry;
            result.ChangeDigit(i + j, product % 10);
            carry = product / 10;
        }
        if (carry > 0) {
            result.ChangeDigit(i + rhs.myNumDigits, result.GetDigit(i + rhs.myNumDigits) + carry);
        }
    }
    result.Normalize();
    return result;
}

BigInt operator *(const BigInt &lhs, int num) {
    return lhs * BigInt(num);
}

BigInt operator *(int num, const BigInt &rhs) {
    return BigInt(num) * rhs;
}

BigInt operator /(const BigInt &lhs, const BigInt &rhs) {
    if (rhs == BigInt(0)) {
        throw std::runtime_error("Division by zero");
    }
    if (lhs < rhs) {
        return BigInt(0);
    }
    if (lhs == rhs) {
        return BigInt(1);
    }
    if (rhs == BigInt(1)) {
        return lhs;
    }
    BigInt result;
    result.mySign = (lhs.mySign == rhs.mySign) ? BigInt::positive : BigInt::negative;

    BigInt devidend(lhs);
    BigInt devisor(rhs);
    BigInt quotient(0);

    devidend.mySign = devisor.mySign = BigInt::positive;
    
    while (devidend >= devisor) {
        devidend = devidend - devisor;
        quotient = quotient + BigInt(1);
    }
    quotient.mySign = result.mySign;
    quotient.Normalize();

    return quotient;
}

BigInt operator /(const BigInt &lhs, int num) {
    return lhs / BigInt(num);
}

BigInt operator %(const BigInt &lhs, const BigInt &rhs) {
    BigInt result = lhs - (lhs / rhs) * rhs;
    if (result.mySign == BigInt::negative) {
        result = result + rhs;
    }
    return result;
}

BigInt operator %(const BigInt &lhs, int num) {
    return lhs % BigInt(num);
}


bool operator == (const BigInt &lhs, const BigInt &rhs) {
    return lhs.mySign == rhs.mySign && lhs.myDigits == rhs.myDigits;
}

bool operator <  (const BigInt &lhs, const BigInt &rhs) {
    if (lhs.mySign != rhs.mySign) {
        return lhs.mySign == BigInt::negative;
    }
    if (lhs.myNumDigits != rhs.myNumDigits) {
        return lhs.myNumDigits < rhs.myNumDigits;
    }
    for (int i = lhs.myNumDigits - 1; i >= 0; i--) {
        if (lhs.myDigits[i] != rhs.myDigits[i]) {
            return lhs.myDigits[i] < rhs.myDigits[i];
        }
    }
    return false;
}

bool operator != (const BigInt &lhs, const BigInt &rhs) {
    return !(lhs == rhs);
}

bool operator >  (const BigInt &lhs, const BigInt &rhs) {
    return rhs < lhs;
}

bool operator >= (const BigInt &lhs, const BigInt &rhs) {
    return !(lhs < rhs);
}

bool operator <= (const BigInt &lhs, const BigInt &rhs) {
    return !(rhs < lhs);
}

// int main() {
//     BigInt num1("4");
//     BigInt num2("200");

//     std::cout << "Num1: " << num1 << std::endl;
//     std::cout << "Num2: " << num2 << std::endl;

//     BigInt sum = num1 + num2;
//     std::cout << "Sum: " << sum << std::endl;

//     BigInt diff = num1 - num2;
//     std::cout << "Difference: " << diff << std::endl;

//     BigInt product = num1 * num2;
//     std::cout << "Product: " << product << std::endl;

//     BigInt quotient = num1 / num2;
//     std::cout << "Quotient: " << quotient << std::endl;

//     BigInt remainder = num1 % num2;
//     std::cout << "Remainder: " << remainder << std::endl;


//     return 0;
// }