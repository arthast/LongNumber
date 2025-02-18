#include <gtest/gtest.h>
#include "../src/LongNumber.hpp"

TEST(binary_sub, sub1) {
    LongNumber first("1100");
    LongNumber second("0110");
    LongNumber ans("110");
    ASSERT_EQ((first - second), ans);
}

TEST(binary_sub, sub2) {
    LongNumber first(100);
    LongNumber second(-90);
    LongNumber ans(190);
    ASSERT_EQ((first - second), ans);
}

TEST(binary_sub, sub3) {
    LongNumber first("111011.11101");
    LongNumber second("1.00011");
    LongNumber ans("111010.1101");
    ASSERT_EQ((first - second), ans);
}

TEST(binary_sub, sub4) {
    LongNumber first("-1100");
    LongNumber second("-10.10");
    LongNumber ans("-1001.1");
    ASSERT_EQ((first - second), ans);
}

TEST(binary_sub, sub5) {
    LongNumber first("0.111010100101010010111110011");
    LongNumber second("0.0001001101101010101100101011");
    LongNumber ans("0.1101011011101010000010111011");
    ASSERT_EQ((first - second), ans);
}