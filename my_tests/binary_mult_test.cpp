#include <gtest/gtest.h>
#include "../src/LongNumber.hpp"

TEST(binary_mult, mult1) {
    LongNumber first("110001");
    LongNumber second("1");
    LongNumber ans("110001");
    ASSERT_EQ((first * second), ans);
}

TEST(binary_mult, mult2) {
    LongNumber first(150);
    LongNumber second(10);
    LongNumber ans(1500);
    ASSERT_EQ((first * second), ans);
}

TEST(binary_mult, mult3) {
    LongNumber first("101.01");
    LongNumber second("10.10");
    LongNumber ans("1101");
    ASSERT_EQ((first * second), ans);
}

TEST(binary_mult, mult4) {
    LongNumber first(124123);
    LongNumber second(841784);
    LongNumber ans(104484755432ll);
    ASSERT_EQ((first * second), ans);
}

TEST(binary_mult, mult5) {
    LongNumber first("101010010111.11010011");
    LongNumber second("11110100101.1100101011");
    LongNumber ans("10100010000001100000011.1011110101");
    ASSERT_EQ((first * second), ans);
}