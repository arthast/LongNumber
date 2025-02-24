#ifndef LONGNUMBER_LIBRARY_H
#define LONGNUMBER_LIBRARY_H

#include <algorithm>
#include <iostream>
#include <vector>

inline unsigned ACCURACY = 20;
const std::string NUMBER_PI = "3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679";

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

    static std::string TenPlusInt(const std::string& left, const std::string& right) {
        std::string result;

        const unsigned n = std::max(left.size(), right.size());
        int next = 0;
        for (int i = 0; i < n; i++) {
            const int num = (i < left.size() ? (left[i] - '0') : 0) + (i < right.size() ? (right[i] - '0') : 0) + next;
            result += static_cast<char>(num % 10 + '0');
            next = num / 10;
        }
        if (next)
            result += static_cast<char>(next + '0');

        return result;
    }

    static std::string multTwo(const std::string &number) {
        std::string result;

        int next = 0;
        for (const char i : number) {
            const int num = (i - '0') * 2 + next;
            next = num / 10;
            result += static_cast<char>(num % 10 + '0');
        }

        if (next)
            result += static_cast<char>(next + '0');

        return result;
    }

    static std::string devTwo(const std::string &number) {
        std::string result;

        int next = 0;
        for (const char i : number) {
            const int num = (i - '0') + next * 10;
            next = num % 2;
            result += static_cast<char>(num / 2 + '0');
        }
        if (next)
            result += '5';

        return result;
    }

    static std::string TenPlusFrac(const std::string& left, const std::string& right) {
        std::string result;

        if (left.size() > right.size()) {
            for (int i = 0; i < static_cast<int>(right.size()); i++)
                result += left[i];
        } else {
            for (int i = static_cast<int>(right.size()) - 1; i >= static_cast<int>(left.size()); i--)
                result += right[i];
        }

        int n = static_cast<int>(std::min(left.size(), right.size()));
        int next = 0;
        for (int i = n - 1; i >= 0; i--) {
            const int num = (left[left.size() - i - 1] - '0') + (right[i] - '0') + next;
            result += static_cast<char>(num % 10 + '0');
            next = num / 10;
        }

        if (next)
            result += "!";

        return result;
    }

    // Cast to ten system
    [[nodiscard]] std::string toTen(const unsigned int precision) const {
        std::string resultInt;
        std::string resultFrac;

        std::string pow2 = "5";
        for (char i : fractional) {
            resultFrac = i ? TenPlusFrac(resultFrac, pow2) : resultFrac;
            pow2 = devTwo(pow2);
        }

        std::string fracInt = resultFrac.back() == '!' ? "1" : "0";
        if (resultFrac.back() == '!')
            resultFrac.pop_back();

        std::reverse(resultFrac.begin(), resultFrac.end());
        if (resultFrac.empty())
            resultFrac += "0";

        for (int i = static_cast<int>(integer.size()) - 1; i >= 0; i--) {
            resultInt = multTwo(resultInt);
            resultInt = TenPlusInt(resultInt, integer[i] ? "1": "0");
        }
        resultInt = TenPlusInt(resultInt, fracInt);
        std::reverse(resultInt.begin(), resultInt.end());

        while (resultFrac.size() > precision)
            resultFrac.pop_back();

        if (sign)
            return "-" + resultInt + "." + resultFrac;
        return resultInt + "." + resultFrac;
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
