#include <gtest/gtest.h>
#include "../src/LongNumber.hpp"

TEST(binary_dev, dev1) {
    LongNumber first("110001");
    LongNumber second("1");
    LongNumber ans("110001");
    ASSERT_EQ((first / second), ans);
}

TEST(binary_dev, dev2) {
    LongNumber first(150);
    LongNumber second(10);
    LongNumber ans(15);
    ASSERT_EQ((first / second), ans);
}

TEST(binary_dev, dev3) {
    LongNumber first("101.01");
    LongNumber second("10");
    LongNumber ans("10.101");
    ASSERT_EQ((first / second), ans);
}

TEST(binary_dev, dev4) {
    LongNumber first(16576212);
    LongNumber second(100);
    LongNumber ans(165762.12);
    ASSERT_EQ((first / second), ans);
}

TEST(binary_dev, dev5) {
    LongNumber first("101010010111.11010011");
    LongNumber second("11110100101.1100101011");
    LongNumber ans("1.01100010100110001101");
    ASSERT_EQ((first / second), ans);
}
