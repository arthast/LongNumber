#ifndef LONGNUMBER_LIBRARY_H
#define LONGNUMBER_LIBRARY_H

#include <algorithm>
#include <iostream>
#include <vector>

inline unsigned ACCURACY = 20;

class LongNumber {
private:
    std::vector<char> integer = {};
    std::vector<char> fractional = {false};
    unsigned int precision = 1;
    bool sign = false;

public:
    // Constructors
    LongNumber() = default;

    void convertFromString(const std::string &number) {
        int end = 0;
        if (number[0] == '-' || number[0] == '+') {
            sign = number[0] == '-';
            end = 1;
        }

        const size_t dotPosition = number.find('.');
        int CurrentIndex = static_cast<int>(number.size()) - 1;

        // Get fractional part
        if (dotPosition != std::string::npos) {
            fractional.pop_back();
            while (CurrentIndex > dotPosition) {
                size_t index = dotPosition + number.size() - CurrentIndex--;
                fractional.push_back(number[index] == '1');
            }
            precision = std::max(static_cast<unsigned>(fractional.size()), precision);
        }

        if (dotPosition != std::string::npos) CurrentIndex--;

        // Get integer part
        while (CurrentIndex >= end) {
            integer.push_back(number[CurrentIndex--] == '1');
        }

        integer.push_back(false);
        deleteZeros();
        normalize();
    }

    explicit LongNumber(const std::string &number) {
        convertFromString(number);
    }

    explicit LongNumber(long double number) {
        std::string binary;

        if (number < 0) {
            binary = "-";
            number *= -1;
        }

        long long intPart = static_cast<long long>(number);
        long double fracPart = number - intPart;
        while (intPart != 0) {
            binary += static_cast<char>((intPart % 2) + '0');
            intPart /= 2;
        }
        std::reverse(binary.begin() + (binary[0] == '-'), binary.end());
        if (fracPart > 0) {
            binary += '.';
        }

        int cnt = 0;
        while (cnt <= ACCURACY && fracPart != 0) {
            fracPart *= 2;
            binary += static_cast<char>((fracPart >= 1) + '0');
            fracPart -= fracPart >= 1;
            cnt++;
        }

        if (cnt > ACCURACY) {
            int prev = binary[binary.size() - 1] - '0';
            binary.pop_back();
            for (int i = static_cast<int>(binary.size()) - 1; i >= 0; i--) {
                if (binary[i] == '.') continue;
                if (binary[i] == '1') {
                    if (prev == '1')
                        binary[i] = '0';
                } else {
                    binary[i] = prev ? '1' : '0';
                    prev = 0;
                }
            }
            binary.insert(binary.begin(), prev + '0');
        }
        convertFromString(binary);
    }

    explicit LongNumber(long long number) : LongNumber(static_cast<long double>(number)) {}
    explicit LongNumber(int number) : LongNumber(static_cast<long double>(number)) {}
    explicit LongNumber(unsigned int number) : LongNumber(static_cast<long double>(number)) {}
    explicit LongNumber(double number) : LongNumber(static_cast<long double>(number)) {}


    // Copy constructor
    LongNumber(const LongNumber &other) {
        sign = other.sign;
        integer = other.integer;
        fractional = other.fractional;
        precision = other.precision;
    }

    // Copy operator
    LongNumber &operator=(const LongNumber &other) {
        sign = other.sign;
        integer = other.integer;
        fractional = other.fractional;
        precision = other.precision;
        return *this;
    }

    // Destructor
    ~LongNumber() = default;

    LongNumber& deleteZeros() {
        while (integer.size() > 1 && integer.back() == 0) {
            integer.pop_back();
        }
        if (integer.empty())
            integer.push_back(false);

        while (fractional.size() > 1 && fractional.back() == 0) {
            fractional.pop_back();
        }
        if (fractional.empty())
            fractional.push_back(false);

        precision = fractional.size();
        return *this;
    }

    void normalize() {
        while (fractional.size() > precision) {
            fractional.pop_back();
        }

        if (integer.size() == 1 && integer.back() == 0 && fractional.size() == 1 && fractional.back() == 0)
            sign = false;
    }

    void setPrecision(const unsigned int newPrecision) {
        precision = newPrecision;
    }

    unsigned int getPrecision() const {
        return precision;
    }

    // Cast to string
    [[nodiscard]] std::string toString(bool newSign) const {
        std::string result;
        if (newSign == 1)
            result += "-";

        for (const auto digit: integer)
            result += static_cast<char>(digit + '0');
        std::reverse(result.begin() + (newSign), result.end());

        result += ".";

        for (const auto digit: fractional)
            result += static_cast<char>(digit + '0');

        return result;
    }

    LongNumber operator-() const;

    LongNumber operator+() const;

    LongNumber &operator+=(const LongNumber &other);

    LongNumber &operator-=(const LongNumber &other);

    LongNumber &operator*=(const LongNumber &other);

    LongNumber &operator/=(const LongNumber &other);

    friend LongNumber operator+(const LongNumber &left, const LongNumber &right);

    friend LongNumber operator-(const LongNumber &left, const LongNumber &right);

    friend LongNumber operator*(const LongNumber &left, const LongNumber &right);

    friend LongNumber operator/(const LongNumber &left, const LongNumber &right);

    friend bool operator==(const LongNumber &left, const LongNumber &right);

    friend bool operator!=(const LongNumber &left, const LongNumber &right);

    friend bool operator<(const LongNumber &left, const LongNumber &right);

    friend bool operator>(const LongNumber &left, const LongNumber &right);

    friend bool operator<=(const LongNumber &left, const LongNumber &right);

    friend bool operator>=(const LongNumber &left, const LongNumber &right);

    friend std::istream &operator>>(std::istream &is, LongNumber &number);

    friend std::ostream &operator<<(std::ostream &os, const LongNumber &number);

    friend LongNumber rightShift(const LongNumber &number, unsigned int maxPrecision);
};

LongNumber operator ""_longnumber(const char *);
#endif //LONGNUMBER_LIBRARY_H
