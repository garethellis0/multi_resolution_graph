#include <gtest/gtest.h>
#include <RealNode.h>
#include <memory>

class RealNodeTest : public testing::Test {
protected:
    virtual void SetUp() {
    }
};

TEST_F(RealNodeTest, emptyConstructorTest) {
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

