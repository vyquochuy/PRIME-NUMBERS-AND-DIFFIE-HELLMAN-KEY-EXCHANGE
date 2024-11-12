#ifndef BIGINT_H
#define BIGINT_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class BigInt {
public:
    // Constructors
    BigInt();
    BigInt(const std::string &);
    BigInt(long long);

    // Member functions
    bool IsNegative() const;
    bool IsPositive() const;
    int NumDigits() const;
    int GetDigit(int k) const;
    void AddSigDigit(int value);
    void ChangeDigit(int k, int value);
    void Normalize();
    std::string to_string() const;
    int lastDigit() const;

    // Friend functions
    friend std::ostream & operator << (std::ostream &, const BigInt &);
    friend std::istream & operator >> (std::istream &, BigInt &);
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

    // New shift operators
    friend BigInt operator << (const BigInt & lhs, int shift);
    friend BigInt operator >> (const BigInt & lhs, int shift);
    friend BigInt operator |  (const BigInt & lhs, const BigInt & rhs);

private:
    enum Sign { positive, negative };   // Enumeration for sign
    Sign mySign;                        // Sign of the number (positive or negative)
    std::vector<char> myDigits;         // Digits of the number in reverse order
    int myNumDigits;                    // Number of digits
};
BigInt Pow(int t, const BigInt & rhs);
#endif // BIGINT_H