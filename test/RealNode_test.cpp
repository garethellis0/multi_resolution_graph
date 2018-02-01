#include <gtest/gtest.h>
#include <RealNode.h>
#include <memory>

// TODO: Some note about how intertwined GraphNode and RealNode  are (and hence the all the tests of both are)

// TODO: Finish Tests
class RealNodeTest : public testing::Test {
protected:
    virtual void SetUp() {
    }
};

TEST_F(RealNodeTest, getClosestNodeToCoordinates_Test){
    RealNode realNode(new GraphNode);
    boost::optional<RealNode*> closestNode = realNode.getClosestNodeToCoordinates(
            Coordinates{10,10}
    );
    // We should have been able to find at least one node
    bool foundNode = closestNode.is_initialized();
    ASSERT_TRUE(foundNode);
    // Since this is a RealNode, the closest node is itself
    EXPECT_EQ(&realNode, closestNode);
}

TEST_F(RealNodeTest, getClosestNodeThatPassesFilter_passes_filter_Test){

}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

