// Testing Includes
#include <gtest/gtest.h>
#include <gmock/gmock.h>

// C++ STD Includes

// Thunderbots Includes

class WorldFactoryTest : public testing::Test {
protected:
    virtual void SetUp() {
    }
};

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

