#include <gtest/gtest.h>
#include "../src/LongNumber.hpp"

TEST(binary_plus, plus1) {
    LongNumber first("1100");
    LongNumber second("0110");
    LongNumber ans("10010");
    ASSERT_EQ((first + second), ans);
}

TEST(binary_plus, plus2) {
    LongNumber first(100);
    LongNumber second(-90);
    LongNumber ans(10);
    ASSERT_EQ((first + second), ans);
}

TEST(binary_plus, plus3) {
    LongNumber first("111011.11101");
    LongNumber second("1.00011");
    LongNumber ans("111101");
    ASSERT_EQ((first + second), ans);
}

TEST(binary_plus, plus4) {
    LongNumber first("-1100");
    LongNumber second("-10.10");
    LongNumber ans("-1110.10");
    ASSERT_EQ((first + second), ans);
}

TEST(binary_plus, plus5) {
    LongNumber first("0.111010100101010010111110011");
    LongNumber second("0.0001001101101010101100101011");
    LongNumber ans("0.1111110110111111011100010001");
    ASSERT_EQ((first + second), ans);
}