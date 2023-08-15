#include "gtest/gtest.h"

// indicate that this is a main program for testing
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
