#ifndef LONGNUMBER_LIBRARY_H
#define LONGNUMBER_LIBRARY_H

#include <algorithm>
#include <iostream>
#include <vector>

class LongNumber {
private:
    std::vector<bool> integer = {};
    std::vector<bool> fractional = {false};
    unsigned int precision = 1;
    bool sign = false;

public:
    // Constructors
    LongNumber() = default;

    explicit LongNumber(const std::string &number) {
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
            precision = std::max(static_cast<unsigned int>(fractional.size()), precision);
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

    void deleteZeros() {
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
    }

    void normalize() {
        if (precision > 100) {
            for (int i = 100; i < precision; i++)
                fractional.pop_back();
            precision = 100;
        }

        if (integer.size() == 1 && integer.back() == 0 && fractional.size() == 1 && fractional.back() == 0)
            sign = false;
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

    void makeEpsilon() {
        const unsigned int accuracy = std::max(precision, 100u);
        integer.push_back(false);

        for (int i = 0; i < accuracy; i++) {
            fractional.push_back(false);
        }
        fractional.push_back(true);
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

    friend LongNumber rightShift(const LongNumber &number);
};
LongNumber operator ""_f(const char*);
#endif //LONGNUMBER_LIBRARY_H
