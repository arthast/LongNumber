#include <gtest/gtest.h>
#include "../src/LongNumber.hpp"

TEST(bool_, equal) {
    LongNumber first("110011.1101");
    LongNumber second("110011.1101");
    ASSERT_EQ((first == second), true);
}

TEST(bool_, neg_equal) {
    LongNumber first("110011.1101");
    LongNumber second("-110011.1101");
    ASSERT_EQ((first != second), true);
}

TEST(bool_, more) {
    LongNumber first("110011.110");
    LongNumber second("110000.1101");
    ASSERT_EQ((first > second), true);
}

TEST(bool_, less) {
    LongNumber first("100010.110111");
    LongNumber second("110011.1101");
    ASSERT_EQ((first < second), true);
}

TEST(bool_, more_equal) {
    LongNumber first("110011.1101");
    LongNumber second("110011.1101");
    ASSERT_EQ((first >= second), true);
}

TEST(bool_, less_equal) {
    LongNumber first("110000.1101");
    LongNumber second("110010.1101");
    ASSERT_EQ((first <= second), true);
}

TEST(bool_, double_equal) {
    LongNumber first(101.267);
    LongNumber second(101.267);
    ASSERT_EQ((first == second), true);
}

TEST(bool_, longlong_equal) {
    long long a = 198735362746453, b = 198735362746453;
    LongNumber first(a);
    LongNumber second(b);
    ASSERT_EQ((first == second), true);
}