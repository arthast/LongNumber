#include "LongNumber.hpp"
#include <chrono>

LongNumber LongNumber::operator+() const {
    return *this;
}

LongNumber LongNumber::operator-() const {
    LongNumber other = *this;
    other.sign = !other.sign;
    return other;
}

// Bool operators

bool operator==(const LongNumber &left, const LongNumber &right) {
    if (left.sign != right.sign) return false;
    if (left.integer.size() != right.integer.size()) return false;
    if (left.fractional.size() != right.fractional.size()) return false;

    return std::equal(left.integer.begin(), left.integer.end(), right.integer.begin()) &&
           std::equal(left.fractional.begin(), left.fractional.end(), right.fractional.begin());
}

bool operator!=(const LongNumber &left, const LongNumber &right) {
    return !(left == right);
}

bool operator<(const LongNumber &left, const LongNumber &right) {
    if (left.sign && right.sign) return -right < -left;

    if (left.sign) return true;
    if (right.sign) return false;

    if (left.integer.size() < right.integer.size()) return true;
    if (left.integer.size() > right.integer.size()) return false;

    for (int i = static_cast<int>(left.integer.size()) - 1; i >= 0; i--) {
        if (left.integer[i] < right.integer[i]) return true;
        if (left.integer[i] > right.integer[i]) return false;
    }

    for (int i = 0; i < std::min(left.fractional.size(), right.fractional.size()); i++) {
        if (left.fractional[i] < right.fractional[i]) return true;
        if (left.fractional[i] > right.fractional[i]) return false;
    }

    if (left.fractional.size() < right.fractional.size()) return true;
    return false;
}

bool operator<=(const LongNumber &left, const LongNumber &right) {
    return left < right || left == right;
}

bool operator>(const LongNumber &left, const LongNumber &right) {
    return right < left;
}

bool operator>=(const LongNumber &left, const LongNumber &right) {
    return right <= left;
}

// Binary operators

LongNumber operator+(const LongNumber &left, const LongNumber &right) {
    if (left.sign && right.sign) return -(-left + (-right));
    if (left.sign) return right - (-left);
    if (right.sign) return left - (-right);

    LongNumber sum;
    bool next = false;
    size_t n = std::min(left.integer.size(), right.integer.size());

    // Summing integer part
    for (int i = 0; i < n; i++) {
        int now = left.integer[i] + right.integer[i] + next;
        sum.integer.push_back(now % 2);
        next = now / 2;
    }

    for (size_t i = n; i < left.integer.size(); i++) {
        int now = left.integer[i] + next;
        sum.integer.push_back(now % 2);
        next = now / 2;
    }

    for (size_t i = n; i < right.integer.size(); i++) {
        int now = right.integer[i] + next;
        sum.integer.push_back(now % 2);
        next = now / 2;
    }
    sum.integer.push_back(next);

    // Summing fractional part
    sum.precision = std::max(left.precision, right.precision);
    n = std::min(left.fractional.size(), right.fractional.size());

    if (left.fractional.size() > right.fractional.size()) {
        for (size_t i = left.fractional.size() - 1; i >= n; i--) {
            sum.fractional.push_back(left.fractional[i]);
        }
    } else if (right.fractional.size() > left.fractional.size()) {
        for (size_t i = right.fractional.size() - 1; i >= n; i--) {
            sum.fractional.push_back(right.fractional[i]);
        }
    }

    next = false;
    for (int i = static_cast<int>(n) - 1; i >= 0; i--) {
        int now = left.fractional[i] + right.fractional[i] + next;
        sum.fractional.push_back(now % 2);
        next = now / 2;
    }
    std::reverse(sum.fractional.begin(), sum.fractional.end());

    // If we got an integer part in summing two fractional parts
    int index = 0;
    while (next && index < sum.fractional.size()) {
        if (*(sum.integer.begin() + index++)) {
            *(sum.integer.begin() + index - 1) = false;
        } else {
            next = false;
            *(sum.integer.begin() + index - 1) = true;
        }
    }
    if (next)
        sum.integer.push_back(true);

    sum.deleteZeros();
    return sum;
}

LongNumber operator-(const LongNumber &left, const LongNumber &right) {
    if (left.sign && right.sign) return -right - (-left);
    if (left.sign) return -(-left + right);
    if (right.sign) return left + (-right);

    if (left < right) return -(right - left);

    LongNumber diff = left;

    // Update precision
    while (diff.fractional.size() < right.fractional.size())
        diff.fractional.push_back(false);
    diff.precision = std::max(static_cast<unsigned int>(diff.fractional.size()), diff.precision);

    int sub = 0;
    for (int i = static_cast<int>(right.fractional.size()) - 1; i >= 0; i--) {
        sub += right.fractional[i];
        if (sub == 2) {
            sub = 1;
        } else if (sub == 1) {
            if (!diff.fractional[i]) {
                diff.fractional[i] = true;
            } else {
                diff.fractional[i] = false;
                sub = 0;
            }
        }
    }

    for (int i = 0; i < right.integer.size(); i++) {
        sub += right.integer[i];
        if (sub == 2) {
            sub = 1;
        } else if (sub == 1) {
            if (!diff.integer[i]) {
                diff.integer[i] = true;
            } else {
                diff.integer[i] = false;
                sub = 0;
            }
        }
    }

    int ind = static_cast<int>(right.integer.size());
    while (sub) {
        if (diff.integer[ind]) {
            diff.integer[ind] = false;
            sub = 0;
        } else {
            diff.integer[ind] = true;
        }
        ++ind;
    }
    diff.deleteZeros();

    return diff;
}

LongNumber operator*(const LongNumber &left, const LongNumber &right) {
    if (left.sign && right.sign) return -right * -left;

    int intSize = static_cast<int>(left.integer.size() + right.integer.size());
    int fracSize = static_cast<int>(left.fractional.size() + right.fractional.size());
    int maxSize = intSize + fracSize;
    std::vector<int> result(maxSize, 0);

    for (int i = static_cast<int>(right.fractional.size()) - 1; i >= 0; i--) {
        if (!right.fractional[i]) continue;

        for (int j = static_cast<int>(left.fractional.size()) - 1; j >= 0; j--) {
            result[fracSize - (i + j) - 2] += right.fractional[i] * left.fractional[j];
        }

        for (int j = 0; j < static_cast<int>(left.integer.size()); j++) {
            result[j + fracSize - i - 1] += right.fractional[i] * left.integer[j];
        }
    }

    for (int i = 0; i < static_cast<int>(right.integer.size()); i++) {
        if (!right.integer[i]) continue;

        for (int j = static_cast<int>(left.fractional.size()) - 1; j >= 0; j--) {
            result[i + fracSize - j - 1] += right.integer[i] * left.fractional[j];
        }

        for (int j = 0; j < static_cast<int>(left.integer.size()); j++) {
            result[i + j + fracSize] += right.integer[i] * left.integer[j];
        }
    }

    for (int i = 0; i < maxSize - 1; ++i) {
        result[i + 1] += result[i] / 2;
        result[i] %= 2;
    }
    result.push_back(result[maxSize - 1] / 2);
    result[maxSize - 1] %= 2;

    LongNumber composition;
    composition.sign = (left.sign || right.sign);
    composition.fractional.pop_back();

    for (int i = fracSize - 1; i >= 0; i--) {
        composition.fractional.push_back(result[i]);
    }

    for (int i = fracSize; i < maxSize; i++) {
        composition.integer.push_back(result[i]);
    }

    composition.precision = std::max(left.fractional.size(), right.fractional.size());
    composition.normalize();
    composition.deleteZeros();

    return composition;
}

inline LongNumber min(const LongNumber &left, const LongNumber &right) {
    return left < right ? left : right;
}

inline LongNumber max(const LongNumber &left, const LongNumber &right) {
    return left > right ? left : right;
}

LongNumber rightShift(const LongNumber &middle, unsigned int maxPrecision) {
    LongNumber number = middle;

    bool temp = number.integer[0];
    for (int i = 1; i < number.integer.size(); i++) {
        number.integer[i - 1] = number.integer[i];
    }
    number.integer.pop_back();

    number.fractional.push_back(false);
    for (int i = static_cast<int>(number.fractional.size() - 1); i > 0; --i) {
        number.fractional[i] = number.fractional[i - 1];
    }
    number.fractional[0] = temp;
    number.precision++;
    number.precision = std::min(number.precision, maxPrecision);
    number.normalize();

    return number;
}

LongNumber operator/(const LongNumber &left, const LongNumber &right) {
    if (right.integer.size() == 1 && !right.integer[0] && right.fractional.size() == 1 && !right.fractional[0])
        throw std::invalid_argument("Divide by zero");

    if (left.sign && right.sign) return -right / -left;

    LongNumber l = -(max(left, right) + LongNumber("1"));
    LongNumber r = max(left, right) + LongNumber("1");
    int cnt = 0;

    unsigned maxPresicion = std::max({left.precision, right.precision, ACCURACY + 1});
    while (cnt < 345) {
        LongNumber middle = rightShift(l + r, maxPresicion);
        middle.deleteZeros();
        if ((middle * right) < left)
            l = middle;
        else
            r = middle;
        cnt++;
    }
    r.precision = ACCURACY;
    r.normalize();
    r.deleteZeros();
    return r;
}

LongNumber &LongNumber::operator+=(const LongNumber &other) {
    return (*this = *this + other);
}

LongNumber &LongNumber::operator-=(const LongNumber &other) {
    return (*this = *this - other);
}

LongNumber &LongNumber::operator*=(const LongNumber &other) {
    return (*this = *this * other);
}

LongNumber &LongNumber::operator/=(const LongNumber &other) {
    return (*this = *this / other);
}

LongNumber operator ""_longnumber(const char *string) {
    return LongNumber(string);
}

std::ostream &operator<<(std::ostream &os, const LongNumber &number) {
    bool sign = number.sign;
    if (number.integer.empty()) {
        if (sign) {
            if (!number.fractional.empty())
                os << "-";
            sign = false;
        }
        os << "0";
    }
    os << number.toString(sign);

    int cnt = 0;
    while (number.fractional.size() + cnt++ < number.precision)
        os << "0";
    return os;
}

std::istream &operator>>(std::istream &is, LongNumber &number) {
    std::string s;
    is >> s;
    number = LongNumber(s);
    return is;
}

void calc_PI(int n) {
    LongNumber Pi(0);
    LongNumber pow(1);
    LongNumber sixten(16);

    for(int i = 0; i < n; i++) {
        LongNumber num1((LongNumber(4) / LongNumber(8 * i + 1)));
        LongNumber num2((LongNumber(2) / LongNumber(8 * i + 4)));
        LongNumber num3((LongNumber(1) / LongNumber(8 * i + 5)));
        LongNumber num4((LongNumber(1) / LongNumber(8 * i + 6)));
        Pi += (num1 - num2 - num3 - num4) / pow;
        pow *= sixten;
    }
    std::cout << Pi << '\n';
}
