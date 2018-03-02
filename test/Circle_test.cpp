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
// where the corner of the node lies within the circle
TEST_F(CircleTest, overlapsNode_partially_overlapping_node_corner_in_circle){
    GraphNode<nullptr_t> node(1,10);
    Circle<nullptr_t> circle(3, (Coordinates){-1,-1});
    EXPECT_TRUE(circle.overlapsNode(node));
}

// Test checking the overlap of a partially overlapping circle and node
// where the circle intersects the bottom edge of the node twice
TEST_F(CircleTest, overlapsNode_partially_overlapping_circle_intersects_bottom_edge_twice){
    GraphNode<nullptr_t> node(1,10);
    Circle<nullptr_t> circle(1.1, (Coordinates){5,-1});
    EXPECT_TRUE(circle.overlapsNode(node));
}

// Test checking the overlap of a partially overlapping circle and node
// where the circle intersects the left edge of the node twice
TEST_F(CircleTest, overlapsNode_partially_overlapping_circle_intersects_left_edge_twice){
    GraphNode<nullptr_t> node(1,10);
    Circle<nullptr_t> circle(1.1, (Coordinates){11,5});
    EXPECT_TRUE(circle.overlapsNode(node));
}

// Test checking the overlap of a partially overlapping circle and node
// where the circle intersects the right edge of the node twice
TEST_F(CircleTest, overlapsNode_partially_overlapping_circle_intersects_right_edge_twice){
    GraphNode<nullptr_t> node(1,10);
    Circle<nullptr_t> circle(1.1, (Coordinates){-1,5});
    EXPECT_TRUE(circle.overlapsNode(node));
}

// Test checking the overlap of a partially overlapping circle and node
// where the circle intersects the top edge of the node twice
TEST_F(CircleTest, overlapsNode_partially_overlapping_circle_intersects_top_edge_twice){
    GraphNode<nullptr_t> node(1,10);
    Circle<nullptr_t> circle(1.1, (Coordinates){5,11});
    EXPECT_TRUE(circle.overlapsNode(node));
}

// Test checking the overlap of a Circle that is completely within a Node
TEST_F(CircleTest, overlapsNode_circle_within_node){
    GraphNode<nullptr_t> node(1,10);
    Circle<nullptr_t> circle(1, (Coordinates){5,5});
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

