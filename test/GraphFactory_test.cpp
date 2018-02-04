// Testing Includes
#include <gtest/gtest.h>

// C++ STD Includes
#include <memory>

// Thunderbots Includes
#include <GraphFactory.h>

// TODO: Some note about how intertwined GraphNode and RealNode  are (and hence the all the tests of both are)

// TODO: Finish Tests
class GraphFactoryTest : public testing::Test {
protected:
    virtual void SetUp() {
    }
};

// TODO: Test constructor to make sure if we generate a graph without setting anything we get what we expect

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

