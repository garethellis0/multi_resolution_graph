// Testing Includes
#include <gtest/gtest.h>

// Thunderbots Includes
#include "GraphNode.h"
#include "Circle.h"

class CircleTest : public testing::Test {
protected:
    virtual void SetUp() {
    }
};

// Test checking the overlap of a Circle and a node that do not overlap at all
TEST_F(CircleTest, overlapsNode_not_overlapping){
    GraphNode<nullptr_t> node(1,10);
    Circle<nullptr_t> circle(3, (Coordinates){15,15});
    EXPECT_FALSE(circle.overlapsNode(node));
}

// Test checking the overlap of a partially overlapping circle and node
TEST_F(CircleTest, overlapsNode_partially_overlapping){
    GraphNode<nullptr_t> node(1,10);
    Circle<nullptr_t> circle(3, (Coordinates){-1,-1});
    EXPECT_TRUE(circle.overlapsNode(node));
}

// Test checking the overlap of a Circle that is completely within a Node
TEST_F(CircleTest, overlapsNode_circle_within_node){
    GraphNode<nullptr_t> node(1,10);
    Circle<nullptr_t> circle(3, (Coordinates){1,1});
    EXPECT_TRUE(circle.overlapsNode(node));
}

// Test checking the overlap of a Circle that completely encompasses a Node
TEST_F(CircleTest, overlapsNode_node_within_circle){
    GraphNode<nullptr_t> node(1,2);
    Circle<nullptr_t> circle(5, (Coordinates){-1,-1});
    EXPECT_TRUE(circle.overlapsNode(node));
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

