//
// Created by Kostya on 10/4/2023.
//

#include <gtest/gtest.h>

TEST(TestGroupName, Subtest_1) {
    ASSERT_TRUE(1 == 1);
}

TEST(TestGroupName, Subtest_2) {
    ASSERT_TRUE('b' == 'b');
    std::cout << "continue test after failure" << std::endl;
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}